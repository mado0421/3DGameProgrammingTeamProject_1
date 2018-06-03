#pragma once
class CGameIocp : public CIocp
{
	//room 매니저와 플레이어 매니저를 둘 다 갖고있으며, 플레이어 내부에 통신기능이 들어갈 예정
public:
	CGameIocp();
	~CGameIocp();
};

