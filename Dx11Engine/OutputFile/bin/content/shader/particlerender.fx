#ifndef _PARTICLERENDER
#define _PARTICLERENDER

#include "value.fx"
#include "struct.fx"

// ====================
// ParticleRenderShader
// Mesh     : PointMesh
// Domain   : Opaque
// Blend    : AlphaBlend
// DepthStencil : Less
// Rasterizer : CULL_NONE


// Parameter
// =======
#define PARTICLE_TYPE_NUM g_int_2
// =======
#define PARTICLE_INDEX  g_int_1
#define POS_INHERIT     g_int_0
// ====================

StructuredBuffer<tParticle> ParticleBuffer : register(t16);

struct VTX_IN
{
    uint InstID : SV_InstanceID;
};

struct VTX_OUT
{
    uint InstID : SV_InstanceID;
};


VTX_OUT VS_ParticleRender(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
   
    output.InstID = _in.InstID;
    
    return output; // VTX 반환 타입이 Geometry Shdaer 의 입력으로 들어간다. 
    // PointMesh => 입력이 하나만 들어오게 설정 
}

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint InstID : SV_InstanceID;
};

// 1 개의 정점을 6개로 분할 // 삼각형 2개 -> 사각형 1개 
[maxvertexcount(6)] // 6개의 정점을 생성하겠다.  inout : 레퍼런스 / TriangleStream : Vector 로 생각 
void GS_ParticleRender(point VTX_OUT _in[1], inout TriangleStream<GS_OUT> _output)
{
    uint ID = _in[0].InstID;
    
    // 해당 인스턴스가 비활성화면, 출력 정점이 없다 ==> 아무일도 없다
    if (0 == ParticleBuffer[ID].Alive)
        return;
    
     // 파티클 정점의 월드위치 구하기
    float3 vParticleWorldPos = (float3) 0.f;
    
    
    if (POS_INHERIT)
    { // 부모의 좌표에 영향을 받음 
        vParticleWorldPos = g_matWorld._41_42_43 + ParticleBuffer[ID].vPos;
    }
    else
    { // 부모의 좌표에 영향을 안 받음 
        vParticleWorldPos = ParticleBuffer[ID].vPos;


    }
    
    // World 좌표를 View, Proj 으로 변환
    float4 vViewPos = mul(float4(vParticleWorldPos, 1.f), g_matView);
    float3 vScale = ParticleBuffer[ID].vScale;
    
    // ViewSpace 에서 Point 를 Rect 로 분할
    // 0 --- 1
    // |  \  |
    // 3 --- 2
    float3 arrRect[4] =
    {
        float3(vViewPos.x - vScale.x / 2.f, vViewPos.y + vScale.y / 2.f, vViewPos.z),
        float3(vViewPos.x + vScale.x / 2.f, vViewPos.y + vScale.y / 2.f, vViewPos.z),
        float3(vViewPos.x + vScale.x / 2.f, vViewPos.y - vScale.y / 2.f, vViewPos.z),
        float3(vViewPos.x - vScale.x / 2.f, vViewPos.y - vScale.y / 2.f, vViewPos.z)
    };
    
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    for (int i = 0; i < 4; ++i)
    {
        // 투영좌표계 로 변환해서 대입 
        output[i].vPosition = mul(float4(arrRect[i], 1.f), g_matProj);
        output[i].InstID = _in[0].InstID;
    }
    
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
        
            
    _output.Append(output[0]);
    _output.Append(output[2]);
    _output.Append(output[3]);
    _output.RestartStrip(); // 하나의 면임을 알림  
    
    _output.Append(output[2]);
    _output.Append(output[0]);
    _output.Append(output[1]);
    _output.RestartStrip();
}


float4 PS_ParticleRender(GS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    if (g_btex_0 && PARTICLE_TYPE_NUM == 0)
    {
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor;
   
    }
    else if (g_btex_1 && PARTICLE_TYPE_NUM == 1)
    {
       
            vOutColor = g_tex_1.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor;
       
       
    }
    else if (g_btex_2 && PARTICLE_TYPE_NUM == 2)
    {
        if (0 <= _in.InstID && _in.InstID <= 300)
            vOutColor = g_tex_2.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor;
        else
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor;
    }
    else if( PARTICLE_TYPE_NUM == 3)
    {
         vOutColor = g_tex_3.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor; // Light
        //if (_in.InstID == 0)
        //    vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor; // Thunderbolt
        //else if (_in.InstID == 1)
        //    vOutColor = g_tex_1.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor; // Bolt
        //else if(_in.InstID == 2)
        //    vOutColor = g_tex_2.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor; // Cloud
            
        //else if (30 <= _in.InstID && _in.InstID < 40)
        //    vOutColor = g_tex_2.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor;
        //else if (40 <= _in.InstID && _in.InstID < 50)
            //vOutColor = g_tex_3.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor; // Light
        
    }
    else if (PARTICLE_TYPE_NUM == 4)
    {
        vOutColor = g_tex_1.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor; // Bolt
    }
    else if(PARTICLE_TYPE_NUM == 5)
    {
        vOutColor = g_tex_2.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor; // Bolt
    
        }
    else if(PARTICLE_TYPE_NUM == 6)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor; // Bolt
        
    }
    else
    {
        vOutColor = ParticleBuffer[_in.InstID].vColor;
    }
    
    return vOutColor;
}

#endif