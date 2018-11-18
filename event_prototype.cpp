// EventPrototype.cpp : Defines the entry point for the console application.
//

#include <string>
#include <map>
#include <vector>
#include <set>
#include <chrono>
#include <algorithm>
#include <atomic>
#include <thread>
//#include <omp.h>

#pragma region Helpers
double timeGetCurrentSeconds()
{
	long long v = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	double dv = static_cast<double>(v);
	dv /= 1000000000.0;
	return dv;
}

#pragma endregion

// * EVENT SYSTEM
struct SEvent
{
	std::set<std::string> topics;
	void* payload = nullptr;
};

struct SEventConsumer;

typedef void(*SEventConsumerHandler)(const SEventConsumer*, const SEvent*);

struct SEventConsumer
{
	void* custom;
	std::set<std::string> topics;
	SEventConsumerHandler handler;

	bool operator < (const SEventConsumer& o) const
	{
		if(topics != o.topics)
			return topics < o.topics;
		return this < &o;
	}
};

bool operator < (const std::set<std::string>& a, const SEventConsumer& b)
{
	return a < b.topics;
}

bool operator < (const SEventConsumer& a, const std::set<std::string>& b)
{
	return a.topics < b;
}

struct SEventConsumerCache
{
	std::map<std::string, SEventConsumerCache> keyword;
	std::vector<int> leaf;
};

struct SEventState
{
	std::vector<SEventConsumer> consumers;

	// tag caches
	SEventConsumerCache cache;
};

int eventConsumerCreate(SEventState& state, const std::set<std::string>& topics, SEventConsumerHandler handler, void* custom=nullptr)
{
	SEventConsumer consumer;
	consumer.topics = topics;
	consumer.handler = handler;
	consumer.custom = custom;
	state.consumers.push_back(consumer);
	return -1;
}

void eventStateCompile(SEventState& state)
{
	int i = 0;
	for (auto& consumer : state.consumers)
	{
		SEventConsumerCache* current = &state.cache;
		for (auto& topic : consumer.topics)
		{
			current = &current->keyword[topic];
		}
		current->leaf.push_back(i++);
	}
}

// Slow O(n^2) version.
void eventEmit(SEventState& state, const SEvent& ev)
{
	for (auto it=state.consumers.begin(); it!=state.consumers.end(); it++)
	{
 		for (auto& topic : it->topics)
 		{
 			if (ev.topics.find(topic) == ev.topics.end())
 				goto skip_emit;
 		}
		
		it->handler(&(*it), &ev);
	skip_emit:
		0;
	}
}

// Fast but incorrect version. Doesn't work when tags don't match fully.
void eventEmitBtree(SEventState& state, const SEvent& ev)
{
	auto itBegin = std::lower_bound(state.consumers.begin(), state.consumers.end(), ev.topics);
	auto itEnd = std::upper_bound(state.consumers.begin(), state.consumers.end(), ev.topics);
	for (auto it = itBegin; it != itEnd; it++)
	{
		it->handler(&(*it), &ev);
	}
}

void eventEmitRecurtree(SEventState& state, const SEvent& ev)
{
	std::vector<int> consider;
	
	consider.insert(consider.end(), state.cache.leaf.begin(), state.cache.leaf.end());

	for (auto itKeywordBase = ev.topics.begin(); itKeywordBase != ev.topics.end(); itKeywordBase++)
	{
		auto itKeyword = itKeywordBase;
		SEventConsumerCache* current = &state.cache;
		
		while (itKeyword != ev.topics.end())
		{
			auto it = current->keyword.find(*(itKeyword++));
			if (it == current->keyword.end())
				continue; // chain ends here

			current = &(it->second);
			consider.insert(consider.end(), current->leaf.begin(), current->leaf.end());
		}
	}
	std::sort(consider.begin(), consider.end());
	int lastidx = -1;
	for (auto idx : consider)
	{
		if (idx == lastidx)
			continue;
		lastidx = idx;
		state.consumers[idx].handler(&state.consumers[idx], &ev);
	}
}


template< class T>
int testValidityPermutation(std::vector<std::set<std::string>> consumer, std::set<std::string> event, const T& emit)
{
	SEventState state;
	int numHits = 0;
	for (auto& it : consumer)
	{
		eventConsumerCreate(state, it, [](const SEventConsumer* consumer, const SEvent* ev) {
			int* v = ((int*)consumer->custom);
			(*v)++;
		}, &numHits);
	}
	eventStateCompile(state);
	SEvent e;
	e.topics = event;
	emit(state, e);
	return numHits;
}

template <class T>
bool testValidity(const T& eventFunc)
{
#define test(x) if((x) == false) { printf("Test failed: %s\n", #x); ret = false; } else { printf("Test succeeded: %s\n", #x); }
	printf("-------------------------\n");
	bool ret = true;
	test(testValidityPermutation({ {  } }, { "hello" }, eventFunc) == 1);
	test(testValidityPermutation({ { "hello" } }, { "hello" }, eventFunc) == 1);
	test(testValidityPermutation({ { "hello", "test" } }, { "hello" }, eventFunc) == 0);
	test(testValidityPermutation({ { "hello" } }, { "hello", "test" }, eventFunc) == 1);
	test(testValidityPermutation({ { "hello", "test" } }, { "hello", "test" }, eventFunc) == 1);
	test(testValidityPermutation({ { "hello", "baby", "bomb" } }, { "hello", "baby", "got", "an", "atom", "bomb" }, eventFunc) == 1);
	test(testValidityPermutation({ { "hello", "bomb", "baby" }, {"atom", "baby"} }, { "hello", "baby", "got", "an", "atom", "bomb" }, eventFunc) == 2);
	
	return ret;
#undef test
}

template <class T>
void testProfile(const T& eventFunc)
{
	printf("-------------------------\n");
	double timeElapsed;
	double timeConsumerCreateStart = timeGetCurrentSeconds();
	int numConsumerIds = 10000;
	int numConsumers = 10000;
	int numEvents = 100000;
	std::atomic<int> numHits = 0;
	SEventState state;

	// Setup consumer states.
	for (int i = 0; i < numConsumers; i++)
	{
		std::set<std::string> set;
		set.insert("type=log");

		int group = i % numConsumerIds;
		set.insert("id=" + std::to_string(group));
		eventConsumerCreate(state, set, [](const SEventConsumer* consumer, const SEvent* ev) {
			std::atomic<int>* v = ((std::atomic<int>*)consumer->custom);
			++(*v);
		}, &numHits);
	}
	printf("Profiler: Created consumers in %f seconds.\n", timeGetCurrentSeconds() - timeConsumerCreateStart);

	// Compile & optimize tree
	double timeConsumerOptimize = timeGetCurrentSeconds();
	eventStateCompile(state);
	printf("Profiler: Created optimized structure for consumers in %f seconds.\n", timeGetCurrentSeconds() - timeConsumerOptimize);

	// Start emitting events.
	double timeConsumerEventEmit = timeGetCurrentSeconds();

	SEvent e;
	e.topics.insert("type=log");
	e.topics.insert("id=100");
	e.topics.insert("cat");
	//#pragma omp parallel for
	for (int i = 0; i < numEvents; i++)
	{

		eventFunc(state, e);
	}

	timeElapsed = timeGetCurrentSeconds() - timeConsumerEventEmit;
	printf("Profiler: Emitted (%d) events in %f seconds (events/sec: %f).\n", numEvents, timeElapsed, ((double)numEvents) / timeElapsed);
	printf("Num hits: %d\n", numHits.fetch_add(0));
}

int main()
{
	
	testValidityPermutation({ { "hello", "bomb", "baby" },{ "atom", "baby" } }, { "hello", "baby", "got", "an", "atom", "bomb" }, [](SEventState& state, SEvent& ev) { eventEmitRecurtree(state, ev); });

	testValidity([](SEventState& state, SEvent& ev) { eventEmit(state, ev); });
	testValidity([](SEventState& state, SEvent& ev) { eventEmitRecurtree(state, ev); });
	testProfile([](SEventState& state, SEvent& ev) { eventEmit(state, ev); });
	testProfile([](SEventState& state, SEvent& ev) { eventEmitRecurtree(state, ev); });
	
	
	//testProfile();
    return 0;
}

