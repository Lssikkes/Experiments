#!/bin/bash
echo \* Pruning images.
docker image prune -f
echo \* Pruning containers.
docker container prune -f
echo \* Pruning logs.
for dir in /var/lib/docker/containers/*/
do
        dir=${dir%*/}
        container=`basename "$dir"`
        if [ -f "$dir/$container-json.log" ]; then
                echo \* Pruning container log \'$container\'
                mv $dir/$container-json.log $dir/$container-json.log.2 -f
                tail -n 100000 $dir/$container-json.log.2 > $dir/$container-json.log
        fi
done
