#pragma once
#include "CRenderComponent.h"

#include "CStructuredBuffer.h"
#include "CDamageParticleShader.h"


class CDamageParticle :
    public CRenderComponent
{

private:
    Ptr<CDamageParticleShader>      m_CS;           // ��ƼŬ ������Ʈ ���̴�

    CStructuredBuffer* m_ParticleBuffer;   // ��ƼŬ ����ȭ ����
    CStructuredBuffer* m_DataBuffer;       // �߰� ������ ����(��ƼŬ AliveCount)
    UINT                            m_iMaxCount;        // ��ƼŬ �ִ� ����

    int                             m_bPosInherit;      // ������Ʈ ��ǥ�� ����
    int                             m_iAliveCount;      // �̹� �����ӿ� ����(Ȱ��ȭ) �� ��ƼŬ ��

    float                           m_fMinLifeTime;     // ��ƼŬ�� �ּ� ���� �ð�
    float                           m_fMaxLifeTime;     // ��ƼŬ�� �ִ� ���� �ð�

    float                           m_fStartSpeed;      // ��ƼŬ�� �ּ� �ӷ�
    float                           m_fEndSpeed;        // ��ƼŬ�� �ִ� �ӷ�

    Vec4                            m_vStartColor;      // ��ƼŬ �ʱ� ����
    Vec4                            m_vEndColor;        // ��ƼŬ ���� ����

    Vec3                            m_vStartScale;      // ��ƼŬ �ʱ� ũ��
    Vec3                            m_vEndScale;        // ��ƼŬ ���� ũ��


    float                           m_fParticleCreateDistance;  // ��ƼŬ ���� ����
    float                           m_fParticleCreateTerm;      // ��ƼŬ ���� ����
    float                           m_fAccTime;

    Vec2                            m_vStartLT_UV;
    Vec2                            m_vSlice_UV;
    int                             m_DamageNum;

    float                            m_fDeltaTime;
    float                           m_fTimer;

    Vec2                            m_hitEffect_StartUV;
    Vec2                            m_hitEffect_SliceUV;

public:
    void SetDamageNum(int _damage) { m_DamageNum = _damage; }
    void SetStartLT_UV(Vec2 _LTuv) { m_vStartLT_UV = _LTuv;}
    void setSliceUV(Vec2 _sliceUV) { m_vSlice_UV = _sliceUV; }
    void SetHitEffectStartUV(Vec2 _uv) { m_hitEffect_StartUV = _uv; }
    void SetHitEffectSliceUV(Vec2 _uv) { m_hitEffect_SliceUV = _uv; }

public:
    // MaxCnt�� �ٶ� ParticleBufferũ�⸦ �����Ѵ�. 
    void SetMaxParticleCount(UINT _iMax);


public:
    virtual void finalupdate() override;
    virtual void render()   override;

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;
    CLONE(CDamageParticle);




public:
    // Get 
    UINT    GetMaxCount() { return m_iMaxCount; }
    int     GetPosInherit() { return m_bPosInherit; }

    int     GetAliveCount() { return m_iAliveCount; }
    float   GetMinLifeTime() { return m_fMinLifeTime; }
    float   GetMaxLifeTime() { return m_fMaxLifeTime; }
    float   GetStartSpeed() { return m_fStartSpeed; }
    float   GetEndSpeed() { return m_fEndSpeed; }
    Vec4    GetStartColor() { return m_vStartColor; }
    Vec4    GetEndColor() { return m_vEndColor; }
    Vec3    GetStartScale() { return m_vStartScale; }
    Vec3    GetEndScale() { return m_vEndScale; }

    float   GetParticleCreateDIstance() { return  m_fParticleCreateDistance; }
    float   GetParticleCreateTerm() { return  m_fParticleCreateTerm; }


public:
    // Set 
    void    SetMaxCount(UINT _maxCnt) { m_iMaxCount = _maxCnt; }
    void    SetPosInherit(int _posInherit) { m_bPosInherit = _posInherit; }

    void    SetAliveCount(int _AliveCnt) { m_iAliveCount = _AliveCnt; }
    void    SetMinLifeTime(float _MinLifeTime) { m_fMinLifeTime = _MinLifeTime; }
    void    SetMaxLifeTime(float _MaxLifeTime) { m_fMaxLifeTime = _MaxLifeTime; }
    void    SetStartSpeed(float _StartSpeed) { m_fStartSpeed = _StartSpeed; }
    void    SetEndSpeed(float _EndSpeed) { m_fEndSpeed = _EndSpeed; }
    void    SetStartColor(Vec4 _StartColor) { m_vStartColor = _StartColor; }
    void    SetEndColor(Vec4 _EndColor) { m_vEndColor = _EndColor; }
    void    SetStartScale(Vec3 _StartScale) { m_vStartScale = _StartScale; }
    void    SetEndScale(Vec3 _EndScale) { m_vEndScale = _EndScale; }

    void    SetParticleCreateDIstance(float _dist) { m_fParticleCreateDistance = _dist; }
    void    SetParticleCreateTerm(float _term) { m_fParticleCreateTerm = _term; }


public:
    CDamageParticle();
    CDamageParticle(const CDamageParticle& _origin);
    ~CDamageParticle();
};

