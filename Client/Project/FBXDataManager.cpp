#include "stdafx.h"
#include "FBXDataManager.h"


FBXDataManager::FBXDataManager(std::string fileName="")
{
	m_fileName		= fileName;

	m_pMaterial		= nullptr;
	m_nMaterial		= 0;

	m_pName			= nullptr;
	m_nName			= 0;

	m_pVertex		= nullptr;
	m_nVertex		= 0;
	
	m_pUV			= nullptr;
	m_nUV			= 0;

	m_pMesh			= nullptr;
	m_nVertexIdx	= 0;
	m_nUVIdx		= 0;
	m_nNormal		= 0;
	m_nBiNormal		= 0;
	m_nTangent		= 0;
}

FBXDataManager::~FBXDataManager()
{
}

bool FBXDataManager::FileRead(std::string fileName)
{
	m_fileName = fileName;
	return FileRead();
}

bool FBXDataManager::FileRead()
{
	std::ifstream fs;

	fs.open(m_fileName, std::ios::binary);
	if (fs.fail())return false;

	/*	Read Material	*/
	fs.read((char*)&m_nMaterial, sizeof(int));
	if (m_nMaterial > 0){
		m_pMaterial = new cv::Material[m_nMaterial];
		fs.read((char*)m_pMaterial, sizeof(cv::Material)*m_nMaterial);
	}
	else{
		return FileErrorShutDown(fs);
	}

	/*	Read Texture File Name	*/
	fs.read((char*)&m_nName, sizeof(int));
	if (m_nName > 0) {
		m_pName = new cv::Name[m_nName];
		fs.read((char*)m_pName, sizeof(cv::Name)*m_nName);
	}
	else {
		return FileErrorShutDown(fs);
	}

	/*	Read Vertex	*/
	fs.read((char*)&m_nVertex, sizeof(int));
	if (m_nVertex > 0) {
		m_pVertex = new cv::Vertex[m_nVertex];
		fs.read((char*)m_pVertex, sizeof(cv::Vertex)*m_nVertex);
	}
	else {
		return FileErrorShutDown(fs);
	}

	/*	Read UV	*/
	fs.read((char*)&m_nUV, sizeof(int));
	if (m_nUV > 0) {
		m_pUV = new cv::UV[m_nUV];
		fs.read((char*)m_pUV, sizeof(cv::UV)*m_nUV);
	}
	else {
		return FileErrorShutDown(fs);
	}

	/*	Read Mesh	*/
	m_pMesh = new cv::Mesh;

	//	Vertex Index
	fs.read((char*)&m_nVertexIdx, sizeof(int));
	if (m_nVertexIdx > 0) {
	m_pMesh->vertexIdx = new int[m_nVertexIdx];
	fs.read((char*)m_pMesh->vertexIdx, sizeof(int)*m_nVertexIdx);
	}
	else {
		return FileErrorShutDown(fs);
	}
	//	UV Index
	fs.read((char*)&m_nUVIdx, sizeof(int));
	if (m_nUVIdx > 0) {
		m_pMesh->uvIdx = new int[m_nUVIdx];
		fs.read((char*)m_pMesh->uvIdx, sizeof(int)*m_nUVIdx);
	}
	else {
		return FileErrorShutDown(fs);
	}
	//	Normal
	fs.read((char*)&m_nNormal, sizeof(int));
	if (m_nNormal > 0) {
		m_pMesh->normal = new cv::float3[m_nNormal];
		fs.read((char*)m_pMesh->normal, sizeof(cv::float3)*m_nNormal);
	}
	else {
		return FileErrorShutDown(fs);
	}
	//	BiNormal
	fs.read((char*)&m_nBiNormal, sizeof(int));
	if (m_nBiNormal > 0) {
		m_pMesh->biNormal = new cv::float3[m_nBiNormal];
		fs.read((char*)m_pMesh->biNormal, sizeof(cv::float3)*m_nBiNormal);
	}
	else {
		return FileErrorShutDown(fs);
	}
	//	Tangent
	fs.read((char*)&m_nTangent, sizeof(int));
	if (m_nTangent > 0) {
		m_pMesh->tangent = new cv::float3[m_nTangent];
		fs.read((char*)m_pMesh->tangent, sizeof(cv::float3)*m_nTangent);
	}
	else {
		return FileErrorShutDown(fs);
	}

	fs.close();
}

void FBXDataManager::Destroy()
{
	if (m_pMaterial)delete[] m_pMaterial;
	if (m_pName)delete[] m_pName;
	if (m_pVertex)delete[] m_pVertex;
	if (m_pUV)delete[] m_pUV;

	if (m_pMesh)
	{
		if (m_pMesh->vertexIdx)delete[] m_pMesh->vertexIdx;
		if (m_pMesh->uvIdx)delete[] m_pMesh->uvIdx;
		if (m_pMesh->normal)delete[] m_pMesh->normal;
		if (m_pMesh->biNormal)delete[] m_pMesh->biNormal;
		if (m_pMesh->tangent)delete[] m_pMesh->tangent;
		delete m_pMesh;
	}
}
