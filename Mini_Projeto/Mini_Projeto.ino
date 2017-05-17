#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <IRremote.h>
#include <avr/wdt.h>


#define AR 0
#define DW 1
#define SS_PIN 10
#define RST_PIN 9

// --- Inicialização dos Dispositivos ---

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

MFRC522 mfrc522(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

IRsend irsend;


// --- Variáveis Globais ---
String teste_Validacao = "";
String code_UID = "";
String code_UID_exit = "";
int s_ArCondicionado = 0;
int s_DataShow = 0;
char Id_nome[16];
int condicao_AR = 0;
int condicao_DW = 0;
int temp_AR = 18;




// --- Codigos IR ---

const PROGMEM unsigned int irSignal_OnAr[] = {8200,4200,550,1650,550,550,550,550,550,500,600,1650,550,500,600,500,550,550,550,550,550,550,500,600,550,550,550,550,550,550,550,550,500,600,550,550,500,1650,550,1650,550,1650,550,550,550,1650,550,550,550,550,550,1600,600,550,500,1650,600,1600,550};
const PROGMEM unsigned int irSignal_OffAr[] = {8200,4150,600,1650,500,550,600,500,550,550,550,1650,550,550,550,550,550,550,550,1650,550,1650,550,500,600,500,550,600,500,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,1600,600,550,500,1650,550,550,550,550,550,550,550,1650,550};

const PROGMEM unsigned int irSignal_OnData[] = {8800,4450,500,600,500,600,500,600,500,600,500,600,500,600,500,600,550,550,550,550,500,600,500,600,550,550,550,1650,550,1650,550,550,550,550,550,550,550,1650,550,550,550,550,550,550,550,550,550,550,550,550,550,1650,550,550,550,1650,550,1650,550,1650,550,1650,550,1650,550,1650,550};
const PROGMEM unsigned int irSignal_OffData[] = {8800,4450,500,600,500,600,500,600,500,600,500,600,500,600,500,600,550,550,550,550,500,600,500,600,550,550,550,1650,550,1650,550,550,550,550,550,550,550,1650,550,550,550,550,550,550,550,550,550,550,550,550,550,1650,550,550,550,1650,550,1650,550,1650,550,1650,550,1650,550,1650,550};

const PROGMEM unsigned int irSignaltemp18[] = {8250,4150,550,1650,550,550,550,550,500,600,550,1650,500,600,500,600,500,550,550,600,500,550,550,550,550,550,550,1650,550,550,550,500,600,550,500,600,500,550,600,1650,500,1700,500,600,500,1650,550,550,550,550,550,1650,550,1650,500,1650,550,1700,500};
const PROGMEM unsigned int irSignaltemp19[] = {8250,4150,550,1700,500,600,500,550,550,550,550,1650,550,550,500,600,500,600,550,550,500,550,550,600,500,550,550,1650,550,550,550,600,500,550,550,550,550,1600,600,550,500,600,500,550,550,1650,550,550,550,600,500,600,500,550,550,550,550,550,550};
const PROGMEM unsigned int irSignaltemp20[] = {8150,4200,550,1650,500,600,500,600,500,600,500,1650,550,600,500,600,450,600,500,600,550,550,550,550,500,600,500,1700,500,550,600,500,550,600,500,600,500,1650,550,550,550,1700,450,600,550,1650,500,600,500,600,500,600,500,600,500,600,500,1700,500};
const PROGMEM unsigned int irSignaltemp21[] = {8300,4150,550,1650,500,550,550,550,550,600,500,1650,550,550,550,550,550,550,500,600,550,550,550,550,550,550,500,1700,500,600,500,550,600,550,500,600,500,1650,550,1650,550,550,500,600,550,1650,500,600,550,500,600,550,500,600,500,1650,550,550,550};
const PROGMEM unsigned int irSignaltemp22[] = {8250,4150,550,1650,550,550,550,550,500,650,450,1700,550,550,500,550,550,600,500,600,500,550,550,550,550,600,500,1700,450,650,450,600,550,550,500,600,500,1700,500,1650,550,1700,500,600,500,1650,550,550,550,550,500,600,500,600,500,1700,500,1700,500};
const PROGMEM unsigned int irSignaltemp23[] = {8250,4200,550,1650,550,500,600,550,500,600,550,1600,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,1650,550,550,550,500,600,500,550,1650,550,550,550,550,550,550,550,550,500,1700,550,550,550,550,550,550,500,1700,500,550,550,550,550};
const PROGMEM unsigned int irSignaltemp24[] = {8250,4150,550,1650,550,550,550,550,550,550,550,1650,500,600,500,550,600,500,550,550,550,550,550,550,550,550,550,1650,550,550,550,550,550,550,500,1650,600,500,550,600,500,1650,550,550,550,1650,550,550,550,550,550,550,550,1600,600,550,500,1650,600};
const PROGMEM unsigned int irSignaltemp25[] = {8300,4100,600,1650,500,550,550,550,550,550,550,1650,550,550,550,550,550,550,550,550,550,500,600,550,500,600,550,1600,550,550,550,550,550,550,550,1650,550,550,550,1650,550,550,550,550,500,1650,550,600,500,550,550,550,550,1650,550,1650,550,550,550};
const PROGMEM unsigned int irSignaltemp26[] = {8300,4150,550,1650,550,550,550,550,550,550,500,1650,600,500,550,600,500,550,550,550,550,550,550,550,550,550,550,1650,550,550,550,550,550,500,600,1650,550,500,550,1650,550,1650,550,550,550,1650,550,550,550,550,550,550,550,1600,600,1650,500,1650,550};


// --- Inicio ---

void setup(){

  Serial.begin(9600);   //Inicia a serial
  SPI.begin();          //Inicia  SPI bus
  mfrc522.PCD_Init();   //Inicia MFRC522
  lcd.begin(16, 2);     //Inicializa o LCD 16x2
 
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF; //Prepara chave - padrao de fabrica = FFFFFFFFFFFFh
}
 
void loop(){

  Serial.println("Sistema em Funcionamento");

  modo_leitura();

    Serial.println(teste_Validacao);
    Serial.println(code_UID);
    Serial.println(s_ArCondicionado);
    Serial.println(s_DataShow);
    Serial.println(Id_nome[0]);
   
  Serial.println("Leirura Realizada");

  if(teste_Validacao == "858069") // UPE
  {
    Serial.println("Cartão Valido!");

    start_system();
    delay(2000);

    while(function_exit() == false)
    {
      Serial.println("Em espera!");

      delay(1000);

    }

    shutdown_system();
  }
  else
  {
    Serial.println("Cartão Invalido!");
  }
  
  delay(1000);
  wdt_enable (WDTO_15MS);
  wdt_reset (); 
  delay(50);
  
}

void start_system(){

  if (s_ArCondicionado == 00){ }
  else
  {
    condicao_AR = 1;
    temp_AR = s_ArCondicionado; 

    OnOff(condicao_AR,AR);
    Temperatura(temp_AR);
  }

  if (s_DataShow == 0){}
  else
  {
    condicao_DW = 1;

    OnOff(condicao_DW,DW);
  }

 }


void shutdown_system(){
 
  if (condicao_AR == 0){ }
  else
  {
    condicao_AR = 0;
  
    OnOff(condicao_AR,AR);
  }

  if (condicao_DW == 0){}
  else
  {
    condicao_DW = 0;

    OnOff(condicao_DW,DW);
  }

}

 
void modo_leitura(){

  // -- Aguarda um cartão --

  while ( ! mfrc522.PICC_IsNewCardPresent())
  {
    delay(1000);
  }
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }


  //Mostra UID na serial

  Serial.print("UID da tag : ");
  String conteudo = "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i]<0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  code_UID = conteudo;
  Serial.println();
 
  //Obtem os dados do setor 1, bloco 4 = CODE_VALIDACAO  e nome
  byte sector         = 1;
  byte blockAddr      = 4;
  byte trailerBlock   = 7;
  byte status;
  byte buffer[18];
  byte size = sizeof(buffer);
 
  //Autenticacao usando chave A
  status=mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,trailerBlock, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);

  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }

  // Extrair o CODE_VALIDACAO e nome no Serial Monitor
  String temp_validacao = "";
  int j = 0;

  for (byte i = 0; i < 16; i++){

    if(i < 3){
      teste_Validacao.concat(int(buffer[i]));

    }else{
       Id_nome[j++] = (char(buffer[i]));
    }
   
  }
 
  //Obtem os dados do setor 0, bloco 1 = Status Pré-definido
  sector         = 0;
  blockAddr      = 1;
  trailerBlock   = 3;
 
  //Autenticacao usando chave A
  status=mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,trailerBlock, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK){

    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);

  if (status != MFRC522::STATUS_OK){

    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }

  // extrai os dados do staus Pré-definido
  String  temp_s_ArCondicionado = "";
  String temp_s_DataShow = "";

  for (byte i = 0; i < 3; i++){
    if(i < 2){
      temp_s_ArCondicionado.concat(char(buffer[i]));

    }else{
      temp_s_DataShow.concat(char(buffer[i]));

    }
  }
  s_ArCondicionado = temp_s_ArCondicionado.toInt();
  s_DataShow = temp_s_DataShow.toInt();
 

  Serial.println();
 
  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
  delay(3000);
}


void Temperatura (int temperatura_ar){

  int khz = 28;
  
  if (temperatura_ar == 18){
/* unsigned int *irSignaltemp;
    irSignaltemp = malloc( sizeof(irSignaltemp18) * sizeof(int) );

    for(int i = 0; i < sizeof(irSignaltemp18); i++)
    {
      irSignaltemp[i] = pgm_read_word_near(irSignaltemp18 + i);
    }
*/
    irsend.sendRaw(irSignaltemp18, sizeof(irSignaltemp18) / sizeof(irSignaltemp18[0]), khz);
   delay(50);
  }
  else if (temperatura_ar == 19){

    irsend.sendRaw(irSignaltemp19, sizeof(irSignaltemp19) / sizeof(irSignaltemp19[0]), khz);
    delay(50);
  }
  else if (temperatura_ar == 20){

    irsend.sendRaw(irSignaltemp20, sizeof(irSignaltemp20) / sizeof(irSignaltemp20[0]), khz);
    delay(50);
  }
  else if (temperatura_ar == 21){

    irsend.sendRaw(irSignaltemp21, sizeof(irSignaltemp21) / sizeof(irSignaltemp21[0]), khz);
    delay(50);
  }
  else if (temperatura_ar == 22){

    irsend.sendRaw(irSignaltemp22, sizeof(irSignaltemp22) / sizeof(irSignaltemp22[0]), khz);
    delay(50);
  }
  else if (temperatura_ar == 23){

    irsend.sendRaw(irSignaltemp23, sizeof(irSignaltemp23) / sizeof(irSignaltemp23[0]), khz);
    delay(50);
  }
  else if (temperatura_ar == 24){

    irsend.sendRaw(irSignaltemp24, sizeof(irSignaltemp24) / sizeof(irSignaltemp24[0]), khz);
    delay(50);
  }
  else if (temperatura_ar == 25){

    irsend.sendRaw(irSignaltemp25, sizeof(irSignaltemp25) / sizeof(irSignaltemp25[0]), khz);
    delay(50);
  }
  else if (temperatura_ar == 26){

    irsend.sendRaw(irSignaltemp26, sizeof(irSignaltemp26) / sizeof(irSignaltemp26[0]), khz);
    delay(50);
  }
}

void OnOff (int condicao, int aparelho){
  int khz_DW = 32;
  int khz_AR = 28;
  
  if (condicao == 1){
    if (aparelho == 0){

      irsend.sendRaw(irSignal_OnAr, sizeof(irSignal_OnAr) / sizeof(irSignal_OnAr[0]), khz_AR);
    }
    else if (aparelho == 1){

      irsend.sendRaw(irSignal_OnData, sizeof(irSignal_OnData) / sizeof(irSignal_OnData[0]), khz_DW);
    }
    delay(50);
  }
  else {
    if (aparelho == 0){

      irsend.sendRaw(irSignal_OffAr, sizeof(irSignal_OffAr) / sizeof(irSignal_OffAr[0]), khz_AR);
    }
    else if (aparelho == 1){

      irsend.sendRaw(irSignal_OffData, sizeof(irSignal_OffData) / sizeof(irSignal_OffData[0]), khz_DW);    
    }
    delay(50);
  }
}

bool function_exit(){

  // -- Aguarda um cartão --

  if( ! mfrc522.PICC_IsNewCardPresent())
  {
    return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return false;
  }


  //Mostra UID na serial
  Serial.print("UID da tag EXIT : ");
  String conteudo = "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i]<0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  code_UID_exit = conteudo;
  Serial.println();

  if (code_UID == code_UID_exit)
  {
    return true;
  }
  else
  {
    return false;
  }

}
