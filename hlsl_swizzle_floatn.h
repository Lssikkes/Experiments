// EXPERIMENT - HOW TO HLSL/GLSL SWIZZLE WITH C++
// DISCLAIMER: IT ISN'T PRETTY..
// -----

#pragma once

template <int N, class T, int i0, int i1 = 0, int i2 = 0, int i3 = 0>
class float1_swizzle
{
public:
	operator float() const { float* src = (float*)&M; return i0<N ? src[i0] : 0.0f; }
	float operator =(float x) { if (i0 < N) ((float*)&M)[i0] = x; return ((float*)&M)[i0]; }
	float operator +=(float x) { if (i0 < N) ((float*)&M)[i0] += x; return ((float*)&M)[i0]; }
	float operator -=(float x) { if (i0 < N) ((float*)&M)[i0] -= x; return ((float*)&M)[i0]; }
	float operator /=(float x) { if (i0 < N) ((float*)&M)[i0] *= x; return ((float*)&M)[i0]; }
	float operator *=(float x) { if (i0 < N) ((float*)&M)[i0] /= x; return ((float*)&M)[i0]; }
	char M;
};

template <int N, class T, int i0, int i1 = 0, int i2 = 0, int i3 = 0>
class float_swizzle
{
public:
	operator T() const { float* src = (float*)&M; return T(i0<N ? src[i0] : 0.0f, i1<N ? src[i1] : 0.0f, i2<N ? src[i2] : 0.0f, i3<N ? src[i3] : 0.0f); }
	T _get() const { float* src = (float*)&M; return T(i0 < N ? src[i0] : 0.0f, i1 < N ? src[i1] : 0.0f, i2 < N ? src[i2] : 0.0f, i3 < N ? src[i3] : 0.0f); }

	T operator +(const T& x) { return _get() + x; }
	T operator -(const T& x) { return _get() - x; }
	T operator /(const T& x) { return _get() / x; }
	T operator *(const T& x) { return _get() * x; }
	char M;
};

template <int N>
class floatn
{
public:
	typedef floatn<N> T;
	floatn(float X=0.0f) { if (N > 0) _xyzw[0] = X; if (N > 1) _xyzw[1] = X; if (N > 2) _xyzw[2] = X; if (N > 3) _xyzw[3] = X; }
	floatn(float X, float Y) { if (N > 0) _xyzw[0] = X; if (N > 1) _xyzw[1] = Y; if (N > 2) _xyzw[2] = Y; if (N > 3) _xyzw[3] = Y; }
	floatn(float X, float Y, float Z) { if (N > 0) _xyzw[0] = X; if (N > 1) _xyzw[1] = Y; if (N > 2) _xyzw[2] = Z; if (N > 3) _xyzw[3] = Z; }
	floatn(float X, float Y, float Z, float W) { if (N > 0) _xyzw[0] = X; if (N > 1) _xyzw[1] = Y; if (N > 2) _xyzw[2] = Z; if (N > 3) _xyzw[3] = W; }
	T operator +(const T& x) const { T ret; for (int i = 0;i < N; i++) { ret._xyzw[i] = _xyzw[i] + x._xyzw[i]; } return ret; }
	T operator -(const T& x) const { T ret; for (int i = 0;i < N; i++) { ret._xyzw[i] = _xyzw[i] - x._xyzw[i]; } return ret; }
	T operator /(const T& x) const { T ret; for (int i = 0;i < N; i++) { ret._xyzw[i] = _xyzw[i] * x._xyzw[i]; } return ret; }
	T operator *(const T& x) const { T ret; for (int i = 0;i < N; i++) { ret._xyzw[i] = _xyzw[i] / x._xyzw[i]; } return ret; }
	

	union
	{
		struct { float1_swizzle<N, float, 0> x; };
		struct { float1_swizzle<N, float, 1> y; };
		struct { float1_swizzle<N, float, 2> z; };
		struct { float1_swizzle<N, float, 3> w; };

		struct { float_swizzle<N, floatn<2>, 0, 0> xx; };
		struct { float_swizzle<N, floatn<2>, 0, 1> xy; };
		struct { float_swizzle<N, floatn<2>, 0, 2> xz; };
		struct { float_swizzle<N, floatn<2>, 0, 3> xw; };
		struct { float_swizzle<N, floatn<2>, 1, 0> yx; };
		struct { float_swizzle<N, floatn<2>, 1, 1> yy; };
		struct { float_swizzle<N, floatn<2>, 1, 2> yz; };
		struct { float_swizzle<N, floatn<2>, 1, 3> yw; };
		struct { float_swizzle<N, floatn<2>, 2, 0> zx; };
		struct { float_swizzle<N, floatn<2>, 2, 1> zy; };
		struct { float_swizzle<N, floatn<2>, 2, 2> zz; };
		struct { float_swizzle<N, floatn<2>, 2, 3> zw; };
		struct { float_swizzle<N, floatn<2>, 3, 0> wx; };
		struct { float_swizzle<N, floatn<2>, 3, 1> wy; };
		struct { float_swizzle<N, floatn<2>, 3, 2> wz; };
		struct { float_swizzle<N, floatn<2>, 3, 3> ww; };

		struct { float_swizzle<N, floatn<3>, 0, 0, 0> xxx; };
		struct { float_swizzle<N, floatn<3>, 0, 0, 1> xxy; };
		struct { float_swizzle<N, floatn<3>, 0, 0, 2> xxz; };
		struct { float_swizzle<N, floatn<3>, 0, 0, 3> xxw; };
		struct { float_swizzle<N, floatn<3>, 0, 1, 0> xyx; };
		struct { float_swizzle<N, floatn<3>, 0, 1, 1> xyy; };
		struct { float_swizzle<N, floatn<3>, 0, 1, 2> xyz; };
		struct { float_swizzle<N, floatn<3>, 0, 1, 3> xyw; };
		struct { float_swizzle<N, floatn<3>, 0, 2, 0> xzx; };
		struct { float_swizzle<N, floatn<3>, 0, 2, 1> xzy; };
		struct { float_swizzle<N, floatn<3>, 0, 2, 2> xzz; };
		struct { float_swizzle<N, floatn<3>, 0, 2, 3> xzw; };
		struct { float_swizzle<N, floatn<3>, 0, 3, 0> xwx; };
		struct { float_swizzle<N, floatn<3>, 0, 3, 1> xwy; };
		struct { float_swizzle<N, floatn<3>, 0, 3, 2> xwz; };
		struct { float_swizzle<N, floatn<3>, 0, 3, 3> xww; };

		struct { float_swizzle<N, floatn<3>, 1, 0, 0> yxx; };
		struct { float_swizzle<N, floatn<3>, 1, 0, 1> yxy; };
		struct { float_swizzle<N, floatn<3>, 1, 0, 2> yxz; };
		struct { float_swizzle<N, floatn<3>, 1, 0, 3> yxw; };
		struct { float_swizzle<N, floatn<3>, 1, 1, 0> yyx; };
		struct { float_swizzle<N, floatn<3>, 1, 1, 1> yyy; };
		struct { float_swizzle<N, floatn<3>, 1, 1, 2> yyz; };
		struct { float_swizzle<N, floatn<3>, 1, 1, 3> yyw; };
		struct { float_swizzle<N, floatn<3>, 1, 2, 0> yzx; };
		struct { float_swizzle<N, floatn<3>, 1, 2, 1> yzy; };
		struct { float_swizzle<N, floatn<3>, 1, 2, 2> yzz; };
		struct { float_swizzle<N, floatn<3>, 1, 2, 3> yzw; };
		struct { float_swizzle<N, floatn<3>, 1, 3, 0> ywx; };
		struct { float_swizzle<N, floatn<3>, 1, 3, 1> ywy; };
		struct { float_swizzle<N, floatn<3>, 1, 3, 2> ywz; };
		struct { float_swizzle<N, floatn<3>, 1, 3, 3> yww; };

		struct { float_swizzle<N, floatn<3>, 2, 0, 0> zxx; };
		struct { float_swizzle<N, floatn<3>, 2, 0, 1> zxy; };
		struct { float_swizzle<N, floatn<3>, 2, 0, 2> zxz; };
		struct { float_swizzle<N, floatn<3>, 2, 0, 3> zxw; };
		struct { float_swizzle<N, floatn<3>, 2, 1, 0> zyx; };
		struct { float_swizzle<N, floatn<3>, 2, 1, 1> zyy; };
		struct { float_swizzle<N, floatn<3>, 2, 1, 2> zyz; };
		struct { float_swizzle<N, floatn<3>, 2, 1, 3> zyw; };
		struct { float_swizzle<N, floatn<3>, 2, 2, 0> zzx; };
		struct { float_swizzle<N, floatn<3>, 2, 2, 1> zzy; };
		struct { float_swizzle<N, floatn<3>, 2, 2, 2> zzz; };
		struct { float_swizzle<N, floatn<3>, 2, 2, 3> zzw; };
		struct { float_swizzle<N, floatn<3>, 2, 3, 0> zwx; };
		struct { float_swizzle<N, floatn<3>, 2, 3, 1> zwy; };
		struct { float_swizzle<N, floatn<3>, 2, 3, 2> zwz; };
		struct { float_swizzle<N, floatn<3>, 2, 3, 3> zww; };

		struct { float_swizzle<N, floatn<3>, 3, 0, 0> wxx; };
		struct { float_swizzle<N, floatn<3>, 3, 0, 1> wxy; };
		struct { float_swizzle<N, floatn<3>, 3, 0, 2> wxz; };
		struct { float_swizzle<N, floatn<3>, 3, 0, 3> wxw; };
		struct { float_swizzle<N, floatn<3>, 3, 1, 0> wyx; };
		struct { float_swizzle<N, floatn<3>, 3, 1, 1> wyy; };
		struct { float_swizzle<N, floatn<3>, 3, 1, 2> wyz; };
		struct { float_swizzle<N, floatn<3>, 3, 1, 3> wyw; };
		struct { float_swizzle<N, floatn<3>, 3, 2, 0> wzx; };
		struct { float_swizzle<N, floatn<3>, 3, 2, 1> wzy; };
		struct { float_swizzle<N, floatn<3>, 3, 2, 2> wzz; };
		struct { float_swizzle<N, floatn<3>, 3, 2, 3> wzw; };
		struct { float_swizzle<N, floatn<3>, 3, 3, 0> wwx; };
		struct { float_swizzle<N, floatn<3>, 3, 3, 1> wwy; };
		struct { float_swizzle<N, floatn<3>, 3, 3, 2> wwz; };
		struct { float_swizzle<N, floatn<3>, 3, 3, 3> www; };

		struct { float_swizzle<N, floatn<4>, 0, 0, 0, 0> xxxx; };
		struct { float_swizzle<N, floatn<4>, 0, 0, 0, 1> xxxy; };
		struct { float_swizzle<N, floatn<4>, 0, 0, 0, 2> xxxz; };
		struct { float_swizzle<N, floatn<4>, 0, 0, 0, 3> xxxw; };
		struct { float_swizzle<N, floatn<4>, 0, 0, 1, 0> xxyx; };
		struct { float_swizzle<N, floatn<4>, 0, 0, 1, 1> xxyy; };
		struct { float_swizzle<N, floatn<4>, 0, 0, 1, 2> xxyz; };
		struct { float_swizzle<N, floatn<4>, 0, 0, 1, 3> xxyw; };
		struct { float_swizzle<N, floatn<4>, 0, 0, 2, 0> xxzx; };
		struct { float_swizzle<N, floatn<4>, 0, 0, 2, 1> xxzy; };
		struct { float_swizzle<N, floatn<4>, 0, 0, 2, 2> xxzz; };
		struct { float_swizzle<N, floatn<4>, 0, 0, 2, 3> xxzw; };
		struct { float_swizzle<N, floatn<4>, 0, 0, 3, 0> xxwx; };
		struct { float_swizzle<N, floatn<4>, 0, 0, 3, 1> xxwy; };
		struct { float_swizzle<N, floatn<4>, 0, 0, 3, 2> xxwz; };
		struct { float_swizzle<N, floatn<4>, 0, 0, 3, 3> xxww; };
		struct { float_swizzle<N, floatn<4>, 0, 1, 0, 0> xyxx; };
		struct { float_swizzle<N, floatn<4>, 0, 1, 0, 1> xyxy; };
		struct { float_swizzle<N, floatn<4>, 0, 1, 0, 2> xyxz; };
		struct { float_swizzle<N, floatn<4>, 0, 1, 0, 3> xyxw; };
		struct { float_swizzle<N, floatn<4>, 0, 1, 1, 0> xyyx; };
		struct { float_swizzle<N, floatn<4>, 0, 1, 1, 1> xyyy; };
		struct { float_swizzle<N, floatn<4>, 0, 1, 1, 2> xyyz; };
		struct { float_swizzle<N, floatn<4>, 0, 1, 1, 3> xyyw; };
		struct { float_swizzle<N, floatn<4>, 0, 1, 2, 0> xyzx; };
		struct { float_swizzle<N, floatn<4>, 0, 1, 2, 1> xyzy; };
		struct { float_swizzle<N, floatn<4>, 0, 1, 2, 2> xyzz; };
		struct { float_swizzle<N, floatn<4>, 0, 1, 2, 3> xyzw; };
		struct { float_swizzle<N, floatn<4>, 0, 1, 3, 0> xywx; };
		struct { float_swizzle<N, floatn<4>, 0, 1, 3, 1> xywy; };
		struct { float_swizzle<N, floatn<4>, 0, 1, 3, 2> xywz; };
		struct { float_swizzle<N, floatn<4>, 0, 1, 3, 3> xyww; };
		struct { float_swizzle<N, floatn<4>, 0, 2, 0, 0> xzxx; };
		struct { float_swizzle<N, floatn<4>, 0, 2, 0, 1> xzxy; };
		struct { float_swizzle<N, floatn<4>, 0, 2, 0, 2> xzxz; };
		struct { float_swizzle<N, floatn<4>, 0, 2, 0, 3> xzxw; };
		struct { float_swizzle<N, floatn<4>, 0, 2, 1, 0> xzyx; };
		struct { float_swizzle<N, floatn<4>, 0, 2, 1, 1> xzyy; };
		struct { float_swizzle<N, floatn<4>, 0, 2, 1, 2> xzyz; };
		struct { float_swizzle<N, floatn<4>, 0, 2, 1, 3> xzyw; };
		struct { float_swizzle<N, floatn<4>, 0, 2, 2, 0> xzzx; };
		struct { float_swizzle<N, floatn<4>, 0, 2, 2, 1> xzzy; };
		struct { float_swizzle<N, floatn<4>, 0, 2, 2, 2> xzzz; };
		struct { float_swizzle<N, floatn<4>, 0, 2, 2, 3> xzzw; };
		struct { float_swizzle<N, floatn<4>, 0, 2, 3, 0> xzwx; };
		struct { float_swizzle<N, floatn<4>, 0, 2, 3, 1> xzwy; };
		struct { float_swizzle<N, floatn<4>, 0, 2, 3, 2> xzwz; };
		struct { float_swizzle<N, floatn<4>, 0, 2, 3, 3> xzww; };
		struct { float_swizzle<N, floatn<4>, 0, 3, 0, 0> xwxx; };
		struct { float_swizzle<N, floatn<4>, 0, 3, 0, 1> xwxy; };
		struct { float_swizzle<N, floatn<4>, 0, 3, 0, 2> xwxz; };
		struct { float_swizzle<N, floatn<4>, 0, 3, 0, 3> xwxw; };
		struct { float_swizzle<N, floatn<4>, 0, 3, 1, 0> xwyx; };
		struct { float_swizzle<N, floatn<4>, 0, 3, 1, 1> xwyy; };
		struct { float_swizzle<N, floatn<4>, 0, 3, 1, 2> xwyz; };
		struct { float_swizzle<N, floatn<4>, 0, 3, 1, 3> xwyw; };
		struct { float_swizzle<N, floatn<4>, 0, 3, 2, 0> xwzx; };
		struct { float_swizzle<N, floatn<4>, 0, 3, 2, 1> xwzy; };
		struct { float_swizzle<N, floatn<4>, 0, 3, 2, 2> xwzz; };
		struct { float_swizzle<N, floatn<4>, 0, 3, 2, 3> xwzw; };
		struct { float_swizzle<N, floatn<4>, 0, 3, 3, 0> xwwx; };
		struct { float_swizzle<N, floatn<4>, 0, 3, 3, 1> xwwy; };
		struct { float_swizzle<N, floatn<4>, 0, 3, 3, 2> xwwz; };
		struct { float_swizzle<N, floatn<4>, 0, 3, 3, 3> xwww; };

		struct { float_swizzle<N, floatn<4>, 1, 0, 0, 0> yxxx; };
		struct { float_swizzle<N, floatn<4>, 1, 0, 0, 1> yxxy; };
		struct { float_swizzle<N, floatn<4>, 1, 0, 0, 2> yxxz; };
		struct { float_swizzle<N, floatn<4>, 1, 0, 0, 3> yxxw; };
		struct { float_swizzle<N, floatn<4>, 1, 0, 1, 0> yxyx; };
		struct { float_swizzle<N, floatn<4>, 1, 0, 1, 1> yxyy; };
		struct { float_swizzle<N, floatn<4>, 1, 0, 1, 2> yxyz; };
		struct { float_swizzle<N, floatn<4>, 1, 0, 1, 3> yxyw; };
		struct { float_swizzle<N, floatn<4>, 1, 0, 2, 0> yxzx; };
		struct { float_swizzle<N, floatn<4>, 1, 0, 2, 1> yxzy; };
		struct { float_swizzle<N, floatn<4>, 1, 0, 2, 2> yxzz; };
		struct { float_swizzle<N, floatn<4>, 1, 0, 2, 3> yxzw; };
		struct { float_swizzle<N, floatn<4>, 1, 0, 3, 0> yxwx; };
		struct { float_swizzle<N, floatn<4>, 1, 0, 3, 1> yxwy; };
		struct { float_swizzle<N, floatn<4>, 1, 0, 3, 2> yxwz; };
		struct { float_swizzle<N, floatn<4>, 1, 0, 3, 3> yxww; };
		struct { float_swizzle<N, floatn<4>, 1, 1, 0, 0> yyxx; };
		struct { float_swizzle<N, floatn<4>, 1, 1, 0, 1> yyxy; };
		struct { float_swizzle<N, floatn<4>, 1, 1, 0, 2> yyxz; };
		struct { float_swizzle<N, floatn<4>, 1, 1, 0, 3> yyxw; };
		struct { float_swizzle<N, floatn<4>, 1, 1, 1, 0> yyyx; };
		struct { float_swizzle<N, floatn<4>, 1, 1, 1, 1> yyyy; };
		struct { float_swizzle<N, floatn<4>, 1, 1, 1, 2> yyyz; };
		struct { float_swizzle<N, floatn<4>, 1, 1, 1, 3> yyyw; };
		struct { float_swizzle<N, floatn<4>, 1, 1, 2, 0> yyzx; };
		struct { float_swizzle<N, floatn<4>, 1, 1, 2, 1> yyzy; };
		struct { float_swizzle<N, floatn<4>, 1, 1, 2, 2> yyzz; };
		struct { float_swizzle<N, floatn<4>, 1, 1, 2, 3> yyzw; };
		struct { float_swizzle<N, floatn<4>, 1, 1, 3, 0> yywx; };
		struct { float_swizzle<N, floatn<4>, 1, 1, 3, 1> yywy; };
		struct { float_swizzle<N, floatn<4>, 1, 1, 3, 2> yywz; };
		struct { float_swizzle<N, floatn<4>, 1, 1, 3, 3> yyww; };
		struct { float_swizzle<N, floatn<4>, 1, 2, 0, 0> yzxx; };
		struct { float_swizzle<N, floatn<4>, 1, 2, 0, 1> yzxy; };
		struct { float_swizzle<N, floatn<4>, 1, 2, 0, 2> yzxz; };
		struct { float_swizzle<N, floatn<4>, 1, 2, 0, 3> yzxw; };
		struct { float_swizzle<N, floatn<4>, 1, 2, 1, 0> yzyx; };
		struct { float_swizzle<N, floatn<4>, 1, 2, 1, 1> yzyy; };
		struct { float_swizzle<N, floatn<4>, 1, 2, 1, 2> yzyz; };
		struct { float_swizzle<N, floatn<4>, 1, 2, 1, 3> yzyw; };
		struct { float_swizzle<N, floatn<4>, 1, 2, 2, 0> yzzx; };
		struct { float_swizzle<N, floatn<4>, 1, 2, 2, 1> yzzy; };
		struct { float_swizzle<N, floatn<4>, 1, 2, 2, 2> yzzz; };
		struct { float_swizzle<N, floatn<4>, 1, 2, 2, 3> yzzw; };
		struct { float_swizzle<N, floatn<4>, 1, 2, 3, 0> yzwx; };
		struct { float_swizzle<N, floatn<4>, 1, 2, 3, 1> yzwy; };
		struct { float_swizzle<N, floatn<4>, 1, 2, 3, 2> yzwz; };
		struct { float_swizzle<N, floatn<4>, 1, 2, 3, 3> yzww; };
		struct { float_swizzle<N, floatn<4>, 1, 3, 0, 0> ywxx; };
		struct { float_swizzle<N, floatn<4>, 1, 3, 0, 1> ywxy; };
		struct { float_swizzle<N, floatn<4>, 1, 3, 0, 2> ywxz; };
		struct { float_swizzle<N, floatn<4>, 1, 3, 0, 3> ywxw; };
		struct { float_swizzle<N, floatn<4>, 1, 3, 1, 0> ywyx; };
		struct { float_swizzle<N, floatn<4>, 1, 3, 1, 1> ywyy; };
		struct { float_swizzle<N, floatn<4>, 1, 3, 1, 2> ywyz; };
		struct { float_swizzle<N, floatn<4>, 1, 3, 1, 3> ywyw; };
		struct { float_swizzle<N, floatn<4>, 1, 3, 2, 0> ywzx; };
		struct { float_swizzle<N, floatn<4>, 1, 3, 2, 1> ywzy; };
		struct { float_swizzle<N, floatn<4>, 1, 3, 2, 2> ywzz; };
		struct { float_swizzle<N, floatn<4>, 1, 3, 2, 3> ywzw; };
		struct { float_swizzle<N, floatn<4>, 1, 3, 3, 0> ywwx; };
		struct { float_swizzle<N, floatn<4>, 1, 3, 3, 1> ywwy; };
		struct { float_swizzle<N, floatn<4>, 1, 3, 3, 2> ywwz; };
		struct { float_swizzle<N, floatn<4>, 1, 3, 3, 3> ywww; };

		struct { float_swizzle<N, floatn<4>, 2, 0, 0, 0> zxxx; };
		struct { float_swizzle<N, floatn<4>, 2, 0, 0, 1> zxxy; };
		struct { float_swizzle<N, floatn<4>, 2, 0, 0, 2> zxxz; };
		struct { float_swizzle<N, floatn<4>, 2, 0, 0, 3> zxxw; };
		struct { float_swizzle<N, floatn<4>, 2, 0, 1, 0> zxyx; };
		struct { float_swizzle<N, floatn<4>, 2, 0, 1, 1> zxyy; };
		struct { float_swizzle<N, floatn<4>, 2, 0, 1, 2> zxyz; };
		struct { float_swizzle<N, floatn<4>, 2, 0, 1, 3> zxyw; };
		struct { float_swizzle<N, floatn<4>, 2, 0, 2, 0> zxzx; };
		struct { float_swizzle<N, floatn<4>, 2, 0, 2, 1> zxzy; };
		struct { float_swizzle<N, floatn<4>, 2, 0, 2, 2> zxzz; };
		struct { float_swizzle<N, floatn<4>, 2, 0, 2, 3> zxzw; };
		struct { float_swizzle<N, floatn<4>, 2, 0, 3, 0> zxwx; };
		struct { float_swizzle<N, floatn<4>, 2, 0, 3, 1> zxwy; };
		struct { float_swizzle<N, floatn<4>, 2, 0, 3, 2> zxwz; };
		struct { float_swizzle<N, floatn<4>, 2, 0, 3, 3> zxww; };
		struct { float_swizzle<N, floatn<4>, 2, 1, 0, 0> zyxx; };
		struct { float_swizzle<N, floatn<4>, 2, 1, 0, 1> zyxy; };
		struct { float_swizzle<N, floatn<4>, 2, 1, 0, 2> zyxz; };
		struct { float_swizzle<N, floatn<4>, 2, 1, 0, 3> zyxw; };
		struct { float_swizzle<N, floatn<4>, 2, 1, 1, 0> zyyx; };
		struct { float_swizzle<N, floatn<4>, 2, 1, 1, 1> zyyy; };
		struct { float_swizzle<N, floatn<4>, 2, 1, 1, 2> zyyz; };
		struct { float_swizzle<N, floatn<4>, 2, 1, 1, 3> zyyw; };
		struct { float_swizzle<N, floatn<4>, 2, 1, 2, 0> zyzx; };
		struct { float_swizzle<N, floatn<4>, 2, 1, 2, 1> zyzy; };
		struct { float_swizzle<N, floatn<4>, 2, 1, 2, 2> zyzz; };
		struct { float_swizzle<N, floatn<4>, 2, 1, 2, 3> zyzw; };
		struct { float_swizzle<N, floatn<4>, 2, 1, 3, 0> zywx; };
		struct { float_swizzle<N, floatn<4>, 2, 1, 3, 1> zywy; };
		struct { float_swizzle<N, floatn<4>, 2, 1, 3, 2> zywz; };
		struct { float_swizzle<N, floatn<4>, 2, 1, 3, 3> zyww; };
		struct { float_swizzle<N, floatn<4>, 2, 2, 0, 0> zzxx; };
		struct { float_swizzle<N, floatn<4>, 2, 2, 0, 1> zzxy; };
		struct { float_swizzle<N, floatn<4>, 2, 2, 0, 2> zzxz; };
		struct { float_swizzle<N, floatn<4>, 2, 2, 0, 3> zzxw; };
		struct { float_swizzle<N, floatn<4>, 2, 2, 1, 0> zzyx; };
		struct { float_swizzle<N, floatn<4>, 2, 2, 1, 1> zzyy; };
		struct { float_swizzle<N, floatn<4>, 2, 2, 1, 2> zzyz; };
		struct { float_swizzle<N, floatn<4>, 2, 2, 1, 3> zzyw; };
		struct { float_swizzle<N, floatn<4>, 2, 2, 2, 0> zzzx; };
		struct { float_swizzle<N, floatn<4>, 2, 2, 2, 1> zzzy; };
		struct { float_swizzle<N, floatn<4>, 2, 2, 2, 2> zzzz; };
		struct { float_swizzle<N, floatn<4>, 2, 2, 2, 3> zzzw; };
		struct { float_swizzle<N, floatn<4>, 2, 2, 3, 0> zzwx; };
		struct { float_swizzle<N, floatn<4>, 2, 2, 3, 1> zzwy; };
		struct { float_swizzle<N, floatn<4>, 2, 2, 3, 2> zzwz; };
		struct { float_swizzle<N, floatn<4>, 2, 2, 3, 3> zzww; };
		struct { float_swizzle<N, floatn<4>, 2, 3, 0, 0> zwxx; };
		struct { float_swizzle<N, floatn<4>, 2, 3, 0, 1> zwxy; };
		struct { float_swizzle<N, floatn<4>, 2, 3, 0, 2> zwxz; };
		struct { float_swizzle<N, floatn<4>, 2, 3, 0, 3> zwxw; };
		struct { float_swizzle<N, floatn<4>, 2, 3, 1, 0> zwyx; };
		struct { float_swizzle<N, floatn<4>, 2, 3, 1, 1> zwyy; };
		struct { float_swizzle<N, floatn<4>, 2, 3, 1, 2> zwyz; };
		struct { float_swizzle<N, floatn<4>, 2, 3, 1, 3> zwyw; };
		struct { float_swizzle<N, floatn<4>, 2, 3, 2, 0> zwzx; };
		struct { float_swizzle<N, floatn<4>, 2, 3, 2, 1> zwzy; };
		struct { float_swizzle<N, floatn<4>, 2, 3, 2, 2> zwzz; };
		struct { float_swizzle<N, floatn<4>, 2, 3, 2, 3> zwzw; };
		struct { float_swizzle<N, floatn<4>, 2, 3, 3, 0> zwwx; };
		struct { float_swizzle<N, floatn<4>, 2, 3, 3, 1> zwwy; };
		struct { float_swizzle<N, floatn<4>, 2, 3, 3, 2> zwwz; };
		struct { float_swizzle<N, floatn<4>, 2, 3, 3, 3> zwww; };

		struct { float_swizzle<N, floatn<4>, 3, 0, 0, 0> wxxx; };
		struct { float_swizzle<N, floatn<4>, 3, 0, 0, 1> wxxy; };
		struct { float_swizzle<N, floatn<4>, 3, 0, 0, 2> wxxz; };
		struct { float_swizzle<N, floatn<4>, 3, 0, 0, 3> wxxw; };
		struct { float_swizzle<N, floatn<4>, 3, 0, 1, 0> wxyx; };
		struct { float_swizzle<N, floatn<4>, 3, 0, 1, 1> wxyy; };
		struct { float_swizzle<N, floatn<4>, 3, 0, 1, 2> wxyz; };
		struct { float_swizzle<N, floatn<4>, 3, 0, 1, 3> wxyw; };
		struct { float_swizzle<N, floatn<4>, 3, 0, 2, 0> wxzx; };
		struct { float_swizzle<N, floatn<4>, 3, 0, 2, 1> wxzy; };
		struct { float_swizzle<N, floatn<4>, 3, 0, 2, 2> wxzz; };
		struct { float_swizzle<N, floatn<4>, 3, 0, 2, 3> wxzw; };
		struct { float_swizzle<N, floatn<4>, 3, 0, 3, 0> wxwx; };
		struct { float_swizzle<N, floatn<4>, 3, 0, 3, 1> wxwy; };
		struct { float_swizzle<N, floatn<4>, 3, 0, 3, 2> wxwz; };
		struct { float_swizzle<N, floatn<4>, 3, 0, 3, 3> wxww; };
		struct { float_swizzle<N, floatn<4>, 3, 1, 0, 0> wyxx; };
		struct { float_swizzle<N, floatn<4>, 3, 1, 0, 1> wyxy; };
		struct { float_swizzle<N, floatn<4>, 3, 1, 0, 2> wyxz; };
		struct { float_swizzle<N, floatn<4>, 3, 1, 0, 3> wyxw; };
		struct { float_swizzle<N, floatn<4>, 3, 1, 1, 0> wyyx; };
		struct { float_swizzle<N, floatn<4>, 3, 1, 1, 1> wyyy; };
		struct { float_swizzle<N, floatn<4>, 3, 1, 1, 2> wyyz; };
		struct { float_swizzle<N, floatn<4>, 3, 1, 1, 3> wyyw; };
		struct { float_swizzle<N, floatn<4>, 3, 1, 2, 0> wyzx; };
		struct { float_swizzle<N, floatn<4>, 3, 1, 2, 1> wyzy; };
		struct { float_swizzle<N, floatn<4>, 3, 1, 2, 2> wyzz; };
		struct { float_swizzle<N, floatn<4>, 3, 1, 2, 3> wyzw; };
		struct { float_swizzle<N, floatn<4>, 3, 1, 3, 0> wywx; };
		struct { float_swizzle<N, floatn<4>, 3, 1, 3, 1> wywy; };
		struct { float_swizzle<N, floatn<4>, 3, 1, 3, 2> wywz; };
		struct { float_swizzle<N, floatn<4>, 3, 1, 3, 3> wyww; };
		struct { float_swizzle<N, floatn<4>, 3, 2, 0, 0> wzxx; };
		struct { float_swizzle<N, floatn<4>, 3, 2, 0, 1> wzxy; };
		struct { float_swizzle<N, floatn<4>, 3, 2, 0, 2> wzxz; };
		struct { float_swizzle<N, floatn<4>, 3, 2, 0, 3> wzxw; };
		struct { float_swizzle<N, floatn<4>, 3, 2, 1, 0> wzyx; };
		struct { float_swizzle<N, floatn<4>, 3, 2, 1, 1> wzyy; };
		struct { float_swizzle<N, floatn<4>, 3, 2, 1, 2> wzyz; };
		struct { float_swizzle<N, floatn<4>, 3, 2, 1, 3> wzyw; };
		struct { float_swizzle<N, floatn<4>, 3, 2, 2, 0> wzzx; };
		struct { float_swizzle<N, floatn<4>, 3, 2, 2, 1> wzzy; };
		struct { float_swizzle<N, floatn<4>, 3, 2, 2, 2> wzzz; };
		struct { float_swizzle<N, floatn<4>, 3, 2, 2, 3> wzzw; };
		struct { float_swizzle<N, floatn<4>, 3, 2, 3, 0> wzwx; };
		struct { float_swizzle<N, floatn<4>, 3, 2, 3, 1> wzwy; };
		struct { float_swizzle<N, floatn<4>, 3, 2, 3, 2> wzwz; };
		struct { float_swizzle<N, floatn<4>, 3, 2, 3, 3> wzww; };
		struct { float_swizzle<N, floatn<4>, 3, 3, 0, 0> wwxx; };
		struct { float_swizzle<N, floatn<4>, 3, 3, 0, 1> wwxy; };
		struct { float_swizzle<N, floatn<4>, 3, 3, 0, 2> wwxz; };
		struct { float_swizzle<N, floatn<4>, 3, 3, 0, 3> wwxw; };
		struct { float_swizzle<N, floatn<4>, 3, 3, 1, 0> wwyx; };
		struct { float_swizzle<N, floatn<4>, 3, 3, 1, 1> wwyy; };
		struct { float_swizzle<N, floatn<4>, 3, 3, 1, 2> wwyz; };
		struct { float_swizzle<N, floatn<4>, 3, 3, 1, 3> wwyw; };
		struct { float_swizzle<N, floatn<4>, 3, 3, 2, 0> wwzx; };
		struct { float_swizzle<N, floatn<4>, 3, 3, 2, 1> wwzy; };
		struct { float_swizzle<N, floatn<4>, 3, 3, 2, 2> wwzz; };
		struct { float_swizzle<N, floatn<4>, 3, 3, 2, 3> wwzw; };
		struct { float_swizzle<N, floatn<4>, 3, 3, 3, 0> wwwx; };
		struct { float_swizzle<N, floatn<4>, 3, 3, 3, 1> wwwy; };
		struct { float_swizzle<N, floatn<4>, 3, 3, 3, 2> wwwz; };
		struct { float_swizzle<N, floatn<4>, 3, 3, 3, 3> wwww; };

		struct { float _xyzw[N]; };
	};
};

typedef floatn<2> float2;
typedef floatn<3> float3;
typedef floatn<4> float4;

#include <iostream>
using namespace std;


int main() {
	const float2 x(10.0f, 20.0f);
	//float3 y = x.xyz;
	float4 z = x.xyyy;
	float3 y = z.xyz + float3(1.0f,2.0f);
	z.y = 6.66f;
	std::cout << x.x << "," << x.y << "\n";
	std::cout << y.x << "," << y.y << "," << y.z << "\n";
	std::cout << z.x << "," << z.y << "," << z.z << "," << z.w << "\n";
	return 0;
}
