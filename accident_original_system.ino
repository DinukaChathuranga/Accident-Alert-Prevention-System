  #include <TinyGPS++.h>
 #include <Wire.h>
#include<DFRobot_SIM808.h>
#include <sim808.h>
#include <hd44780.h>                       
#include <hd44780ioClass/hd44780_I2Cexp.h>

const int vs = 9;
const int buzzer = 11;
const int led = 13;
const int LCD_COLS = 16;
const int LCD_ROWS = 2;
hd44780_I2Cexp lcd;

#include <SoftwareSerial.h>


int state = 0;
const int p1 = 12;
const int p2 = 10;
float gpslat, gpslon;
char message[400];

TinyGPSPlus gps;
SoftwareSerial sgps(4, 5);
SoftwareSerial sgsm(7, 8);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(vs, INPUT);
  pinMode(led, OUTPUT);
  pinMode(p1, INPUT);
  pinMode(p2, INPUT);
  lcd.begin(LCD_COLS, LCD_ROWS);
 
  sgsm.begin(9600);
  sgps.begin(9600);
   int flame = analogRead(A0);  
   delay(2000);

   
 
}
void loop() {
  // put your main code here, to run repeatedly:
  int val = digitalRead(vs);
  int val1 = analogRead(A2);
  int val2 = analogRead(A3);
  int button = digitalRead(p1 );
  int flame = analogRead(A0);
  long measurement =TP_init();
  alcoholsensor();

 Serial.println(val1);
 delay(500);
 Serial.println(val2);
  delay(50);
   Serial.println(val2);
 
  if  (((measurement >= 50 && measurement<=12000)&&(val2 >= 1 || val1 >= 1))||(flame < 60 ))
  {
    while (button == LOW)
    {
      for (int n = 0; n < 30; n++)
      {
        int button = digitalRead(p1);
        
        if (button == HIGH)
        {
          break;
        }
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("ACCIDENT");
        delay(200);
        lcd.setCursor(2,1);
        lcd.print("DETECTED !!!");
        delay(100);
        tone(buzzer, 450);
        digitalWrite(led, HIGH);
        delay(500);
        noTone(buzzer);
        digitalWrite(led, LOW);
        delay(500);
        
      }

      int button = digitalRead(p1);
      if (button == HIGH)
      {
        break;
      }
       sgps.listen();
   
   while(1)
  {
   while (sgps.available() > 0)
   { gps.encode(sgps.read()); }

      if (gps.location.isUpdated())
      {
       gpslat=gps.location.lat();
       gpslon=gps.location.lng();
      break;
      }}

      sgsm.listen();
      sgsm.print("\r");
      delay(1000);
      sgsm.print("AT+CMGF=1\r");
      delay(1000);
      /*Replace XXXXXXXXXX to 10 digit mobile number &
        ZZ to 2 digit country code*/
      sgsm.print("AT+CMGS=\"+94770162082\"\r");
      delay(1000);
      locationprint();   

      sprintf(message,"!!!...An Accident was Reported...!!!\n *** Vehicle No-JY681 has been involed in an Accident.***\n\nTake action as soon as Possible \n You can Locate Accident Folllowing this two links\n");
      sgsm.println(message);
      sgsm.print("Latitude :");
      sgsm.println(gpslat, 6);
      sgsm.print("Longitude:");
      sgsm.println(gpslon, 6);
      sgsm.println("http://www.latlong.net/Show-Latitude-Longitude.html");
      sgsm.print("https://www.google.com/maps/?q=");
      sgsm.print(gpslat, 6);
      sgsm.print(",");
      sgsm.print(gpslon, 6);
      delay(1000);
      sgsm.write(0x1A);
      delay(1000);
      state=1;
    } 
  }
  else
  {
    noTone(buzzer);
    digitalWrite(led, LOW);
    delay(500);
  }
}
long TP_init(){
  delay(10);
  long measurement=pulseIn (vs, HIGH);  //wait for the pin to get HIGH and returns measurement
  return measurement;
}
void alcoholsensor()
{
  lcd.clear();
  int Sober = 120;
  int Drunk = 400;
  int sensorValue = analogRead(A1);
   lcd.setCursor(0,0);
  lcd.print("Sensor Value: ");
  lcd.setCursor(8,1);
  lcd.print(sensorValue);
  delay(2000);
  
  if (sensorValue < Sober) {
    lcd.clear();
    delay(300);
    lcd.setCursor(0,0);
    lcd.print("Status: Sober");
  } else if (sensorValue >= Sober && sensorValue < Drunk) {
    lcd.clear();
    delay(300);
    lcd.setCursor(0,0);
    lcd.print("Status: within legal");
    lcd.setCursor(3,1);
    lcd.print("Limits");
    
  } else {
    lcd.clear();
    delay(300);
    lcd.setCursor(0,0);
    lcd.print("Status: DRUNK"); 
  }
  }

   void locationprint()
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Latitude:");
      lcd.setCursor(9,0);
      lcd.print(gpslat, 6);
      lcd.setCursor(0,1);
      lcd.print("Longitude:");
      lcd.setCursor(10,1);
      lcd.print(gpslon, 6);
      }
