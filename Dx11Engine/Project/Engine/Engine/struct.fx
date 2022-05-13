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
    float3      vPos;           // 12
    float3      vScale;         // 12
    float3      vDir;           // 12
    float4      vColor;         // 16

    // ===========

    float       fInclination;   // ���� 
    float3      vTop;           // ������ 
    int         TextureTypeNum;
    
    // ===========
    int         Alive;          // 4
    float       m_fCurTime;     // 4
    float       m_fMaxTime;     // 4
};

struct tParticleData
{
    int iAliveCount;
    float3 vPadding;
    
};




#endif