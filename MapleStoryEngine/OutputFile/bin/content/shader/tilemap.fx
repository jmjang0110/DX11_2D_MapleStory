#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"
#include "struct.fx"

StructuredBuffer<tTileData> TileDataBuffer : register(t16);

// ==============
// TileMap Shader
// Domain : Mask
// Blend : Default
// Depth : LESS
#define TileCountX  g_int_0
#define TileCountY  g_int_1
#define ShowGrid    g_int_2 // 0 : No 1 : Yes

#define SliceSizeUV g_vec2_0
// ==============

struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VTX_OUT VS_TileMap(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_TileMap(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;    
    
    // 0~1 의 UV 를 NxM 타일 크기로 확장한다
    float2 vUV = _in.vUV * float2(TileCountX, TileCountY);// * SliceSizeUV + LeftTopUV;
    
    // 각 타일 위치를 정수단위로 나누기 위해서 정수값을 취한다.
    // 해당 픽셀이 위치하는 타일의 행, 렬 정보를 알아 낼 수 있다.
    int2 iTileRowCol = floor(vUV);
    
    // 얻은 타일 행렬 정보를 일차원 인덱스로 변환(타일 데이터 배열에 접근하기 위함)
    int TileDataIdx = iTileRowCol.y * TileCountX + iTileRowCol.x;
    
    // 타일 데이터에 접근하면, 각 타일 별로 좌상단 UV 값이 들어있다.
    // 만약 이미지 설정이 안된 타일이면 버림
    if (-1 == TileDataBuffer[TileDataIdx].iImgIdx)
    {
       if(ShowGrid == 1)
        {
            vOutColor = float4(0.5f, 1.f, 1.f, 1.f);
            if (iTileRowCol.x + 0.03f < vUV.x && vUV.x < iTileRowCol.x + 0.97f
            && iTileRowCol.y + 0.03f < vUV.y && vUV.y < iTileRowCol.y + 0.97f)
            {
                discard;
            }
        }
       
        else
            discard;
    }
    else
    {
        float2 vLeftTopUV = TileDataBuffer[TileDataIdx].vLTUV;
    
    // 0~1 의 UV 를 NxM 타일 크기로 확장 UV 의 소수점 부분을 샘플링 용도로 사용한다.
        float2 vImgUV = frac(vUV);
    
    // 최종 Sample 위치 UV 를 계산한다.
        float2 vSampleUI = vLeftTopUV + vImgUV * SliceSizeUV;

        vOutColor = g_tex_0.Sample(g_sam_1, vSampleUI);
        
   // vOutColor *= 1.5f;
    }
    
    //if(vOutColor.a <= 0.0f)
    //    discard;
    
    return vOutColor;
}



// ==============
// TileMap2 Shader - for Damage Skin 
// Domain : Mask
// Blend : Default
// Depth : LESS
#define TileCountX  g_int_0
#define TileCountY  g_int_1
#define ShowGrid    g_int_2 // 0 : No 1 : Yes

#define Offset      g_vec2_2
#define StartLT     g_vec2_1
#define SliceSizeUV g_vec2_0

#define DeltaTime    g_float_0

// ==============
VTX_OUT VS_TileMap2(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;

    return output;
}

float4 PS_TileMap2(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    // 0~1 의 UV 를 NxM 타일 크기로 확장한다
    float2 vUV = _in.vUV * float2(TileCountX, TileCountY); // * SliceSizeUV + LeftTopUV;

    // 각 타일 위치를 정수단위로 나누기 위해서 정수값을 취한다.
    // 해당 픽셀이 위치하는 타일의 행, 렬 정보를 알아 낼 수 있다.
    int2 iTileRowCol = floor(vUV);
    
    // 얻은 타일 행렬 정보를 일차원 인덱스로 변환(타일 데이터 배열에 접근하기 위함)
    int TileDataIdx = iTileRowCol.y * TileCountX + iTileRowCol.x;
    
    
    // 타일 데이터에 접근하면, 각 타일 별로 좌상단 UV 값이 들어있다.
    // 만약 이미지 설정이 안된 타일이면 버림
    if (-1 == TileDataBuffer[TileDataIdx].iImgIdx)
    {
        discard;
    }
    else
    {
        
        float2 vLeftTopUV = StartLT + TileDataBuffer[TileDataIdx].vLTUV;
        
    // 0~1 의 UV 를 NxM 타일 크기로 확장 UV 의 소수점 부분을 샘플링 용도로 사용한다.
        float2 vImgUV = frac(vUV);
    
    // 최종 Sample 위치 UV 를 계산한다.
        float2 vSampleUI = vLeftTopUV + vImgUV * SliceSizeUV;
            
        vOutColor = g_tex_0.Sample(g_sam_1, vSampleUI);

    
    }
    
    if (vOutColor.a <= 0.0f)
        discard;
    else
        vOutColor.a -= DeltaTime * 0.25f;
    
    return vOutColor;
}





#endif