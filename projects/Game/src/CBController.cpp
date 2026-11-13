#include "CBController.h"

#include <UniDx/D3DManager.h>
#include <UniDx/Input.h>

#include <algorithm>


using namespace UniDx;

namespace
{
    // 定数バッファ（型定義自体はシェーダーに伝わらないことに注意）
    struct CBParam // register(b0)
    {
        Color baseColor;
        float brightness;
        float padding[3];
    };
    constexpr int CBSlot = 0;

    /*Color colorTable[] =
    {
        Color(1, 1, 1),
        Color(1, 1, 0),
        Color(1, 0, 1),
        Color(0, 1, 1),
        Color(0.5, 0, 0),
        Color(0, 0.5, 0),
        Color(0, 0, 0.5),
    };*/

    Color colorTable[] =
    {
        Color(1, 0, 0),       // 赤
        Color(0, 1, 0),       // 緑
        Color(0, 0, 1),       // 青
        Color(1, 0.5, 0),     // オレンジ
        Color(0.5, 0, 1),     // 紫
        Color(1, 0.4, 0.7),   // ピンク
        Color(1, 1, 1),       // 白
    };

    constexpr int tableSize = sizeof(colorTable) / sizeof(Color);
}


void CBController::Start()
{
    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth = sizeof(CBParam);       // 構造体のサイズを指定
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;

    // 定数バッファを確保
    D3DManager::getInstance()->GetDevice()->CreateBuffer(&desc, nullptr, constantBuffer.GetAddressOf());
}


void CBController::Update()
{
    /*if(Input::GetKeyDown(Keyboard::Down))
    {
        colorIndex = (colorIndex + 1) % tableSize;
    }*/

    if (Input::GetKeyDown(Keyboard::Right))
    {
        colorIndex = (colorIndex + 1) % tableSize;
    }

    if (Input::GetKeyDown(Keyboard::Left))
    {
        colorIndex = (colorIndex - 1 + tableSize) % tableSize;
    }

    if (Input::GetKeyDown(Keyboard::Up))
    {
        brightness += 0.1f;
    }

    if (Input::GetKeyDown(Keyboard::Down))
    {
        brightness -= 0.1f;
    }

    brightness = std::clamp(brightness, 0.0f, 2.0f);


    CBParam cb;

    // カラーテーブルから現在のインデクスの色を設定
    cb.baseColor = colorTable[colorIndex];

    // 明るさを設定
    cb.brightness = brightness;

    // 定数バッファを更新
    D3DManager::getInstance()->GetContext()->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &cb, 0, 0);

    // シェーダーへ設定
    ID3D11Buffer* cbs[1] = { constantBuffer.Get() };
    D3DManager::getInstance()->GetContext()->VSSetConstantBuffers(CBSlot, 1, cbs);
    D3DManager::getInstance()->GetContext()->PSSetConstantBuffers(CBSlot, 1, cbs);

}