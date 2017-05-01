//-----------------------------------------------------------------//
//---RFID & LCD ---------- PINOS COM A PROGRMACAO -----------------//
//_________________________________________________________________//
//-----------------------------------------------------------------//
// LCD ____________________________________________________________//
//_____ GND PIN GND -----------------------------------------------//
//_____ VCC PIN 5V  -----------------------------------------------//
//_____ SDA PIN A4  -----------------------------------------------//
//_____ SCL PIN A5  -----------------------------------------------//
//RFID ____________________________________________________________//
//_____ SDA   PIN 10 ----------------------------------------------//
//_____ SCK   PIN 13 ----------------------------------------------//
//_____ MOSI  PIN 11 ----------------------------------------------//
//_____ MISO  PIN 12 ----------------------------------------------//
//_____ IRQ   PIN N/A ---------------------------------------------//
//_____ GND   PIN GND ---------------------------------------------//
//_____ RST   PIN 9  ----------------------------------------------//
//_____ 3.3V  PIN 3.3V --------------------------------------------//
//__________________________________________________________________//
//------------------------------------------------------------------//
//******************************************************************//


// --- Bibliotecas Auxiliares ---
#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h> 

#define SS_PIN 10
#define RST_PIN 9

// --- Mapeamento de Hardware ---

// Cria instância com MFRC522
MFRC522 mfrc522(SS_PIN, RST_PIN);   

// Inicializa o display no endereço 0x27  PINO SDA A4 & SCL A5
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// --- Variáveis Globais --- 
char st[20];
String conteudo= "";

// --- Configurações Iniciais ---
void setup() {
  Serial.begin(9600);   // Inicia comunicação Serial em 9600 baud rate
  
  SPI.begin();          // Inicia comunicação SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  
 lcd.begin (16, 2); // Inicializa o display LCD 16x2
 lcd.setBacklight(HIGH);  // Liga a luz de fundo do LCD
 
 Serial.println("Aproxime o seu cartao do leitor...");
 Serial.println();
 lcd.clear();
}


// --- assinaturas ---
void inciarLcd();
bool capturandoSinal();
void leitura();

// --- LOOP ---
void loop() {


  iniciarLcd();

  if(capturandoSinal() == true){
    
         leitura();

         if (conteudo.substring(1) == "A4 3B 4F EC") //UID 1 - Chaveiro
          {
           Serial.println("Chaveiro identificado!");
            Serial.println();
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("ACCESS  RELEASED");
            lcd.setCursor(4,1);
            lcd.print("KEY  RING");
            delay(2000);
            lcd.clear();
            lcd.setCursor(2,0);
            lcd.print("Welcome Gean");
            int k = 0;
            while(k<16){
            lcd.setCursor(k,1);
            lcd.print("*");
            delay(130);
            k++;
            }
            
             
          }else if (conteudo.substring(1) == "B4 A1 7D 01") { //UID 2 - Cartao
          
            Serial.println("Cartao identificado");
            Serial.println();
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("ACCESS  RELEASED");
            lcd.setCursor(6,1);
            lcd.print("CARD");
            delay(2000);
            lcd.clear();
            lcd.setCursor(2,0);
            lcd.print("Welcome Jose");
            int k = 0;
            while(k<16){
            lcd.setCursor(k,1);
            lcd.print("*");
            delay(130);
            k++;
            }

          }else if(conteudo.substring(1) == "F4 EB 8C BC") {
            Serial.println("Cartao identificado");
            Serial.println();
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("ACCESS  RELEASED");
            lcd.setCursor(6,1);
            lcd.print("CARD");
            delay(2000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Welcome Geovanne");
            int k = 0;
            while(k<16){
            lcd.setCursor(k,1);
            lcd.print("*");
            delay(130);
            k++;
            }
            
          }else{
            lcd.clear();
            lcd.setCursor(1,0);
            lcd.print("ACCESS  DENIED");
            lcd.setCursor(0,1);
            lcd.print("****************");
            delay(3000);
            
          }
        
          lcd.clear();
          conteudo = ""; 
      
   
  }
  

} //end 


void iniciarLcd(){
   lcd.setCursor(2, 0);
   lcd.print("PRESENT YOUR");
   lcd.setCursor(6,1);
   lcd.print("CARD");
   delay(1000);
}

bool capturandoSinal(){
   // algum cartão?
  if ( ! mfrc522.PICC_IsNewCardPresent()) return false;
  
  // Pode ler?
  if ( ! mfrc522.PICC_ReadCardSerial()) return false;

  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("VERIFICANDO...");
  delay(50);

  return true;
}

void leitura(){
   
  Serial.print("ID da tag :");  // Mostra ID na serial
  conteudo = "";
  byte letra;
  byte tam = 0;
  for (byte i = 0; i < mfrc522.uid.size; i++){

     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
     
    
    lcd.setCursor(tam,1);
    lcd.print("****");
    tam =+4;

  }

  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  
}




