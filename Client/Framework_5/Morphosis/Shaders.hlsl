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

cbuffer cbGameUIInfo : register(b2)
{
	matrix		gmtxGameUI : packoffset(c0);
	float4		gf4Rect : packoffset(c4);
	uint		gnUIMaterial : packoffset(c8);
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UI Shader

VS_TEXTURED_OUTPUT VSUI(VS_TEXTURED_INPUT input, uint nVertexID : SV_VertexID)
{
	VS_TEXTURED_OUTPUT output;

	output.position = float4(input.position, 1.0f);
	//if (nVertexID % 6 == 0) {
	//	output.position.x + gf4Rect.x;
	//	output.position.y + gf4Rect.y;
	//}
	//else if (nVertexID % 6 == 1) {
	//	output.position.x + gf4Rect.z;
	//	output.position.y + gf4Rect.y;
	//}
	//else if (nVertexID % 6 == 2) {
	//	output.position.x + gf4Rect.z;
	//	output.position.y + gf4Rect.w;
	//}
	//else if (nVertexID % 6 == 3) {
	//	output.position.x + gf4Rect.x;
	//	output.position.y + gf4Rect.y;
	//}
	//else if (nVertexID % 6 == 4) {
	//	output.position.x + gf4Rect.z;
	//	output.position.y + gf4Rect.w;
	//}
	//else if (nVertexID % 6 == 5) {
	//	output.position.x + gf4Rect.x;
	//	output.position.y + gf4Rect.w;
	//}
	output.uv = input.uv;

	//output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameUI);
	//output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	//output.position = mul(mul(float4((float3)mul(float4(input.position, 1.0f), gmtxGameUI), 1.0f), gmtxView), gmtxProjection);
	//float3 temp = (float3)mul(float4(input.position, 1.0f), gmtxGameUI);
	//output.position = mul(mul(float4(temp, 1.0f), gmtxView), gmtxProjection);
	//if (gmtxGameUI._11 + gmtxGameUI._12 + gmtxGameUI._13 + gmtxGameUI._14 +
	//	gmtxGameUI._21 + gmtxGameUI._22 + gmtxGameUI._23 + gmtxGameUI._24 +
	//	gmtxGameUI._31 + gmtxGameUI._32 + gmtxGameUI._33 + gmtxGameUI._34 +
	//	gmtxGameUI._41 + gmtxGameUI._42 + gmtxGameUI._43 + gmtxGameUI._44 != 0.0f) {
	output.position = mul(mul(float4((float3)mul(float4(input.position, 1.0f), gmtxGameUI), 1.0f), gmtxView), gmtxProjection);

	//}
	return(output);
}

float4 PSUI(VS_TEXTURED_OUTPUT input) : SV_TARGET
{

	float4 cColor = gtxtTexture.Sample(gSamplerState, input.uv);
	//clip(cColor.a - 0.1f);
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

	output.normalW = mul(input.normal, (float3x3)gmtxGameObject);
	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.uv = input.uv;
	//if (!Test()){

	//}
	//else {
	//	if (nVertexID == 0) { output.position		= float4(	0.0f,	0.5f,	0.5f,	1.0f); output.uv = float2(0.5f, 0.0f); }
	//	else if (nVertexID == 1) { output.position	= float4(	0.5f,	-0.5f,	0.5f,	1.0f); output.uv = float2(1.0f, 1.0f); }
	//	else if (nVertexID == 2) { output.position	= float4(-	0.5f,	-0.5f,	0.5f,	1.0f); output.uv = float2(0.0f, 1.0f); }
	//}

	return output;
}

float4 PS(VS_TEXTURED_ILLUMINATED_OUTPUT input) : SV_TARGET
{

	float4 cColor = gtxtTexture.Sample(gSamplerState, input.uv);
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);

	return (lerp(cColor, cIllumination, 0.9f));
//	return cIllumination;
}