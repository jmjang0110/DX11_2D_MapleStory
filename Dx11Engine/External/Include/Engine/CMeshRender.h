#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"
#include "CRenderComponent.h"
#include "CMesh.h"
#include "CMaterial.h"


class CMeshRender :
    public CRenderComponent
{
private:


public:
    virtual void finalupdate() override;
    virtual void render() override;

    CLONE(CMeshRender)


public:
    // �̹��� �¿� ���� ���� 
    void SetUVReverse(bool _reverse);
    // �̹��� ���� ���� ���� 
    void SetUVUpsideDown(bool _upsideDown);

public:
    CMeshRender();
    ~CMeshRender();
};

