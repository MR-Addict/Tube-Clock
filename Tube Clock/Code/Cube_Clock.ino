#include <stdio.h>
#include <DS1302.h>

const uint8_t Tube1 = 10;
const uint8_t Tube2 = 11;
const uint8_t Tube3 = 12;
const uint8_t Tube4 = 13;
const uint8_t LED = A0;
const uint8_t RST = 4;
const uint8_t DAT = 3;
const uint8_t CLK = 2;

DS1302 rtc(RST, DAT, CLK);

uint8_t LedChar[] = {  //数码管显示字符转换表
  0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90
};

uint8_t LedBuff[4] = {  //数码管显示缓冲区，初值0xFF确保启动时都不亮
  0xFF, 0xFF, 0xFF, 0xFF
};

const int LedPins[] = {
  5, 6, 7, 8, 9, A1, A2, Tube1, Tube2, Tube3, Tube4, LED
};

void setup() {
  for (int i = 0; i < 12; i++) {
    pinMode(LedPins[i], OUTPUT);
  }
}

void loop() {
  static uint8_t i = 0;   //动态扫描的索引
  static uint8_t count = 0;
  static uint8_t Preserved_Min = 0;
  static boolean flag = 0;

  switch (i)
  {
    case 0: DigitalWrite(1, 0, 0, 0); i++; Display(LedBuff[0]); break;
    case 1: DigitalWrite(0, 1, 0, 0); i++; Display(LedBuff[1]); break;
    case 2: DigitalWrite(0, 0, 1, 0); i++; Display(LedBuff[2]); break;
    case 3: DigitalWrite(0, 0, 0, 1); i = 0; Display(LedBuff[3]); break;
    default: break;
  }

  if (count++ == 200) {
    count = 0;
    flag = !flag;
    if (flag)digitalWrite(LED, HIGH);
    else digitalWrite(LED, LOW);
    
    Time time = rtc.time();
    if (Preserved_Min != time.min) {
      Preserved_Min = time.min;
      Refresh_Time(time);
    }
  }
  delay(5);
}

void DigitalWrite(boolean value1, boolean value2, boolean value3, boolean value4) {
  digitalWrite(Tube1, value1);
  digitalWrite(Tube2, value2);
  digitalWrite(Tube3, value3);
  digitalWrite(Tube4, value4);
}

void Display(uint8_t value) {
  for (int i = 0; i < 7; i++)
    digitalWrite(LedPins[i], bitRead(value, i));
}

void Refresh_Time(Time time) {
  LedBuff[0] = LedChar[time.min % 10];
  LedBuff[1] = LedChar[(time.min % 100) / 10];
  LedBuff[2] = LedChar[time.hr % 10];
  LedBuff[3] = LedChar[(time.hr % 100) / 10];
}
