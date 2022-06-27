#pragma once
#include <Engine/CScript.h>

/*


*/
// G = M * A 
// - ������ ������ �ʹ� Ŀ�� ��ü�� ������ ������ ����ģ��, 
// -> �������� ���ӵ� 9.8 m/s 

// �Ʒ����� - ������ ���ӵ� 
// ������ ���� �Ʒ� �������� �� �ۿ� 
class CGravityScript :
    public CScript
{
private:




public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;



public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


    CLONE(CGravityScript)


public:
    CGravityScript();
    ~CGravityScript();

};

