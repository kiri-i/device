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
#ifndef __FULLCOLORLED_H__
#define __FULLCOLORLED_H__

class FullColorLed
{
public:
    FullColorLed(int pinR, int pinG, int pinB);
    void setLedColor(bool colorR, bool colorG, bool colorB);
    void setLedColor(bool colorR, bool colorG, bool colorB, bool blink);
    void setLedColor(bool colorR, bool colorG, bool colorB, bool blink, uint8_t blink_timeout);
    void off();
    
    void setSleep(uint8_t sleep_timeout); // 追加：スリープ時間設定メソッド
    
    void onTimer();

private:
    int pinR_;
    int pinG_;
    int pinB_;
    bool colorR_;
    bool colorG_;
    bool colorB_;
    bool isBlinking_; // defaults to off
    volatile bool blinkOn_; // altered inside timer ISR
    volatile uint8_t blink_timer_;
    
    volatile uint8_t sleep_timeout_;  // 追加：スリープ時間格納変数
    volatile uint8_t sleep_timer_;    // 追加：スリープタイマー用変数
};

#endif // __FULLCOLORLED_H__
