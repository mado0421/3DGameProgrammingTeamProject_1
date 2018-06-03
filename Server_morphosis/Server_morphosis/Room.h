#pragma once
#include "stdafx.h"
#include "Player.h"
class CPlayer;
class CGameIocp;
class CRoom
{
public:
	CRoom();
	virtual ~CRoom();
private:
	chrono::high_resolution_clock::time_point mGameTime;
	chrono::high_resolution_clock::time_point mLastTime;
	queue<order*>*			mQueue;
	int						mIndex;										//방 고유 번호
	int						mPlayerNumber;
	bool					mIsRoomStarted;								//로비에서 시작 버튼이 눌렸는지 확인하는 변수
	bool					mIsGameStarted;								//게임이 진행중인지 확인하는 변수

	int						mMapIndex;									//방에 설정된 맵의 번호

	double					mGameTimer;
	CPlayer*				mRoomUser[MAX_USER_COUNT];					//각 사용자들의 정보 포인터, 봇은 유저의 일종으로 처리할 예정
	
	//마지막 플레이어가 들어온 시간, 일정 시간이 경과했을 경우에는 빈 자리를 봇으로 채우고 시작한다.
public:
	BOOL					Begin(int index, queue<order>* queue);																	//initiate, 개체를 시작하는 함수
	BOOL					End(VOID);																			//방 닫을 때 해제해주는 함수

	BOOL					JoinUser(CPlayer *connectedUser);													//플레이어가 추가되었을때 부르는 함수
	BOOL					LeaveUser(CGameIocp *iocp, CPlayer *leavedUser);				//플레이어가 게임을 떠났을 때 부르는 함수, 칸을 비워주거나 봇으로 바꿔야 할 듯

	BOOL					WriteAll(int protocol, char *packet, int packetLength);							//방 안의 모든 사용자에게 패킷을 전송할 때 사용하는 함수
	BOOL					WriteAllInitHPAP(VOID);	//게임 시작시 모든 플레이어에게 각자의 정보를 전송하는 함수															//방 안의 모든 사용자에게 초기값을 전송하는 함수

	BOOL					IsAllLoadComplete(VOID);															//모두 로딩이 끝났는지 확인하는 함수

	DWORD					RoomStart(VOID);																	//시간이 다 지나거나 플레이어가 꽉 찼을 경우 게임을 시작하는 함수
	BOOL					GameStart(VOID);																	//실제 게임 루프를 시작하는 함수
	BOOL					GameEnd(CGameIocp *iocp);															//게임이 끝났을 때 호출하는 함수

	//남은 게임 시간을 줄이면서 끝났는지 확인하는 함수
	BOOL					DecreaseGameTime(VOID);
	//현재 방이 비어있는지 확인하는 함수
	BOOL					GetIsEmpty(VOID);
	//방이 꽉 차면 시작을 활성화 시키는 함수
	inline BOOL				GetIsFull(VOID);
	
	VOID					Update(float fElapsedTime);

};
