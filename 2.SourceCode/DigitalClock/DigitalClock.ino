#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define DataPin D6
#define ClockPin D0
#define Latch D5
#define OE D7

const char *ssid     = "GOCLOUD";
const char *password = "1234567890";
const char *NtpServer = "ntp.aliyun.com";
int TimeZone = 8;
int RefreshRate = 10; //in minutes

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,NtpServer,60*60*TimeZone,RefreshRate*60*1000);

int hour1 = 0;
int hour2 = 0;
int minute1 = 0;
int minute2 = 0;

byte num[10]={119,17,107,91,29,94,126,19,127,95}; //数字0到9
int brightness = 800; //亮度值范围为0-1023，1023最暗，0最亮
byte dot = 1;
long times = 0;
long RefreshTime = 0;
byte starts[7]= {2,1,16,64,32,4,8};

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();

  pinMode(DataPin,OUTPUT);
  pinMode(ClockPin,OUTPUT);
  pinMode(Latch,OUTPUT);
  pinMode(OE,OUTPUT);
  analogWrite(OE,brightness);
  times = millis();
  RefreshTime = millis();
  for(int i = 0;i<8;i++){
  digitalWrite(Latch,LOW);
  shiftOut(DataPin,ClockPin,LSBFIRST,starts[i]);
  shiftOut(DataPin,ClockPin,LSBFIRST,starts[i]);
  shiftOut(DataPin,ClockPin,LSBFIRST,starts[i]);
  shiftOut(DataPin,ClockPin,LSBFIRST,starts[i]);
  digitalWrite(Latch,HIGH);
  delay(400);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
/*
  for(int i=0;i<10;i++){
  digitalWrite(Latch,LOW);
  shiftOut(DataPin,ClockPin,LSBFIRST,num[i]);
  digitalWrite(Latch,HIGH);
  delay(1000);
  }
*/
  timeClient.update();
  
  if(RefreshTime - millis()>=30000){
    hour1 = timeClient.getHours()/10;
    hour2 = timeClient.getHours()%10;
    minute1 = timeClient.getMinutes()/10;
    minute2 = timeClient.getMinutes()%10;
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
