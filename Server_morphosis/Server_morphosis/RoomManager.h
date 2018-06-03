#pragma once
#include "Room.h"
class CRoomManager
{
	vector<CRoom> mRooms;
	int roomNum;
public:
	CRoomManager();
	~CRoomManager();
	BOOL	Begin(DWORD maxRoomCount);
	BOOL	End(VOID);

	CRoom*	QuickJoin(CPlayer *connectedUser, USHORT &slotIndex);

	BOOL	CheckGameTime(CGameIocp *iocp);
};

