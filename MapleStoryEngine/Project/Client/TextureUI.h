#pragma once
#include "ResInfoUI.h"

class TextureUI :
    public ResInfoUI
{
private:

public:
    virtual void update() override;
    virtual void render_update() override;

    // ================== Todo
public:
    void TextureSelect_toMtrl(DWORD_PTR _param);

public:
    TextureUI();
    ~TextureUI();
};

