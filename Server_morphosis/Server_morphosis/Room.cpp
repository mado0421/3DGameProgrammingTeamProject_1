#include "stdafx.h"
#include "Room.h"


CRoom::CRoom()
{
}


CRoom::~CRoom()
{
}

BOOL CRoom::Begin(int index, queue<order>* queue)
{
	mIndex = 0;//전역이든 보내주든 해야 할 듯
	mIsRoomStarted = false;
	mIsGameStarted = false;
	mPlayerNumber = 0;
	mMapIndex = index;
	mQueue = queue;
	for (int i = 0; i < MAX_USER_COUNT; ++i)
	{
		mRoomUser[i] = nullptr;
	}
	return true;
}

BOOL CRoom::End(VOID)
{
	return 0;
}

BOOL CRoom::JoinUser(CPlayer * connectedUser)
{
	//이러면 접속했을 때 받는 정보로 내용을 채워줬어야 함
	for (int i = 0; i < MAX_USER_COUNT; ++i)
	{
		if (mRoomUser[i] == nullptr)
		{
			mRoomUser[i] = connectedUser;
			mPlayerNumber++;
			//팀 넣어 줘야 함.
			return true;
		}
	}
	return false;
}

BOOL CRoom::LeaveUser(CGameIocp * iocp, CPlayer * leavedUser)
{
	for (int i = 0; i < MAX_USER_COUNT; ++i)
	{
		if (leavedUser == mRoomUser[i])
		{
			mRoomUser[i] = nullptr;
			mPlayerNumber--;
			(*mQueue).push(new order{ leave,"a" });
			return true;
		}
	}
	return false;
}

BOOL CRoom::WriteAll(int protocol, char * packet, int packetLength)
{
	//모든 플레이어의 소켓에 명령어 보내기
	for (int i = 0; i < MAX_USER_COUNT; ++i)
	{
		if (mRoomUser[i]->getType())
			(*mQueue).push(new order{ protocol, *packet });
	}
}

BOOL CRoom::WriteAllInitHPAP(VOID)
{
	for (int i = 0; i < MAX_USER_COUNT; ++i)
	{
		if (mRoomUser[i]->getType())
			(*mQueue).push(new order{ 0, *(char*)mRoomUser[i] });
	}
}

BOOL CRoom::IsAllLoadComplete(VOID)
{
	return 0;
}

DWORD CRoom::RoomStart(VOID)
{
	for (int i = 0; i < MAX_USER_COUNT; ++i)
	{
		if (mRoomUser[i]->getType())
		{
			(*mQueue).push(new order{ 0, *(char*)mRoomUser[i] });
		}
	}
}

BOOL CRoom::GameStart(VOID)
{
	for (int i = 0; i < MAX_USER_COUNT; ++i)
	{
		if (mRoomUser[i]->getType())
			(*mQueue).push(new order{ 0, *(char*)mRoomUser[i] });
	}
}

BOOL CRoom::GameEnd(CGameIocp * iocp)
{
	for (int i = 0; i < MAX_USER_COUNT; ++i)
	{
		if (mRoomUser[i]->getType())
			(*mQueue).push(new order{ 0, *(char*)mRoomUser[i] });
	}
}

BOOL CRoom::DecreaseGameTime(VOID)
{
	auto time = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - mGameTime);
	if(time.count()>10)
	{
		mGameTime = chrono::high_resolution_clock::now();
	}
	(*mQueue).push(new order{ 0, *(char*)mRoomUser[i] });
}

BOOL CRoom::GetIsEmpty(VOID)
{
	if (mPlayerNumber == 0)
		return true;
	return false;
}

inline BOOL CRoom::GetIsFull(VOID)
{
	if (mPlayerNumber == 8)
		return true;
	return false;
}

VOID CRoom::Update(float fElapsedTime)
{
	for (int i = 0; i < MAX_USER_COUNT; ++i)
	{
		if (mRoomUser[i]->getType())
		{
			mRoomUser[i]->update(fElapsedTime);
		}
	}
	//충돌체크..

}

