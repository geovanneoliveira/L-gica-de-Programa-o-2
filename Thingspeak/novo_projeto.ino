#include <ESP8266WiFi.h>  
#include <stdlib.h>
#include <string.h>

//defines
#define SSID_REDE     "MW Daniel"  //coloque aqui o nome da rede que se deseja conectar
#define SENHA_REDE    "c1234567"  //coloque aqui a senha da rede que se deseja conectar
#define TEMPO_ESPERA      30    //Tempo entre envios de dados ao servidor (em segundos)
#define INTERVALO_ENVIO_THINGSPEAK  TEMPO_ESPERA * 1000  //intervalo entre envios de dados ao ThingSpeak (em milisegundos)
 
//constantes e variáveis globais
char EnderecoAPIThingSpeak[] = "api.thingspeak.com";
String ChaveEscritaThingSpeak = "IVDLZ7ZFNZD2P4T1";
long lastConnectionTime;
WiFiClient client;



//------------------------

const int Rele = D1;       //3
int corrente1 = 0;      
const int inputPin = D2;    //Sensor de presenca 
const int pinoSensor =A0;   //Sensor de energia
int presenca = 0;


 
int sensorValue_aux = 0;
float valorSensor = 0;
float valorCorrente = 0;
float voltsporUnidade = 0.004887586; // 5%1023
// Para ACS712 de  5 Amperes use 0.185
// Para ACS712 de 10 Amperes use 0.100
//  Para ACS712 de 5 Amperes use 0.066
float sensibilidade = 0.185;
int tensao = 220;



 
//prototypes
void EnviaInformacoesThingspeak(String StringDados);
void FazConexaoWiFi(void);
//float FazLeituraUmidade(void);
 
/*
 * Implementações
 */

//sensor de corrente

void funcao_e(){
   for(int i=2000; i>0; i--){
    // le o sensor na pino analogico A0 e ajusta o valor lido ja que a saída do sensor é (1023)vcc/2 para corrente =0
    sensorValue_aux = (analogRead(pinoSensor) -510);
    // somam os quadrados das leituras.
    valorSensor += pow(sensorValue_aux,2);
    delay(1);
  }
  // finaliza o calculo da média quadratica e ajusta o valor lido para volts
  valorSensor = (sqrt(valorSensor/ 2000)) * voltsporUnidade;
  // calcula a corrente considerando a sensibilidade do sernsor (185 mV por amper)
  valorCorrente = (valorSensor/sensibilidade);

}
 
//Função: envia informações ao ThingSpeak
//Parâmetros: String com a  informação a ser enviada
//Retorno: nenhum
void EnviaInformacoesThingspeak(String StringDados)
{
    if (client.connect(EnderecoAPIThingSpeak, 80))
    {         
        //faz a requisição HTTP ao ThingSpeak
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+ChaveEscritaThingSpeak+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(StringDados.length());
        client.print("\n\n");
        client.print(StringDados);
   
        lastConnectionTime = millis();
        Serial.println("\n- Informações enviadas ao ThingSpeak!");
        
     }   
}

//Função: faz a conexão WiFI
//Parâmetros: nenhum
//Retorno: nenhum


void FazConexaoWiFi(void){
    client.stop();
    Serial.print("Conectando-se à rede WiFi..."); 
    delay(1000);
    WiFi.begin(SSID_REDE, SENHA_REDE);
 
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
        //WiFi.begin(SSID_REDE, SENHA_REDE);
    }
 
    Serial.println("");
    Serial.println("WiFi connectado com sucesso!");  
    Serial.print("\nNOME DA REDE: ");
    Serial.println(SSID_REDE);
    Serial.print("IP OBTIDO: ");
    Serial.println(WiFi.localIP());
 
    delay(1000);
}

 
void setup()
{  
    Serial.begin(9600);
    lastConnectionTime = 0; 
   
    FazConexaoWiFi();
    Serial.print("\nEnviando dados a cada ");
    Serial.print(TEMPO_ESPERA);
    Serial.print(" Segundos...");
    Serial.print("\nAguardando para enviar dados...");

//-------------------------------
    pinMode(Rele, OUTPUT);
    pinMode(inputPin, INPUT);
    pinMode(pinoSensor,INPUT);
}
 
//loop principal
void loop(){
    char  Fieldenergia[40], 
          Fieldpresenca[20];

    //--------------------


  presenca = digitalRead(inputPin);
  
  funcao_e();
  float oo = abs((valorCorrente * tensao)-126);
  corrente1 = (int)oo;
  //Serial.println(" E(W)  L   Pre");
 
  //Serial.println("  ");
  
  //sequencia de if para "parar" o sistema

  if (presenca == LOW) { 
    Serial.println("Nao tem gente");
    digitalWrite(Rele, LOW);
  }
  else{
    Serial.println("TEM gente");
    digitalWrite(Rele, HIGH);
  }
  delay(150);

//-----------
    
    //Força desconexão ao ThingSpeak (se ainda estiver desconectado)
    if (client.connected())
    {
        client.stop();
        Serial.print("  Presenca: ");
        Serial.println(presenca);
        Serial.print("  Corrente: ");
        Serial.println(corrente1);
        Serial.print("\n\nAguardando para enviar dados...");
    }
         
    //verifica se está conectado no WiFi e se é o momento de enviar dados ao ThingSpeak
    if(!client.connected() && (millis() - lastConnectionTime > INTERVALO_ENVIO_THINGSPEAK)){
        sprintf(Fieldenergia,"field1=%d",corrente1);        //Formata a string para o padrão de envio do thingspeak
        sprintf(Fieldpresenca,"&field2=%d",presenca);       //Formata a string para o padrão de envio do thingspeak
        strcat(Fieldenergia, Fieldpresenca);                     //Concatena as duas strings em uma (Fieldenergia)
        EnviaInformacoesThingspeak(Fieldenergia);                         //Envia as informaçõe
    }else{
        Serial.print(".");
    }
 
     delay(1000);

}
