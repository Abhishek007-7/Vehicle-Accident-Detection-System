#include <LiquidCrystal.h>
#include <TinyGPS.h>
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

const int relay_Pin = 2;
const int buzzer_Pin = 3;
const int ir_Sensor = 10;
const int alcohol_Sensor = 11;
const int vibration_Sensor = 12;
TinyGPS gps;
long lat,lon;
bool ir_status = LOW;
bool alcohol_Status = LOW;
bool vibration_Status = LOW;

void setup() {
  pinMode(relay_Pin, OUTPUT);
  pinMode(buzzer_Pin, OUTPUT);
  pinMode(ir_Sensor, INPUT);
  pinMode(alcohol_Sensor, INPUT);
  pinMode(vibration_Sensor, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("ACCIDENT DETECTION");
  lcd.setCursor(3,2);
  lcd.print("SYSTEM");

}
void loop() {
  ir_status = digitalRead(ir_Sensor);
  delay(10);
  if(ir_status == HIGH)
  {
    digitalWrite(buzzer_Pin, LOW);   
    delay(10);
lcd.clear();
    lcd.print("Seat Belt");
    lcd.setCursor(3,2);
    lcd.print("Detected");
    delay(10);  
  
    while(1)
    {
      alcohol_Status = digitalRead(alcohol_Sensor);
      delay(10);  
      if(alcohol_Status == LOW)
      {
       digitalWrite(buzzer_Pin, LOW);   
       delay(10);
       lcd.clear();
       lcd.print("Alcohol not");
       lcd.setCursor(3,2);
       lcd.print("Detected");
       delay(10);  
       digitalWrite(relay_Pin, HIGH);   
       delay(10);  
       while(1)  
       {
        lcd.clear();
        lcd.print("Vehicle Started");
        delay(10);     
        while(1)
        {   
          vibration_Status = digitalRead(vibration_Sensor);
          delay(10);   
          if(vibration_Status == HIGH)
          {
           lcd.clear();
           lcd.print("Accident Detected");
           lcd.setCursor(3,2);
           lcd.print("Sending Msg");
           delay(10);    
           Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
           delay(10);  // Delay of 1000 milli seconds or 1 second
           Serial.println("AT+CMGS=\"+916366252979\"\r"); // Replace x with mobile number
           Serial.println("AT+CMGS=\"108\"\r");
           Serial.println("AT+CMGS=\"100\"\r");
           Serial.println("AT+CMGS=\"112\"\r");
           delay(10);
           Serial.println("Accident Detected ");// The SMS text you want to send
           Serial.println("please check location");// The SMS text you want to send
           while(1)
           {
            gps_read();
           }
          }
          else
          {
           /* Do nothing */
          }
        }                 
       }
      }
      else
      {
        lcd.clear();
        lcd.print("Alcohol ");
        lcd.setCursor(3,2);
        lcd.print("Detected");
        delay(10);  
        digitalWrite(relay_Pin, LOW);   
        delay(10);              
        digitalWrite(buzzer_Pin, HIGH);   
        delay(10);        
      }
    }
  } 
  else
      {
    lcd.clear();
    lcd.print("Seat Belt");
    lcd.setCursor(3,2);
    lcd.print("not Detected");
    digitalWrite(relay_Pin, LOW);   
    delay(10);              
    digitalWrite(buzzer_Pin, HIGH);   
    delay(10);          
  }
}


void gps_read()
{ 
 byte a;
  
  if(Serial.available())  
  {
    a=Serial.read();
   
   //Serial.write(a);
   
    while(gps.encode(a))   // encode gps data 
    { 
      gps.get_position(&lat,&lon); // get latitude and longitude
    
      Serial.println("Position: ");
      Serial.print("lat:");
      Serial.println((lat*0.000001),8);
      Serial.print("log:");
      Serial.println((lon*0.000001),8);
}
}
}
