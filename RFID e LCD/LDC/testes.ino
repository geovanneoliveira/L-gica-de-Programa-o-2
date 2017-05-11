// LCD ____________________________________________________________//
//_____ GND PIN GND -----------------------------------------------//
//_____ VCC PIN 5V  -----------------------------------------------//
//_____ SDA PIN A4  -----------------------------------------------//
//_____ SCL PIN A5  -----------------------------------------------//

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int ledPin = 13; 
const int inputPin = 2; 
int presenca = 0;

void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0,0);
  
  pinMode(ledPin, OUTPUT); 
  pinMode(inputPin, INPUT); 
  pinMode(A0,INPUT);
  Serial.begin(9600);
  
}

void loop(){
  float potencia = analogRead(A0);
  float percentual = (potencia *100.0 / 1023.0);
  presenca = digitalRead(inputPin);
  lcd.print("   UTIIZACAO");
  lcd.setCursor(5,1);
  lcd.print(percentual); 
  lcd.print("% ");
  lcd.setCursor(0,0);
  Serial.println(potencia);
  if (presenca == LOW) { 
    Serial.println("Movimento nao detectado!");
    //digitalWrite(ledPin, LOW);
  }
  else{
    Serial.println("Movimento detectado!");
    //digitalWrite(ledPin, HIGH);
  }
  delay(550);
}
