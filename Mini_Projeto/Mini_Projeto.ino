#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <avr/wdt.h>
#include <string.h>

#define LEDGREEN 8
#define BUTTON1 4
#define BUTTON2 5
#define BUTTON3 6
#define BUTTON4 7
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

const PROGMEM unsigned int irSignal_OnData[] = {8850,4450,500,600,500,600,500,600,500,600,500,600,500,600,500,600,500,600,500,600,500,600,500,600,550,550,550,1650,550,1650,550,550,550,550,550,550,550,1650,550,550,550,550,550,550,550,550,550,550,550,550,550,1650,550,550,550,1650,550,1650,550,1650,550,1650,550,1650,550,1650,550};
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
  lcd.clear();             //Inicializa o LCD 16x2

  pinMode(BUTTON1,INPUT);//Inicia Buttons
  pinMode(BUTTON2,INPUT);//Inicia Buttons
  pinMode(BUTTON3,INPUT);//Inicia Buttons
  pinMode(BUTTON4,INPUT);//Inicia Buttons
 
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF; //Prepara chave - padrao de fabrica = FFFFFFFFFFFFh

  pinMode(LEDGREEN,OUTPUT); // Led
  digitalWrite(LEDGREEN,LOW);
}
 

void nome_lcd(){
  String var;
  int len = 0;
  
  for( int i = 0; i< strlen(Id_nome);i++){
    if(!(Id_nome[i] == ' ')){
      var.concat(String(Id_nome[i]));
      len++;
    }
  }
  lcd.setCursor((16 - len)/2,0);
  lcd.print(var);
}

void atributos_lcd(){
  lcd.setCursor(1,1);
  lcd.print("AR ");
  lcd.setCursor(4,1);
  if (condicao_AR == 0){
    lcd.print("OFF");
  }
  else {
    lcd.print(temp_AR);
    //lcd.print("    dw");
  }
  lcd.setCursor(10,1);
  lcd.print("DW ");
  lcd.setCursor(13,1);
  if (condicao_DW == 0){
    lcd.print("OFF");
  }
  else {
    lcd.print("ON");
  }
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
    digitalWrite(LEDGREEN,HIGH);
    Serial.println("Cartão Valido!");
    start_system();
    nome_lcd();
    atributos_lcd();
    
    delay(2000);

    while(function_exit() == false) {
      
      if(digitalRead(BUTTON1) == HIGH){
        while (digitalRead(BUTTON1) == HIGH) {}
        if(condicao_DW == 0) condicao_DW = 1;
        else condicao_DW = 0;

        OnOff(condicao_DW,DW);
        lcd.clear();
        nome_lcd();
        atributos_lcd();     
      }

      if( digitalRead(BUTTON2) == HIGH){
        while ( digitalRead(BUTTON2) == HIGH) {}
         
         }
         
      if(digitalRead(BUTTON3) == HIGH){
        while (digitalRead(BUTTON3) == HIGH){}
        
        }
      if(digitalRead(BUTTON4) == HIGH){
        while (digitalRead(BUTTON4) == HIGH){}
        
        }


       //Serial.println("Em espera!");


    }


    shutdown_system();
  }
  else
  {
    Serial.println("Cartão Invalido!");
  }
  digitalWrite(LEDGREEN,LOW);
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

  int len_temp = 59;
  int khz = 28;
  unsigned int irSignaltemp[len_temp];

  if (temperatura_ar == 18){

    for(int i = 0; i < len_temp; i++)
    {
      irSignaltemp[i] = pgm_read_word_near(irSignaltemp18 + i);
    }

  }
  else if (temperatura_ar == 19){

    for(int i = 0; i < len_temp; i++)
    {
      irSignaltemp[i] = pgm_read_word_near(irSignaltemp19 + i);
    }

  }
  else if (temperatura_ar == 20){
    
    for(int i = 0; i < len_temp; i++)
    {
      irSignaltemp[i] = pgm_read_word_near(irSignaltemp20 + i);
    }

  }
  else if (temperatura_ar == 21){

    for(int i = 0; i < len_temp; i++)
    {
      irSignaltemp[i] = pgm_read_word_near(irSignaltemp21 + i);
    }

  }
  else if (temperatura_ar == 22){

    for(int i = 0; i < len_temp; i++)
    {
      irSignaltemp[i] = pgm_read_word_near(irSignaltemp22 + i);
    }

  }
  else if (temperatura_ar == 23){

    for(int i = 0; i < len_temp; i++)
    {
      irSignaltemp[i] = pgm_read_word_near(irSignaltemp23 + i);
    }

  }
  else if (temperatura_ar == 24){

    for(int i = 0; i < len_temp; i++)
    {
      irSignaltemp[i] = pgm_read_word_near(irSignaltemp24 + i);
    }

  }
  else if (temperatura_ar == 25){

    for(int i = 0; i < len_temp; i++)
    {
      irSignaltemp[i] = pgm_read_word_near(irSignaltemp25 + i);
    }

  }
  else if (temperatura_ar == 26){

    for(int i = 0; i < len_temp; i++)
    {
      irSignaltemp[i] = pgm_read_word_near(irSignaltemp26 + i);
    }
  }  
    
  irsend.sendRaw(irSignaltemp, sizeof(irSignaltemp) / sizeof(irSignaltemp[0]), khz);
  delay(50);
}

void OnOff (int condicao, int aparelho){
  int khz = 32;

  int len_DW = 67;
  int len_AR = 59;

  unsigned int irSignalAR[len_AR];
  unsigned int irSignalDW[len_DW];
  
  if (condicao == 1){
    if (aparelho == 0){


    for(int i = 0; i < len_AR ; i++)
    {
      irSignalAR[i] = pgm_read_word_near(irSignal_OnAr + i);
    }



   irsend.sendRaw(irSignalAR, sizeof(irSignalAR) / sizeof(irSignalAR[0]), khz);
   delay(1000);

  }
  else if (aparelho == 1){

    for(int i = 0; i < len_DW ; i++)
    {
      irSignalDW[i] = pgm_read_word_near(irSignal_OnData + i);
    }

      irsend.sendRaw(irSignalDW, sizeof(irSignalDW) / sizeof(irSignalDW[0]), khz);
      delay(1000);
    }
    
  }
  else {
    if (aparelho == 0){

    for(int i = 0; i < len_AR ; i++){
      irSignalAR[i] = pgm_read_word_near(irSignal_OffAr + i);
    }
    irsend.sendRaw(irSignalAR, sizeof(irSignalAR) / sizeof(irSignalAR[0]), khz);
    delay(50);
    }

    else if (aparelho == 1){
      for(int i = 0; i < len_DW ; i++){
        irSignalDW[i] = pgm_read_word_near(irSignal_OffData + i);
      }

      irsend.sendRaw(irSignalDW, sizeof(irSignalDW) / sizeof(irSignalDW[0]), khz); 
      delay(500);   
      irsend.sendRaw(irSignalDW, sizeof(irSignalDW) / sizeof(irSignalDW[0]), khz);
      delay(500);
    }
    
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
