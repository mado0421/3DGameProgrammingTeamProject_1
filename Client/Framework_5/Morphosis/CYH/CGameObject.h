#pragma once
//움직이는 게임 오브젝트
class CGameObject
{
public:
	int m_objectID;
	XMFLOAT4X4 m_world;
	XMFLOAT3 m_vector;
	BoundingSphere m_BS;
	//float m_radius;
	CGameObject() = delete;
	CGameObject(int objectID);
	CGameObject(int objectID, XMFLOAT4X4 & world, XMFLOAT3 & vector, float radius);
	~CGameObject();

	virtual void update(float time);
	virtual bool collisionCheck(BoundingSphere& other);
	virtual void initiate(XMFLOAT4X4& world, XMFLOAT3& vector, float size);
};

