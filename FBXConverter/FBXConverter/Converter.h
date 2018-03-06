#pragma once

/*
실수는 float, 정수는 int, long
64비트 정수는 __int64
*/

#include <cstdlib>
#include <cstring>

#define EPSILON 1.0e-10f

struct float3
{
	float x, y, z;
};
struct float4
{
	float x, y, z, w;
};
struct Matrix
{
	float val[16];
};

inline bool isAlmostEqual(float a, float b)
{
	return abs(a - b) < EPSILON;
}
inline bool isSame(const char * a, const char * b)
{
	return !strcmp(a, b);
}
inline bool isIn(const char * a, const char * b)
{
	return (strstr(a, b) != NULL);
}
inline void pass(char ** ppCh, const char * target)
{
	*ppCh = strtok(NULL, target);
}
inline void passTo(char ** ppCh, const char * target)
{
	while (!isSame(*ppCh, target))
	{
		pass(ppCh, " ,\n\t*");
	}
}
inline void passOver(char ** ppCh, const char * target)
{
	while (!isSame(*ppCh, target))
	{
		pass(ppCh, " ,\n\t");
	}
	pass(ppCh, " ,\n\t*");
}

class Converter
{
public:
	Converter();
	~Converter();

	void ReadFile();
	void WriteFile();

	void FindCurve();
	void FindCurveNode();
	void FindDeformer();
	void FindPosition();
	void FindVertexIdx();
	void FindUV();
	void FindUVIdx();
	void FindBiNormal();
	void FindNormal();
	void FindTangent();
	void Find();

	void ConnectVertex();
	void ConnectUV();
	void ConnectMesh();
	void ConnectAnimation();
	void ConnectBone();
	void Connect();

};

