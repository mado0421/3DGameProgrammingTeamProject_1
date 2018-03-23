#pragma once
#ifndef _FSTREAM_
#include<fstream>
#endif // !_FSTREAM_

class FBXDataManager
{
public:
	FBXDataManager(std::string fileName);
	virtual ~FBXDataManager();

	bool FileRead(std::string fileName);
	bool FileRead();
	void Destroy();
	inline bool FileErrorShutDown(std::ifstream& fs) {
		fs.close();
		Destroy();
		return false;
	};

public:
	std::string m_fileName;

	cv::Material * m_pMaterial;
	int m_nMaterial;

	cv::Name* m_pName;
	int m_nName;

	cv::Vertex* m_pVertex;
	int m_nVertex;

	cv::UV* m_pUV;
	int m_nUV;

	cv::Mesh* m_pMesh;
	int m_nVertexIdx;
	int m_nUVIdx;
	int m_nNormal;
	int m_nBiNormal;
	int m_nTangent;

};



