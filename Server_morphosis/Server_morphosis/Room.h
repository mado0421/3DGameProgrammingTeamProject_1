#pragma once
class CRoom
{
public:
	CRoom();
	virtual ~CRoom();

private:
	DWORD					mIndex;							//방 고유 번호

	BOOL					mIsRoomStarted;					//로비에서 시작 버튼이 눌렸는지 확인하는 변수
	BOOL					mIsGameStarted;					//게임이 진행중인지 확인하는 변수

	DWORD					mMapIndex;						//방에 설정된 맵의 번호

	CConnectedUser*			mRoomUser[8];					//각 사용자들의 정보, 봇은 유저의 일종으로 처리할 예정
	USHORT					mCurrentUserCount;				//현재 유저의 수

	CConnectedUser			*mRootUser;						//방을 생성한 유저, 여러 세팅이 가능하게 할 예정

	DWORD					mRemainGameTime;				//남은 게임 시간

	//////////////////////////////////////////////////////////////////////////
	// 게임관련 변수들
	//CConnectedUser			*mLastShootUser;
	//CConnectedUser			*mLastPassUser;

	//////////////////////////////////////////////////////////////////////////

public:
	BOOL					Begin(DWORD index);																	//initiate, 개체를 시작하는 함수
	BOOL					End(VOID);																			//방 닫을 때 해제해주는 함수

	BOOL					JoinUser(CConnectedUser *connectedUser, USHORT &slotIndex);							//플레이어가 추가되었을때 부르는 함수
	BOOL					LeaveUser(BOOL isDisconnected, CGameIocp *iocp, CConnectedUser *connectedUser);		//플레이어가 게임을 떠났을 때 부르는 함수

	BOOL					WriteAll(DWORD protocol, BYTE *packet, DWORD packetLength);							//방 안의 모든 사용자에게 패킷을 전송할 때 사용하는 함수
	BOOL					WriteAllInitHPAP(VOID);																//방 안의 모든 사용자에게 초기값을 전송하는 함수

	BOOL					IsAllLoadComplete(VOID);															//모두 준비가 끝났는지 확인하는 함수
	BOOL					IsAllIntroComplete(VOID);															//

	DWORD					RoomStart(VOID);																	//방장이 게임 시작 버튼을 눌렀을 때 실행하는 함수
	BOOL					GameStart(VOID);																	//실제 게임 루프를 시작하는 함수
	BOOL					GameEnd(CGameIocp *iocp);															//게임이 끝났을 때 호출하는 함수

	//남은 게임 시간을 줄이면서 끝났는지 확인하는 함수
	inline BOOL				DecreaseGameTime(VOID) { CThreadSync Sync; mRemainGameTime = max(INT(mRemainGameTime--), 0); return mRemainGameTime ? TRUE : FALSE; }
	//현재 방이 비어있는지 확인하는 함수
	inline BOOL				GetIsEmpty(VOID) { CThreadSync Sync; return mCurrentUserCount ? FALSE : TRUE; }	
	//방이 꽉 차면 시작을 활성화 시키는 함수
	inline BOOL				GetIsFull(VOID) { CThreadSync Sync; return mCurrentUserCount == 8 ? TRUE : FALSE; }
	//게터와 세터
	inline BOOL				GetIsRoomStarted(VOID) { CThreadSync Sync; return mIsRoomStarted; }
	inline BOOL				GetIsGameStarted(VOID) { CThreadSync Synd; return mIsGameStarted; }
	inline CConnectedUser** GetRoomUser(VOID) { CThreadSync Sync; return mRoomUser; }
	inline DWORD			GetIndex(VOID) { CThreadSync Sync; return mIndex; }
	inline LPTSTR			GetTitle(VOID) { CThreadSync Sync; return mTitle; }
	inline DWORD			GetMapIndex(VOID) { CThreadSync Sync; return mMapIndex; }
	inline DWORD			SetMapIndex(DWORD mapIndex) { CThreadSync Sync; mMapIndex = mapIndex; return TRUE; }
	inline USHORT			GetCurrentUserCount(VOID) { CThreadSync Sync; return mCurrentUserCount; }
	inline CConnectedUser*	GetRootUser(VOID) { CThreadSync Sync; return mRootUser; }

	DWORD					GetTeam(CConnectedUser *player);

	//////////////////////////////////////////////////////////////////////////
	// 게임관련 함수들
	inline BOOL				SetBallUser(CConnectedUser* connectedUser) { CThreadSync Sync; mBallUser = connectedUser; return TRUE; }
	inline CConnectedUser*	GetBallUser(VOID) { CThreadSync Sync; return mBallUser; }

	//inline BOOL				SetLastShootUser(CConnectedUser* connectedUser){CThreadSync Sync;mLastShootUser = connectedUser;return TRUE;}
	//inline CConnectedUser*	GetLastShootUser(VOID){CThreadSync Sync;return mLastShootUser;}

	//inline BOOL				SetLastPassUser(CConnectedUser* connectedUser){CThreadSync Sync;mLastPassUser = connectedUser;return TRUE;}
	//inline CConnectedUser*	GetLastPassUser(VOID){CThreadSync Sync;return mLastPassUser;}

	inline BOOL				DecreaseExplosionTime(VOID) { CThreadSync Sync; mExplosionTimeForPersion = max(INT(mExplosionTimeForPersion--), 0); mExplosionTimeForTeam = max(INT(mExplosionTimeForTeam--), 0); if (!mExplosionTimeForPersion || !mExplosionTimeForTeam)return FALSE; return TRUE; }
	inline BOOL				IsReadyExplosion(VOID) { CThreadSync Sync; if (mExplosionReadyFlag)return FALSE; if (mExplosionTimeForPersion <= DEFAULT_EXPLISION_ALRAM_TIME || mExplosionTimeForTeam <= DEFAULT_EXPLISION_ALRAM_TIME) { mExplosionReadyFlag = TRUE; return TRUE; }return FALSE; }
	inline VOID				ResetExplosionTime(VOID) { CThreadSync Sync; mExplosionTimeForPersion = DEFAULT_EXPLOSION_TIME_FOR_PERSION; mExplosionTimeForTeam = DEFAULT_EXPLOSION_TIME_FOR_TEAM; }
	inline VOID				ResetExplosionTimeForPersion(VOID) { CThreadSync Sync; mExplosionTimeForPersion = DEFAULT_EXPLOSION_TIME_FOR_PERSION; }

	inline BOOL				DecreaseTurnOverTime(VOID) { CThreadSync Sync; mTurnOverTime = max(INT(mCurrentUserCount--), 0); if (!mTurnOverTime)return FALSE; return TRUE; }
	inline BOOL				IsReadyTurnOver(VOID) { CThreadSync Sync; if (mTurnOverReadyFlag)return FALSE; if (mTurnOverTime <= DEFAULT_EXPLISION_ALRAM_TIME) { mTurnOverReadyFlag = TRUE; return TRUE; }return FALSE; }
	inline VOID				ResetTurnOverTime(VOID) { CThreadSync Sync; mTurnOverTime = DEFAULT_TURNOVER_TIME; }

	BOOL					IsSameTeam(CConnectedUser *player1, CConnectedUser *player2);
	BOOL					IsAllOut(CConnectedUser *player);

	VOID					PassInfo(CConnectedUser *shooter, CConnectedUser *target);
	VOID					PassReceiveInfo(CConnectedUser *shooter, CConnectedUser *target);

	VOID					ShootInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT power);
	VOID					ShootReceiveInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT power);
	VOID					ShootHitInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT power, CGameIocp *gameIocp);

	VOID					SpecialShootInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT shootType);
	VOID					SpecialShootReceiveInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT shootType);
	VOID					SpecialShootHitInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT shootType, CGameIocp *gameIocp);

	VOID					SkillInfo(CConnectedUser *actor, CConnectedUser *target, USHORT skillType);

	VOID					GetFieldBall(CConnectedUser *user, DWORD parameter);
	VOID					FieldBall(CConnectedUser *user);

	VOID					CheckExplosion(CGameIocp *gameIocp);

	FLOAT					GetAdVantage(CConnectedUser *user);
	//////////////////////////////////////////////////////////////////////////
};
