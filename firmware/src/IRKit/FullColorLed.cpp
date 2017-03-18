/*
 Copyright (C) 2013-2014 Masakazu Ohtsuka
  
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.
  
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
  
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <Arduino.h>
#include "FullColorLed.h"
#include "timer.h"

FullColorLed::FullColorLed(int pinR, int pinG, int pinB) :
    pinR_(pinR),
    pinG_(pinG),
    pinB_(pinB),
    blinkOn_(0),
    isBlinking_(false),
    blink_timer_(TIMER_OFF),    
    sleep_timeout_(TIMER_OFF),  // 追加；スリープ時間の初期値
    sleep_timer_(TIMER_OFF)     // 追加；スリープタイマーの初期値
{
}

// 追加：スリープ時間設定メソッド
void FullColorLed::setSleep(uint8_t sleep_timeout) {
    sleep_timeout_ = sleep_timeout;
}

void FullColorLed::setLedColor(bool colorR, bool colorG, bool colorB) {
    setLedColor(colorR, colorG, colorB, false);
}

void FullColorLed::setLedColor(bool colorR, bool colorG, bool colorB, bool blink) {
    colorR_      = colorR;
    colorG_      = colorG;
    colorB_      = colorB;
    isBlinking_  = blink;

    blink_timer_ = TIMER_OFF;

    // 追加：開始
    if (sleep_timeout_ != TIMER_OFF) {            // スリープ時間がTIMER_OFF(=無効)でなかったら
      TIMER_START(sleep_timer_, sleep_timeout_);  // スリープタイマーを起動
    }
    // 追加：終了  
}

void FullColorLed::setLedColor(bool colorR, bool colorG, bool colorB, bool blink, uint8_t blink_timeout) {
    setLedColor( colorR, colorG, colorB, blink );

    TIMER_START(blink_timer_, blink_timeout);
}

void FullColorLed::off() {
    setLedColor( 0, 0, 0, false );
    TIMER_STOP(sleep_timer_);       // 追加：スリープタイマーを停止   
}

void FullColorLed::onTimer() {
    blinkOn_ = ! blinkOn_;

    if ( blinkOn_ || ! isBlinking_ ) {
        // not blinking = always on
        digitalWrite(pinR_, colorR_);
        digitalWrite(pinG_, colorG_);
        digitalWrite(pinB_, colorB_);
    }
    else {
        digitalWrite(pinR_, LOW);
        digitalWrite(pinG_, LOW);
        digitalWrite(pinB_, LOW);
    }

    TIMER_TICK(blink_timer_);
    if (TIMER_FIRED(blink_timer_)) {
        TIMER_STOP(blink_timer_);
        isBlinking_ = false;
    }
    // 追加：開始
    TIMER_TICK(sleep_timer_);         // スリープタイマーを進める
    if (TIMER_FIRED(sleep_timer_)) {  // スリープタイマーが発火したら
      off();                          // LEDを消灯する
    }
    // 追加：終了
    
}
