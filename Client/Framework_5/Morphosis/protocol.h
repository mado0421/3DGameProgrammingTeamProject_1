#pragma once
#pragma pack(push, 1)

namespace scKIND
{
	enum type
	{
		addPlayer
	};
}

namespace csKIND
{
	enum type
	{
		addPlayer
	};
}

struct cs_packet_addplayer
{
	BYTE size;
	BYTE type;
	BYTE weapon;//무기
	BYTE main1;//스킬종류 1, 11
	BYTE sub11;
	BYTE sub12;
	BYTE main2;
	BYTE sub21;
	BYTE sub22;
	BYTE main3;
	BYTE sub31;
	BYTE sub32;
	float posX;//위치
	float posY;
	float posZ;
	float vectorX;
	float vectorY;
	float vectorZ;
	BYTE hp;
};

struct sc_packet_addplayer
{
	BYTE size;
	BYTE type;
	BYTE weapon;//무기
	BYTE main1;//스킬종류 1, 11
	BYTE sub11;
	BYTE sub12;
	BYTE main2;
	BYTE sub21;
	BYTE sub22;
	BYTE main3;
	BYTE sub31;
	BYTE sub32;
	float posX;//위치
	float posY;
	float posZ;
	float vectorX;
	float vectorY;
	float vectorZ;
	BYTE hp;
	int id;
};

#pragma pack(pop)