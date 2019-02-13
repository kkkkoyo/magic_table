#include <Arduino.h>
// PIN: ソレノイド
const int solenoid[6] = {7,12,11,10,9,8};
// PIN: 人感センサ
const int PEOPELE_OUT = 6;
// PIN: 人感センサ
const int touchPin[2] = {5,4};
// ソレノイドの合計数
const int solenoidNum = 6;
bool isArduinoTimer = true;
bool isDebug = false;
int solenoid_OnTime = 7;
String serialStr;
void setup()
{
 Serial.begin(9600);
 // 初期化：ソレノイド
 for (int i = 0; i < solenoidNum; i ++) {
  pinMode(solenoid[i],OUTPUT);
 }
 //5番ピンをデジタル出力
 pinMode(touchPin[0],OUTPUT);
 //4番ピンをデジタル入力
 pinMode(touchPin[1],INPUT);
}
void loop()
{ 
// シリアルStrをリセット
serialStr = "";
// 人感センサで人を検出する
inputPeople();
// // タッチセンサで検出
inputTouch();
outputSerial();
 // デバック時、Aruduino単体でソレノイドを制御する
 if(isDebug)
 {
  solenoidON();
 }
// 
// 
// シリアル受信；ソレノイドのオンの確認
Serial_SolenoidOn(); 
// 
 // シリアル受信；ソレノイドのセッティング時間の確認
 Serial_SettingOnTime;
// 
 // ArduinoTimerでなく、シリアル通信でオフにする場合
 if(!isArduinoTimer)
 {
  Serial_SolenoidOff();
 }
}
// シリアル通信を送信する
/*
 * 1：人感センサの値 (1:検出, 0: 非検出)
 * 2：タッチセンサの値 (n: n > 0)
 */
void outputSerial()
{
 Serial.println(serialStr);
}
// 人感センサで人を検出する
void inputPeople()
{
 if (digitalRead(PEOPELE_OUT) == HIGH) {
   serialStr = "1\t";
 }
 else{
   serialStr = "0\t";
 }
 
}
// タッチセンサで検出
void inputTouch()
{
 //静電容量変化量の変数を用意
 int touchNum = 0;
 //touchPin[0]番ピンをHIGHで出力
 digitalWrite(touchPin[0], HIGH);
 //指が触れたときtouchPin[1]番ピンがHIGHになるまでをカウント
 
 while (digitalRead(touchPin[1])!=HIGH){
  //カウントする
  touchNum++;
  if(touchNum > 1500)
  {
    break;
  }
 }
 //8番ピンをLOWにする
 digitalWrite(touchPin[1], LOW);
 
 //モニタリング：値を出力
 serialStr = serialStr + touchNum;
}
// ソレノイドをONにする
void solenoidON()
{
  // ソレノイドをONにする
  for (int i = 0; i < solenoidNum; i ++) 
  {
   digitalWrite(solenoid[i],HIGH);
  }
  
  if(isArduinoTimer)
  {
   delay(1000 * solenoid_OnTime);
   solenoidOFF();
  }
}
// ソレノイドをオフにする
void solenoidOFF()
{
 // ソレノイドをオフにする
 for (int i = 0; i < solenoidNum; i ++) 
 {
  digitalWrite(solenoid[i],LOW);
 }
 if(isDebug)
 {
  delay(5000);
 }
}
// シリアル受信でソレノイドをオンにする
void Serial_SolenoidOn()
{
 if(Serial.available())
 {
   char input;
  
    // シリアルポートより1文字読み込む
   input = Serial.read(); 

   if(input == 'a')
   {
    solenoidON();
   }
 }else{
     // Serial.println(0);
  }
}
// シリアル受信でソレノイドをオフにする
void Serial_SolenoidOff()
{
 if(Serial.available())
 {
   char input;
  
    // シリアルポートより1文字読み込む
   input = Serial.read(); 
   if(input == 'b')
   {
    solenoidOFF();
   }
 }
}
// シリアル受信でソレノイドの時間を設定する
void Serial_SettingOnTime()
{
 if(Serial.available())
 {  
    // シリアルポートより読み込む
   byte input = Serial.read();
   input = input - 0x30;
   for(int i = 1; i <= 1000; i ++)
   {
    if(input = i)
    {
     // ソレノイドがONの時間を設定する
     SettingSolenoidOnTime(i);
    }
   }
 }
}
// ソレノイドがオンの時間を設定する
void SettingSolenoidOnTime(int time)
{
 solenoid_OnTime = time;
}
