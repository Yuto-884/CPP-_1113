#pragma once

#include <UniDx.h>
#include <UniDx/Rigidbody.h>


class CBController : public UniDx::Behaviour
{
    float brightness = 1.0f;

public:
    virtual void Start() override;
    virtual void Update() override;

private:
    UniDx::ComPtr<ID3D11Buffer> constantBuffer;    // シェーダー定数バッファ
    int colorIndex = 0;
};


