// LCD 16x2 com módulo I2C
 
// Biblioteca do módulo I2C
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
// Inicializa o display no endereço 0x27
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
 
void setup() {
 // Inicializa o display LCD 16x2
 lcd.begin (16, 2);
 // Liga a luz de fundo do LCD
 lcd.setBacklight(HIGH);
}
 
void loop() {
 lcd.clear();
 lcd.setCursor(2, 0);
 lcd.print("PRESENT YOUR");
 lcd.setCursor(6,1);
 lcd.print("CARD");

 delay(10000);
 
}
