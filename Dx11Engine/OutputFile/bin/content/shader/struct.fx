#ifndef _STRUCT
#define _STRUCT

#include "value.fx"

// ����ȭ ���� : Texture �ȼ� ��ĭ�� Ŀ���� �ϴ� ���� 

struct tTileData
{

    int         iImgIdx;
    float2      vLTUV;
    int         iPadding;

};


struct tParticle
{
    float3      vPos;
    float3      vScale;
    float3      vDir;
    float4      vColor;

    int         Alive;
    float2      Padding;
};



#endif