#pragma once
#include "CComponent.h"

#include "CTexture.h"

class CAnimation2D;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnimation2D*> m_mapAnim;
    CAnimation2D*               m_pCurAnim;
    bool                        m_bRepeat;



public:
    virtual void finalupdate() override;
    virtual void UpdateData() override;
    static void Clear();

public:
    CAnimation2D* FindAnim(const wstring& _strName);
    void CreateAnim(const wstring& _strName, Ptr<CTexture> _pAtlas, Vec2 _vBackgroundSizePixel, Vec2 _vLeftTopPixel, Vec2 _vSlicePixel, Vec2 _vStepPixel, float _fDuration, int _iFrameCount);
    void Play(const wstring& _strName, bool _bRepeat);

     // ==========
    void CreateAnim(const wstring& _strName, CAnimation2D* _NewAnim);
    void DeleteAnim(const wstring& _strName);

    //  ==========
public:
    CAnimation2D* GetCurAnim() { return m_pCurAnim; }
    void UpdateData_DummyAnim();

    const map<wstring, CAnimation2D*> GetAnimList() { return m_mapAnim; }



    // ================= 
public:
    /*
      CreateAnim �Ű����� ����
      1. �̸� 2. Texture ���� 3. �ڸ� �����ü ����
      4. ù��° ������ �»�� ��ġ(pixel)
      5. ���ϴ� ��� ���� (���� ���� pixel)
      5. ���� ������ ��ġ ����
      6.  �����Ӻ� ���� �ð� (1.f : 1�� )
      7. ������ ( ��� ������ �� � ���� )
      8. �� ����
      9. �� ����
  */
    void CreateAnim(const wstring& _strName
        , Ptr<CTexture> _pAtlas, Vec2 _vBackgroundSizePixel
        , Vec2 _vLeftTopPixel, Vec2 _vSlicePixel
        , Vec2 _vStepPixel, float _fDuration, int _iFrameCount
        , int RowNum, int ColumnNum);



public:
    CLONE(CAnimator2D)

public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _origin);
    ~CAnimator2D();
};

