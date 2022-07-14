#pragma once
#include <Engine/CScript.h>

struct tMonsterInfo
{
    float      fSpeed;           // �ӵ�
    float      fHP;              // ü�� 

    float      fRecogRange;      // �ν� ���� 
    float      fAttRange;        // ���� ����
    float      fAtt;             // ���ݷ�
    float      fDef;             // ���� 


};

class CMonsterScript :
    public CScript
{
private:
    class CAIScript*    m_pAI;
    tMonsterInfo        m_tInfo;


public:
    float   GetSpeed() { return m_tInfo.fSpeed; }
    void    SetSpeed(float _f) { m_tInfo.fSpeed = _f; }
    void    SetAIScript(class CAIScript* _pAI) { m_pAI = _pAI; }
    tMonsterInfo    GetMonsterInfo() { return m_tInfo; }

private:
    void    SetMonsterInfo(const tMonsterInfo& _info) { m_tInfo = _info; }


public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;




public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


    CLONE(CMonsterScript)


public:
    CMonsterScript();
    CMonsterScript(const CMonsterScript& _origin);
    ~CMonsterScript();



};

