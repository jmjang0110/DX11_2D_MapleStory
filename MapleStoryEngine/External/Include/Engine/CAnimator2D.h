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
      CreateAnim 매개변수 설명
      1. 이름 2. Texture 정보 3. 자를 배경전체 영역
      4. 첫번째 프레임 좌상단 위치(pixel)
      5. 원하는 출력 간격 (가로 세로 pixel)
      5. 다음 프레임 위치 간격
      6.  프레임별 유지 시간 (1.f : 1초 )
      7. 프레임 ( 모션 사진이 총 몇개 인지 )
      8. 행 개수
      9. 열 개수
  */
    void CreateAnim(const wstring& _strName
        , Ptr<CTexture> _pAtlas, Vec2 _vBackgroundSizePixel
        , Vec2 _vLeftTopPixel, Vec2 _vSlicePixel
        , Vec2 _vStepPixel, float _fDuration, int _iFrameCount
        , int RowNum, int ColumnNum);



public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;
    CLONE(CAnimator2D)

public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _origin);
    ~CAnimator2D();
};

