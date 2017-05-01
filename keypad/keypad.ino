#include <Keypad.h>
#include <string.h>

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = {12, 11, 10, 9};
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 8, 7, 6, 5}; 

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define ledpin 4
#define ledGreen 2
#define ledRed 3

  int i,tamanho = 3;
  char password[] = "123";
  char key = NULL;

void setup()
{
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledpin,OUTPUT);
  digitalWrite(ledGreen,LOW);
  digitalWrite(ledRed,LOW);
  delay(50);
  digitalWrite(ledGreen,HIGH);
  digitalWrite(ledRed,HIGH);
  digitalWrite(ledpin, HIGH);
  Serial.begin(9600);
}

void Dr_Strange(){
  
   char *pass = malloc(tamanho*sizeof(char));
   
   for(i = 0; i <= tamanho; i++){
    pass[i] = NULL;
   }
  
   char var = NULL;
   
      i = 0;
      while( i < tamanho){
          var = kpd.getKey();
          Serial.println("Eu vim barganhar!");
          if(var != NULL){
              Serial.println(var);
            pass[i] = var;
            var = NULL;
            
            digitalWrite(ledRed,HIGH);
            delay(200);
            digitalWrite(ledRed,LOW);
            
            i++;
          }
      }
  
  Serial.println(pass);
  Serial.println(password);
 
      if(strcmp(pass,password) == 0){
         digitalWrite(ledpin, LOW);
         digitalWrite(ledGreen,LOW);
        Serial.println("WIN");
      }else{
         digitalWrite(ledpin,HIGH);
         digitalWrite(ledGreen,HIGH);
         digitalWrite(ledRed,LOW);
            delay(500);
         digitalWrite(ledRed,HIGH);
            delay(200);
         digitalWrite(ledRed,LOW);
            delay(200);
         digitalWrite(ledRed,HIGH);
         Serial.println("OVER");
      }
 
}

void loop()
{
  key = kpd.getKey();

  if(key != NO_KEY)  // Check for a valid key.
  {
    switch (key)
    {
      case '*':
      Serial.println(key);
       digitalWrite(ledGreen,LOW);
       delay(500);
       digitalWrite(ledGreen,HIGH);
       digitalWrite(ledRed,LOW);
        Dr_Strange();
        key = NULL;
        digitalWrite(ledRed,HIGH);
      
        break;
        
      case '#':
      Serial.println(key);
        digitalWrite(ledRed,LOW);
            delay(1000);
        digitalWrite(ledRed,HIGH);
        digitalWrite(ledGreen,HIGH);
        digitalWrite(ledpin, HIGH);
        break;
      
      default:
        Serial.println(key);
        digitalWrite(ledRed,LOW);
          delay(50);
        digitalWrite(ledRed,HIGH);
    }
  }
}

