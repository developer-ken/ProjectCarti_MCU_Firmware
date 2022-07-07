#include <Arduino.h>
#include <TMCStepper.h>
#include <Adafruit_NeoPixel.h>
#include "BaseMovement.h"
#include "SerialCommunication.h"

// 电机和机架参数修改请看BaseMovement.h文件

#define ARGBLED 17
#define BEEPPIN 12

double x, vx, y, vy, r, vr;
double bx, by, br;
unsigned long lasttime;

Adafruit_NeoPixel led(1, ARGBLED, NEO_GRB + NEO_KHZ800);

void setup()
{
  pinMode(BEEPPIN, OUTPUT);
  Serial2.begin(115200); // Init serial port and set baudrate
  while (!Serial2)
    ; // Wait for serial port to initialize
  led.begin();
  BMinitialize();
  led.clear();
  led.setPixelColor(0, led.Color(25, 10, 0));
  led.show();
  beep(50);
  SendPack("SB", "", "", "", "");
}

void beep(short len)
{
  digitalWrite(BEEPPIN, HIGH);
  delay(len);
  digitalWrite(BEEPPIN, LOW);
}

void loop()
{
  if (Serial2.available() > 0)
  {
    Pack data = ReadPack();
    if (data.command.length() > 0)
      if (data.command.equals("ST")) //里程计
      {
        SendPack("CP", "ST", String(x, 4) + "," + String(vx, 4), String(y, 4) + "," + String(vy, 4), String(r, 4) + "," + String(vr, 4));
      }
      else if (data.command.equals("BK")) //刹车
      {
        Break();
        vx = 0;
        vy = 0;
        vr = 0;
        SendPack("CP", "BK", "", "", "");
      }
      else if (data.command.equals("MV")) //以设定速度持续移动
      {
        MoveAtVelocity(data.arg1.toDouble(), data.arg2.toDouble(), data.arg3.toDouble());
        vx = data.arg1.toDouble();
        vy = data.arg2.toDouble();
        vr = data.arg3.toDouble();
        lasttime = millis();
        SendPack("CP", "MV", "", "", "");
      }
      else if (data.command.equals("MD")) //移动指定距离
      {
        Move(data.arg1.toDouble(), data.arg2.toDouble(), data.arg3.toDouble(), data.arg4.toDouble());
        vx = 0;
        vy = 0;
        vr = 0;
        x += data.arg1.toDouble();
        y += data.arg2.toDouble();
        r += data.arg3.toDouble();
        SendPack("CP", "MD", "", "", "");
      }
      else if (data.command.equals("MT")) //移动指定时长
      {
        MoveAtVelocityUntil(data.arg1.toDouble(), data.arg2.toDouble(), data.arg3.toDouble(), data.arg4.toDouble());
        vx = 0;
        vy = 0;
        vr = 0;
        double l = data.arg4.toDouble();
        x += data.arg1.toDouble() * l;
        y += data.arg2.toDouble() * l;
        r += data.arg3.toDouble() * l;
        SendPack("CP", "MT", "", "", "");
      }
      else if (data.command.equals("BB")) //蜂鸣器响
      {
        beep(data.arg1.toInt());
        SendPack("CP", "BB", "", "", "");
      }
      else if (data.command.equals("LT")) // RGB灯泡
      {
        led.clear();
        led.setPixelColor(0, led.Color(data.arg1.toDouble(), data.arg2.toDouble(), data.arg3.toDouble()));
        led.show();
        SendPack("CP", "LT", "", "", "");
      }
      else if (data.command.equals("CL")) //清空里程计
      {
        x = 0;
        y = 0;
        r = 0;
        SendPack("CP", "CL", "", "", "");
      }
      else if (data.command.equals("MF")) //刹停并关闭电机
      {
        Break();
        vx = 0;
        vy = 0;
        vr = 0;
        driverDisable();
        SendPack("CP", "MF", "", "", "");
      }
      else if (data.command.equals("MU")) //开启电机并保证刹停状态
      {
        driverEnable();
        SendPack("CP", "MU", "", "", "");
      }
      else if (data.command.equals("QA")) //询问底盘移动能力
      {
        SendPack("CP", "QA", "X", "Y", "R"); //能够沿X、Y轴移动，同时支持R旋转
      }
      else
      {
        SendPack("ER", data.command, "", "", ""); //无效指令
      }
  }
  double dtime = (0.1 + (millis() - lasttime)) / 1000.0;
  x += vx * dtime;
  y += vy * dtime;
  r += vr * dtime;
  lasttime = millis();
}