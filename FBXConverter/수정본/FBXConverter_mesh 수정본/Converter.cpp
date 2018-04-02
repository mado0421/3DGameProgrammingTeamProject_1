#include "stdafx.h"
#include "Converter.h"


Converter::Converter()
{
}


Converter::~Converter()
{
}

void Converter::ReadFile(const char * fileName)
{
	char filePath[256] = "resource/model/";
	__int64 lSize;
	size_t result;

	strcpy(m_fileName, fileName);
	strcat(filePath, m_fileName);
	strcat(filePath, ".txt");

	m_pFile = fopen(filePath, "r");
	assert(m_pFile && "파일을 찾지 못했습니다.");

	fseek(m_pFile, 0, SEEK_END);
	lSize = ftell(m_pFile);
	rewind(m_pFile);


	m_buffer = (char*)malloc(sizeof(char)*lSize);
	memset(m_buffer, 0, lSize);
	result = fread(m_buffer, 1, lSize, m_pFile);

	fclose(m_pFile);

	//FindAnimationCount();
	m_buffer = strtok(m_buffer, " ,\n\t");

	//-------------------------------------------------
	// 해야하는 것들을 여기 적어주세요.
	while (m_buffer != NULL)
	{
		FindMeshOnly();
		pass(&m_buffer, " ,\n\t");
	}
	//-------------------------------------------------

	ConnectVertex();
	ConnectMesh();

	WriteMeshOnly();
//	WriteFile();
}

void Converter::WriteMeshOnly()
{
	char filePath[256] = "ouput\\model\\";
	long lSize;
	size_t result;
	int num;

	strcat(filePath, m_fileName);
	strcat(filePath, ".dat");

	m_pFile = fopen(m_fileName, "wb");
	assert(m_pFile && "파일을 찾지 못했습니다.");

	num = m_vMaterial.size();
	fwrite(&num, sizeof(int), 1, m_pFile);
	for (int i = 0; i < num; ++i) 
	{
		fwrite(&m_vMaterial[i], sizeof(Material), 1, m_pFile);
	}

	num = m_vTextureName.size();
	fwrite(&num, sizeof(int), 1, m_pFile);
	for (int i = 0; i < num; ++i)
	{
		fwrite(&m_vTextureName[i], sizeof(Name), 1, m_pFile);
	}

	num = m_vVertex.size();
	fwrite(&num, sizeof(int), 1, m_pFile);
	for (int i = 0; i < num; ++i)
	{
		fwrite(&m_vVertex[i], sizeof(Vertex), 1, m_pFile);
	}

	num = m_vUV.size();
	fwrite(&num, sizeof(int), 1, m_pFile);
	for (int i = 0; i < num; ++i)
	{
		fwrite(&m_vUV[i], sizeof(UV), 1, m_pFile);
	}


	num = m_mesh.vertexIdx.size();
	IAVertex* iaVertex = new IAVertex[num];

	fwrite(&num, sizeof(int), 1, m_pFile);
	for (int i = 0; i < num; ++i)
	{
		iaVertex[i].vertexIdx = m_mesh.vertexIdx[i];
		iaVertex[i].uvIdx = m_mesh.uvIdx[i];
		iaVertex[i].normal = m_mesh.normal[i];
		if (m_mesh.biNormal.size() == 0)
			iaVertex[i].biNormal = float3(0, 0, 0);
		else
			iaVertex[i].biNormal = m_mesh.biNormal[i];

		if (m_mesh.tangent.size() == 0)
			iaVertex[i].tangent = float3(0, 0, 0);
		else
			iaVertex[i].tangent = m_mesh.tangent[i];
	}
	fwrite(iaVertex, sizeof(IAVertex)*num, 1, m_pFile);

	fclose(m_pFile);
}

void Converter::FindAnimationCount()
{
	/*
	지금 보니까 AnimationStack은 애니메이션이 있든 없든 카운트가 1이 있더라
	Object properties 나오기 전까지 AnimationCurve
	passOver(&m_buffer, "\"AnimationStack\"");
	m_nAnimationStack = getInt(&m_buffer);
	*/
}

void Converter::FindMeshOnly()
{
	FindMaterials();
	FindTextures();
	FindPosition();
	FindVertexIdx();
	FindUV();
	FindUVIdx();
	FindNormal();
	FindBiNormal();
	FindTangent();
}

void Converter::FindMaterials()
{
	if (isSame(m_buffer, "Material:"))
	{
		Material tempMaterial;
		pass(&m_buffer, " ,");
		tempMaterial.materialIdx = get64int(&m_buffer);

		passTo(&m_buffer, "\"EmissiveFactor\"");
		passOver(&m_buffer, "\"A\"");
		tempMaterial.EmissiveFactor = getFloat(&m_buffer);

		passTo(&m_buffer, "\"AmbientColor\"");
		passOver(&m_buffer, "\"A\"");
		tempMaterial.AmbientColor = getFloat3(&m_buffer);

		passTo(&m_buffer, "\"DiffuseColor\"");
		passOver(&m_buffer, "\"A\"");
		tempMaterial.DiffuseColor = getFloat3(&m_buffer);

		passTo(&m_buffer, "\"TransparentColor\"");
		passOver(&m_buffer, "\"A\"");
		tempMaterial.TransparentColor = getFloat3(&m_buffer);

		passTo(&m_buffer, "\"SpecularColor\"");
		passOver(&m_buffer, "\"A\"");
		tempMaterial.SpecularColor = getFloat3(&m_buffer);

		passTo(&m_buffer, "\"SpecularFactor\"");
		passOver(&m_buffer, "\"A\"");
		tempMaterial.SpecularFactor = getFloat(&m_buffer);

		passTo(&m_buffer, "\"ShininessExponent\"");
		passOver(&m_buffer, "\"A\"");
		tempMaterial.ShininessExponent = getFloat(&m_buffer);

		passTo(&m_buffer, "\"Emissive\"");
		passOver(&m_buffer, "\"\"");
		tempMaterial.Emissive = getFloat3(&m_buffer);

		passTo(&m_buffer, "\"Ambient\"");
		passOver(&m_buffer, "\"\"");
		tempMaterial.Ambient = getFloat3(&m_buffer);

		passTo(&m_buffer, "\"Diffuse\"");
		passOver(&m_buffer, "\"\"");
		tempMaterial.Diffuse = getFloat3(&m_buffer);

		passTo(&m_buffer, "\"Specular\"");
		passOver(&m_buffer, "\"\"");
		tempMaterial.Specular = getFloat3(&m_buffer);

		passTo(&m_buffer, "\"Shininess\"");
		passOver(&m_buffer, "\"\"");
		tempMaterial.Shininess = getDouble(&m_buffer);

		passTo(&m_buffer, "\"Opacity\"");
		passOver(&m_buffer, "\"\"");
		tempMaterial.Opacity = getDouble(&m_buffer);

		passTo(&m_buffer, "\"Reflectivity\"");
		passOver(&m_buffer, "\"\"");
		tempMaterial.Reflectivity = getDouble(&m_buffer);

		m_vMaterial.push_back(tempMaterial);
	}
}

void Converter::FindCurve()
{
	/*시간을 갯수만큼 읽고 float 값을 갯수만큼 읽음*/
	if (isSame(m_buffer, "AnimationCurve:"))
	{
		Curve tempCurve;
		int numOfKey;

		pass(&m_buffer, " ,");
		tempCurve.curveIdx = get64int(&m_buffer);
		passOver(&m_buffer, "KeyTime: ");
		numOfKey = getInt(&m_buffer);
		passOver(&m_buffer, "a: ");

		tempCurve.key.resize(numOfKey);

		for (int i = 0; i < numOfKey; ++i)
		{
			__int64 i64 = get64int(&m_buffer) * 0.01 / 30790772;
			float timePos = i64 / 30.0f;

			tempCurve.key[i].timePos = timePos;
			pass(&m_buffer, " ,\n\t");
		}

		passOver(&m_buffer, "KeyValueFloat:");
		passOver(&m_buffer, "a:");

		for (int i = 0; i < numOfKey; ++i)
		{
			tempCurve.key[i].value = getFloat(&m_buffer);
			pass(&m_buffer, " ,\n\t");
		}

		m_vCurve.push_back(tempCurve);
	}
}

void Converter::FindCurveNode()
{
	if (isSame(m_buffer, "AnimationCurveNode:"))
	{
		CurveNode tempNode;

		pass(&m_buffer, " ,");
		tempNode.nodeIdx = get64int(&m_buffer);
		m_vNode.push_back(tempNode);
	}
}

void Converter::FindDeformer()
{
	if (isSame(m_buffer, "Deformer:"))
	{
		pass(&m_buffer, " ,");
		__int64 deformerIdx = get64int(&m_buffer);
		pass(&m_buffer, " ,");
		if (isSame(m_buffer, "\"SubDeformer::\""))
		{
			Deformer tempDeformer;
			int numOfAffectVertices;
			std::vector<int> affectedVertexIdx;
			std::vector<float> weight;

			tempDeformer.deformerIdx = deformerIdx;

			passOver(&m_buffer, "Indexes:");
			numOfAffectVertices = getInt(&m_buffer);

			affectedVertexIdx.resize(numOfAffectVertices);
			weight.resize(numOfAffectVertices);
			tempDeformer.weight.resize(numOfAffectVertices);

			passOver(&m_buffer, "a:");
			
			for (int i = 0; i < numOfAffectVertices; ++i)
			{
				affectedVertexIdx[i] = getInt(&m_buffer);
				pass(&m_buffer, " ,");
			}

			passOver(&m_buffer, "Weight:");
			passOver(&m_buffer, "a:");

			for (int i = 0; i < numOfAffectVertices; ++i)
			{
				weight[i] = getFloat(&m_buffer);
				pass(&m_buffer, " ,");
			}

			for (int i = 0; i < numOfAffectVertices; ++i)
			{
				tempDeformer.weight[i].idx = affectedVertexIdx[i];
				tempDeformer.weight[i].weight = weight[i];
			}

			m_vDeformer.push_back(tempDeformer);
		}
	}
}

void Converter::FindTextures()
{
	if (isSame(m_buffer, "Texture:"))
	{
		passOver(&m_buffer, "FileName:");
		char * pTexturePath;
		char * pTextureName = NULL;
		pTexturePath = new char[_MAX_PATH*2];
		memcpy(pTexturePath, m_buffer, _MAX_PATH * 2);

		while (*pTexturePath)
		{
			if (*pTexturePath == '\\' && (pTexturePath + 1) != NULL)
			{
				pTextureName = pTexturePath + 1;
			}
			else
			{
			}

			pTexturePath++; //mv pointer

		}
		Name name;
		strcpy(name.string, pTextureName);
		m_vTextureName.push_back(name);
		m_buffer = strtok(NULL, " ,\n\t");
		m_buffer = strtok(NULL, " ,\n\t");
//		pass(&m_buffer, " \t\n");
	}
}

void Converter::FindPosition()
{
	if (isSame(m_buffer, "Vertices:"))
	{
		int numOfVertex;
		pass(&m_buffer, " ,*");
		numOfVertex = getInt(&m_buffer);
		passOver(&m_buffer, "a:");

		m_vPos.resize(numOfVertex/3);

		for (auto i = 0; i < numOfVertex/3; ++i)
		{
			float3 tempFloat3;
			tempFloat3 = getFloat3(&m_buffer);
			m_vPos[i] = tempFloat3;
		}
		printf("TEST\n");
	}
}

void Converter::FindVertexIdx()
{
	if (isSame(m_buffer, "PolygonVertexIndex:"))
	{
		int numOfIdx;
		pass(&m_buffer, " ,*");
		numOfIdx = getInt(&m_buffer);
		passOver(&m_buffer, "a:");

		m_vVertexIdx.resize(numOfIdx);

		for (auto i = 0; i < numOfIdx; ++i)
		{
			m_vVertexIdx[i] = getInt(&m_buffer);
			pass(&m_buffer, " ,\n\t");
		}
	}
}

void Converter::FindUV()
{
	if (isSame(m_buffer, "UV:"))
	{
		int numOfUV;
		pass(&m_buffer, " ,*");
		numOfUV = getInt(&m_buffer);
		passOver(&m_buffer, "a:");

		m_vUV.resize(numOfUV/2);

		for (auto i = 0; i < numOfUV/2; ++i)
		{
			UV tempUV;
			tempUV.u = getFloat(&m_buffer);
			pass(&m_buffer, " ,\n\t");
			tempUV.v = getFloat(&m_buffer);
			pass(&m_buffer, " ,\n\t");
			m_vUV[i] = tempUV;
		}
	}
}

void Converter::FindUVIdx()
{
	if (isSame(m_buffer, "UVIndex:"))
	{
		int numOfIdx;
		pass(&m_buffer, " ,*");
		numOfIdx = getInt(&m_buffer);
		passOver(&m_buffer, "a:");

		m_vUVIdx.resize(numOfIdx);

		for (auto i = 0; i < numOfIdx; ++i)
		{
			m_vUVIdx[i] = getInt(&m_buffer);
			pass(&m_buffer, " ,\n\t");
		}
	}
}

void Converter::FindNormal()
{
	if (isSame(m_buffer, "Normals:"))
	{
		int numOfElements;
		pass(&m_buffer, " ,*");
		numOfElements = getInt(&m_buffer);
		passOver(&m_buffer, "a:");

		m_vNormal.resize(numOfElements/3);

		for (auto i = 0; i < numOfElements/3; ++i)
		{
			float3 tempFloat3;
			tempFloat3 = getFloat3(&m_buffer);
			m_vNormal[i] = tempFloat3;
		}
	}
}

void Converter::FindBiNormal()
{
	if (isSame(m_buffer, "Binormals:"))
	{
		int numOfElements;
		pass(&m_buffer, " ,*");
		numOfElements = getInt(&m_buffer);
		passOver(&m_buffer, "a:");

		m_vBiNormal.resize(numOfElements/3);

		for (auto i = 0; i < numOfElements / 3; ++i)
		{
			float3 tempFloat3;
			tempFloat3 = getFloat3(&m_buffer);
			m_vBiNormal[i] = tempFloat3;
		}
	}
}

void Converter::FindTangent()
{
	if (isSame(m_buffer, "Tangents:"))
	{
		int numOfElements;
		pass(&m_buffer, " ,*");
		numOfElements = getInt(&m_buffer);
		passOver(&m_buffer, "a:");

		m_vTangent.resize(numOfElements/3);

		for (auto i = 0; i < numOfElements / 3; ++i)
		{
			float3 tempFloat3;
			tempFloat3 = getFloat3(&m_buffer);
			m_vTangent[i] = tempFloat3;
		}
	}
}

void Converter::FindOffsetMatrix()
{
	if (isSame(m_buffer, "PoseNode:"))
	{
		PoseNode offset;
		passOver(&m_buffer, "Node:");
		offset.poseIdx = get64int(&m_buffer);
		passOver(&m_buffer, "a:");
		for (int i = 0; i < 16; ++i)
		{
			offset.mat[i] = getFloat(&m_buffer);
			pass(&m_buffer, " ,\n\t");
		}
		m_vPoseNode.push_back(offset);
	}
}

void Converter::FindConnectionPoseNodeToBone()
{

}

void Converter::Find()
{
	FindCurve();
	FindCurveNode();
	FindDeformer();
	FindPosition();
	FindVertexIdx();
	FindUV();
	FindUVIdx();
	FindBiNormal();
	FindNormal();
	FindTangent();
	FindOffsetMatrix();
	FindConnectionPoseNodeToBone();
}

void Converter::ConnectVertex()
{
	m_vVertex.resize(m_vPos.size());

	for (auto i = 0; i < m_vPos.size(); ++i)
	{
		m_vVertex[i].pos = m_vPos[i];
	}

	/*
		Deformer 구조: 본 인덱스, weight(영향을 줄 버텍스, 해당 버텍스에 줄 가중치)
		Vertex 구조: 영향받을 본 인덱스 4개, 가중치 4개, 위치

		따라서 디포머를 순회하면서 버텍스[영향을 줄 인덱스].가중치에 가중치를 주고
		그 버텍스[영향을 줄 인덱스].본 인덱스에 현재 디포머의 본 인덱스를 주면 됨

		단, 해당 버텍스에 정보를 넣어줄 땐 별도의 받는 함수가 있어야 함.
		따라서 inputWeight() 함수를 만들 것.
	
	*/
	for (auto i = 0; i < m_vDeformer.size(); ++i)
	{
		for (auto j = 0; j < m_vDeformer[i].weight.size(); ++j)
		{
			/*
			이게 무슨 소린가 싶겠지만
			버텍스[현재 디포머의 가중치 정보에 있는 '영향을 줄 버텍스 인덱스'].가중치 정보를 갱신하는 내용이다..
			깔끔하지 못한 코드 죄악 그 자체
			*/
			m_vVertex[m_vDeformer[i].weight[j].idx].inputWeight(m_vDeformer[i].boneIdx, m_vDeformer[i].weight[j].weight);
		}
	}
}

void Converter::ConnectUV()
{
}

void Converter::ConnectMesh()
{
	for (auto i = 0; i < m_vVertexIdx.size(); ++i)
	{
		if (i % 3 == 2) m_mesh.vertexIdx.push_back(m_vVertexIdx[i] * -1 - 1);
		else m_mesh.vertexIdx.push_back(m_vVertexIdx[i]);
	}

	for (auto i = 0; i < m_vUVIdx.size(); ++i)
	{
		m_mesh.uvIdx.push_back(m_vUVIdx[i]);
	}

	for (auto i = 0; i < m_vNormal.size(); ++i)
	{
		m_mesh.normal.push_back(m_vNormal[i]);
	}

	for (auto i = 0; i < m_vBiNormal.size(); ++i)
	{
		m_mesh.biNormal.push_back(m_vBiNormal[i]);
	}

	for (auto i = 0; i < m_vTangent.size(); ++i)
	{
		m_mesh.tangent.push_back(m_vTangent[i]);
	}
}

void Converter::ConnectAnimation()
{
}

void Converter::ConnectBone()
{
}

void Converter::Connect()
{
	ConnectVertex();
	ConnectUV();
	ConnectMesh();
	ConnectAnimation();
	ConnectBone();
}
