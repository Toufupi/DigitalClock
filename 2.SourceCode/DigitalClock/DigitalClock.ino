#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <wifi_link_tool.h>

#define FS_CONFIG

#define DataPin D6
#define ClockPin D0
#define Latch D5
#define OE D7

const char *NtpServer = "ntp.aliyun.com";
int TimeZone = 8;  //时区设置
int RefreshRate = 10; //in minutes

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,NtpServer,60*60*TimeZone,RefreshRate*60*1000);

int hour1 = 0;
int hour2 = 0;
int minute1 = 0;
int minute2 = 0;

byte num[10]={119,17,107,91,29,94,126,19,127,95}; //数字0到9
int brightness = 900; //亮度值范围为0-1023，1023最暗，0最亮
byte dot = 1;
long times = 0;   //用于显示刷新
long RefreshTime = 0;  //用于数值刷新
byte starts[7]= {2,1,16,64,32,4,8}; //LED闪烁一周

void gettool();
void loadingAnimation();

void setup() {
  Serial.begin(115200);
  rstb = 0;
  stateled=2;
  Hostname = "ESP";
  wxscan=true;
  load();
  webServer.on("/gettool", gettool);
  
  timeClient.begin();
  pinMode(DataPin,OUTPUT);
  pinMode(ClockPin,OUTPUT);
  pinMode(Latch,OUTPUT);
  pinMode(OE,OUTPUT);
  analogWrite(OE,brightness);
  times = millis();
  
  for(int i = 0;i<8;i++){
  digitalWrite(Latch,LOW);
  shiftOut(DataPin,ClockPin,LSBFIRST,starts[i]);
  shiftOut(DataPin,ClockPin,LSBFIRST,starts[i]);
  shiftOut(DataPin,ClockPin,LSBFIRST,starts[i]);
  shiftOut(DataPin,ClockPin,LSBFIRST,starts[i]);
  digitalWrite(Latch,HIGH);
  delay(400);
  }
  
  loadingAnimation();
}

void loop() {
  pant();
  
  if(millis() - RefreshTime>=1000*10 || RefreshTime ==0){
    timeClient.update();
    hour1 = timeClient.getHours()/10;
    hour2 = timeClient.getHours()%10;
    minute1 = timeClient.getMinutes()/10;
    minute2 = timeClient.getMinutes()%10;
    RefreshTime = millis();
  }
  
  if(millis()-times >= 1000){
    dot = !dot;
    times = millis();
    digitalWrite(Latch,LOW);
    shiftOut(DataPin,ClockPin,LSBFIRST,num[minute2]);
    shiftOut(DataPin,ClockPin,LSBFIRST,num[minute1]+dot*128);
    shiftOut(DataPin,ClockPin,LSBFIRST,num[hour2]+dot*128);
    shiftOut(DataPin,ClockPin,LSBFIRST,num[hour1]);
    digitalWrite(Latch,HIGH);
    //时间每秒刷新一次。
  }
}



void gettool() {
  if(webServer.arg("gettool") == "call") {
    //当有请求gettool参数是call激活请求返回ojbk
    Serial.println("get");
    //串口输出
    webServer.send(200, "text/plain", "ojbk");
  }
}


void loadingOne(){
  digitalWrite(Latch,LOW);
    shiftOut(DataPin,ClockPin,LSBFIRST,0);
    shiftOut(DataPin,ClockPin,LSBFIRST,8);
    shiftOut(DataPin,ClockPin,LSBFIRST,0);
    shiftOut(DataPin,ClockPin,LSBFIRST,0);
  digitalWrite(Latch,HIGH);
}
void loadingTwo(){
  digitalWrite(Latch,LOW);
    shiftOut(DataPin,ClockPin,LSBFIRST,0);
    shiftOut(DataPin,ClockPin,LSBFIRST,128);
    shiftOut(DataPin,ClockPin,LSBFIRST,0);
    shiftOut(DataPin,ClockPin,LSBFIRST,0);
  digitalWrite(Latch,HIGH);
}
void loadingThree(){
  digitalWrite(Latch,LOW);
    shiftOut(DataPin,ClockPin,LSBFIRST,0);
    shiftOut(DataPin,ClockPin,LSBFIRST,0);
    shiftOut(DataPin,ClockPin,LSBFIRST,8);
    shiftOut(DataPin,ClockPin,LSBFIRST,0);
  digitalWrite(Latch,HIGH);
}
void loadingFour(){
  digitalWrite(Latch,LOW);
    shiftOut(DataPin,ClockPin,LSBFIRST,0);
    shiftOut(DataPin,ClockPin,LSBFIRST,0);
    shiftOut(DataPin,ClockPin,LSBFIRST,128);
    shiftOut(DataPin,ClockPin,LSBFIRST,0);
  digitalWrite(Latch,HIGH);
}

void loadingAnimation(){
  loadingOne();
  delay(1000);
  loadingTwo();
  delay(1000);
  loadingThree();
  delay(1000);
  loadingFour();
  delay(1000);
}
