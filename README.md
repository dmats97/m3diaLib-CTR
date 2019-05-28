# m3diaLib [![Build Status](https://travis-ci.org/m3diaLib-Team/m3diaLib-CTR.svg?branch=master)](https://travis-ci.org/StuntHacks/m3diaLib) [![CodeFactor](https://www.codefactor.io/repository/github/m3dialib-team/m3dialib-ctr/badge)](https://www.codefactor.io/repository/github/m3dialib-team/m3dialib-ctr)
A fast, extensive and powerful allround media and game library for the Nintendo 3DS, written in C++.

#### Click [here](#installation) to get to the installation instructions.

### スプライトアニメーション
```cpp
#include <m3dia.hpp>

int main() {
    m3d::Applet app;
    m3d::Screen scr;
    m3d::Sprite spr;
    
    //int型配列にスプライトシートのインデックスを列挙します。末尾は-1である必要が有ります。
    int Table[] = {0, 1, 2, 3, -1};
    
    //メンバ関数Animateを呼び出します
    //列挙した配列とコマ切り替えのインターバル、ループするかどうかのフラグを渡します
    spr.Animate(Table, 250, true);

    while (app.isRunning()) {
        if (m3d::buttons::buttonPressed(m3d::buttons::Button::Start)) {
            app.exit(); // exit if start is pressed
        }
        
        //アニメーションを適用します
        spr.updateSprites();

        scr.drawTop();
        scr.render();
    }
}
```
### その他
インストール方法などについては [m3diaLib-CTR](https://github.com/m3diaLib-Team/m3diaLib-CTR)を参照。
