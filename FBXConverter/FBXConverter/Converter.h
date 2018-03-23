#pragma once

/*
실수는 float, 정수는 int, long
64비트 정수는 __int64
*/

#include "stdafx.h"
#include <cstdlib>
#include <cstring>

#pragma warning(disable:4996)
#define EPSILON 1.0e-10f
#define NUMOFBONE 4

struct float3
{
	float x, y, z;
};
struct float4
{
	float x, y, z, w;
};

using Matrix = float[16];
struct Name
{
	char string[_MAX_PATH];
};

struct Key
{
	float timePos;
	float value;
};
struct Keyframe
{
	float timePos;
	float3 value;
};
struct PoseNode
{
	__int64 poseIdx;
	Matrix	mat;
};
struct Curve
{
	__int64				curveIdx;
	std::vector<Key>	key;
};
struct CurveNode
{
	__int64				nodeIdx;
	std::vector<Key>	translation;
	std::vector<Key>	rotation;
	std::vector<Key>	scale;
};
struct WeightInfo
{
	int		idx;
	float	weight;
};
struct Deformer
{
	__int64					deformerIdx;
	int						boneIdx;
	std::vector<WeightInfo> weight;
};

struct Vertex
{
	float3	pos;
	WeightInfo weights[NUMOFBONE];

	void sortWeights()
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4 - (i + 1); j++) {
				if (weights[j].weight < weights[j + 1].weight)
				{
					WeightInfo temp = weights[j + 1];
					weights[j + 1] = weights[j];
					weights[j] = temp;
				}
			}
		}
	}
	void makeFlatWeights()
	{
		for (int i = 0; i < 4; ++i)
		{
			if (weights[i].weight < EPSILON) weights[i].weight = 0;
		}
	}
	void arrangeWeights()
	{
		makeFlatWeights();
		sortWeights();
	}
	void inputWeight(int boneIdx, float weight)
	{
		weights[NUMOFBONE - 1].weight = weight;
		weights[NUMOFBONE - 1].idx = boneIdx;
		arrangeWeights();
		return;
	}
};
struct UV
{
	float u, v;
};
struct Mesh
{
	std::vector<int> vertexIdx;
	std::vector<int> uvIdx;
	std::vector<float3> normal;
	std::vector<float3> biNormal;
	std::vector<float3> tangent;
};

struct IAVertex
{
	int vertexIdx;
	int uvIdx;
	float3 normal;
	float3 biNormal;
	float3 tangent;
};

struct Bone
{
	__int64 boneIdx;
	char boneName[32];
	Matrix toParent;
	Matrix offset;
	
	std::vector<Keyframe> translation;
	std::vector<Keyframe> rotation;
	std::vector<Keyframe> scale;
};
struct Material
{
	__int64 materialIdx;

	float	EmissiveFactor;
	float3	AmbientColor;
	float3	DiffuseColor;
	float3	TransparentColor;
	float3	SpecularColor;
	float	SpecularFactor;
	float	ShininessExponent;
	float3	Emissive;
	float3	Ambient;
	float3	Diffuse;
	float3	Specular;
	double	Shininess;
	double	Opacity;
	double	Reflectivity;
};
struct Model
{
	Mesh		mesh;
	char		texture_diff[32];
	char		texture_norm[32];
	Material	material;
};

struct AnimationModel : public Model
{
	std::vector<Bone> bone;
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

inline __int64	get64int(char ** ppCh)
{
	return _strtoui64(*ppCh, NULL, 10);
}
inline int		getInt(char ** ppCh)
{
	return atoi(*ppCh);
}
inline float	getFloat(char ** ppCh)
{
	return strtof(*ppCh, NULL);
}
inline double	getDouble(char ** ppCh)
{
	return strtod(*ppCh, NULL);
}
inline float3	getFloat3(char ** ppCh)
{
	float3 tempFloat3;
	tempFloat3.x = getFloat(ppCh);
	pass(ppCh, " ,\n\t");
	tempFloat3.y = getFloat(ppCh);
	pass(ppCh, " ,\n\t");
	tempFloat3.z = getFloat(ppCh);
	pass(ppCh, " ,\n\t");
	return tempFloat3;
}


class Converter
{
	FILE *					m_pFile;
	char *					m_buffer;
	char					m_fileName[256];
	Mesh					m_mesh;

	int						m_nAnimationStack;

	bool					m_bTexture;
	bool					m_bAnimation;
	bool					m_bMaterial;

	std::vector<Curve>		m_vCurve;
	std::vector<CurveNode>	m_vNode;
	std::vector<Deformer>	m_vDeformer;
	std::vector<float3>		m_vPos;
	std::vector<int>		m_vVertexIdx;
	std::vector<UV>			m_vUV;
	std::vector<int>		m_vUVIdx;
	std::vector<float3>		m_vNormal;
	std::vector<float3>		m_vBiNormal;
	std::vector<float3>		m_vTangent;

	std::vector<Vertex>		m_vVertex;

	std::vector<PoseNode>	m_vPoseNode;
	std::vector<Bone>		m_vBone;

	std::vector<Name>		m_vTextureName;
	std::vector<Material>	m_vMaterial;
public:
	Converter();
	~Converter();

	void ReadFile(const char * fileName);
	void WriteFile();

	void WriteMeshOnly();
	void WriteWithAnimation();

	void FindTranslation();
	void FindRotation();
	void FindScale();

	void FindAnimationCount();

	//===================For Mesh==================
	void FindMeshOnly();
	void FindMaterials();
	void FindTextures();
	void FindPosition();
	void FindVertexIdx();
	void FindUV();
	void FindUVIdx();
	void FindNormal();
	void FindBiNormal();
	void FindTangent();
	//===================For Mesh==================


	void FindCurve();
	void FindCurveNode();
	void FindDeformer();
	void FindOffsetMatrix();
	void FindConnectionPoseNodeToBone();
	void Find();

	void ConnectVertex();
	void ConnectUV();
	void ConnectMesh();
	void ConnectAnimation();
	void ConnectBone();
	void Connect();

};

