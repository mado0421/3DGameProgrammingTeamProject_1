cbuffer cbCameraInfo : register(b0)
{
	matrix		gmtxView : packoffset(c0);
	matrix		gmtxProjection : packoffset(c4);
	float3		gvCameraPosition : packoffset(c8);
};

cbuffer cbGameObjectInfo : register(b1)
{
	matrix		gmtxGameObject : packoffset(c0);
	uint		gnMaterial : packoffset(c4);
};

#include "Light.hlsl"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
Texture2D gtxtTexture : register(t4);
SamplerState gSamplerState : register(s0);

struct VS_TEXTURED_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_TEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

#define EPSILON	1.0e-10f
bool isAlmostSame(float a, float b)
{
	if (a - b > EPSILON) return false;
	if (b - a > EPSILON) return false;
	return true;
}

VS_TEXTURED_OUTPUT VSTextured(VS_TEXTURED_INPUT input)
{
	VS_TEXTURED_OUTPUT output;

	output.position = float4(input.position, 1.0f);
	output.uv = input.uv;

	return(output);
}

float4 PSTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtTexture.Sample(gSamplerState, input.uv);
	clip(cColor.a - 0.1f);
//	if(cColor.a < 0.1) discard; // 투명 넣기
	return(cColor);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////

struct VS_ILLUMINATED_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

struct VS_ILLUMINATED_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float4 color : COLOR;
};

VS_ILLUMINATED_OUTPUT VSIlluminated(VS_ILLUMINATED_INPUT input)
{
	VS_ILLUMINATED_OUTPUT output;
	
	output.normalW = mul(input.normal, (float3x3)gmtxGameObject);
	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);

	return(output);
}

float4 PSIlluminated(VS_ILLUMINATED_OUTPUT input) : SV_TARGET
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	return(cIllumination);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
struct VS_TEXTURED_ILLUMINATED_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct VS_TEXTURED_ILLUMINATED_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 uv : TEXCOORD;
};

VS_TEXTURED_ILLUMINATED_OUTPUT VS(VS_TEXTURED_ILLUMINATED_INPUT input, uint nVertexID : SV_VertexID)
{
	VS_TEXTURED_ILLUMINATED_OUTPUT output;
	/*
	개삽질하면서 알아낸 사실
	gmtxGameObject는 다 0.0f 다. (단위행렬도 아닌거 같음) <-- 이거 그러면 오브젝트 갱신이 안되고 있는거잖아 
	gmtxGameObject에 갱신이 안되는 모양
	output.positionW 가 뭔진 몰라도 하여간 0.0f, 0.0f, 0.0f 는 아닌거 같다
	
	output.position = float4(input.position, 1.0f); 하면 네모난게 나오긴 함

	05.21.1155	- gmtxView, gmtxProjection, gmtxGameObject 모두 값이 들어가는걸 확인
				- output.positionW도 뭔가 값이 들어가긴 함.
				- 이젠 메쉬 크기나 위치를 살펴볼 시간? 아니면 카메라?
				- 생각해보니까 이제 gmtxView랑 gmtxProjection을 보는데 카메라도 봐야겠네

	*/


	output.normalW = mul(input.normal, (float3x3)gmtxGameObject);
	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
//	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	//		output.position = float4(input.position, 1.0f);

	//	if (false) {

	if (/*gmtxView._11 + gmtxView._12 + gmtxView._13 + gmtxView._14 + 
		gmtxView._21 + gmtxView._22 + gmtxView._23 + gmtxView._24 + 
		gmtxView._31 + gmtxView._32 + gmtxView._33 + gmtxView._34 +
		gmtxView._41 + gmtxView._42 + gmtxView._43 + gmtxView._44 != 0.0f*/
		output.positionW.x + output.positionW.y + output.positionW.z != 0.0f){

		output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
		output.uv = input.uv;
	}
	else {

		if (nVertexID == 0) { output.position		= float4(	0.0f,	0.5f,	0.5f,	1.0f); output.uv = float2(0.5f, 0.0f); }
		else if (nVertexID == 1) { output.position	= float4(	0.5f,	-0.5f,	0.5f,	1.0f); output.uv = float2(1.0f, 1.0f); }
		else if (nVertexID == 2) { output.position	= float4(-	0.5f,	-0.5f,	0.5f,	1.0f); output.uv = float2(0.0f, 1.0f); }
	}
//	output.position  = float4(0.5f, 0.5f, 0.5f, 1.0f);
//	output.uv = input.uv;


	return output;
}

float4 PS(VS_TEXTURED_ILLUMINATED_OUTPUT input) : SV_TARGET
{

	float4 cColor = gtxtTexture.Sample(gSamplerState, input.uv);
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);	// 얘 쓰면 터짐 시발 왜? 뭔가가 뭐 0이나 그렇겠지

	return cColor;
//	return float4(0.0f, 0.0f, 0.0f, 0.0f);
//	return cIllumination;
//	return cColor;
}