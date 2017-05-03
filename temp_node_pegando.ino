////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Arquivo:   ESP01_DHT11.ino
//Tipo:      Codigo-fonte para utilizar no ESP8266 atraves da IDE do Arduino
//Autor:     Marco Rabelo para o canal Infortronica Para Zumbis (www.youtube.com/c/InfortronicaParaZumbis)
//Descricao: Curso de ESP8266 - Lendo dados de sensores de umidade e temperatura (DHT11 e DHT22), postando
//           estas leituras na internet e recuperando de forma grafica no celular ou PC.
//Video:     https://youtu.be/uzcL80nT5cU
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Importando as bibliotecas necessarias
#include "ESP8266WiFi.h"


//Nome da sua rede Wifi
const char* ssid = "MW Daniel";

//Senha da rede
const char* password = "c5c4c3c2c1";



// Site que receber? os dados
const char* host = "dweet.io";

void setup() {
  

  // Conectando na rede wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("conectando\n");
    delay(50000);
  }
}

void loop() {
  
  Serial.begin(9600);
  
  //Criando uma conexao TCP ERRO TA AQUI!! NESSA PORTA
  
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.print("ta no whlie");
    return;
  }
  Serial.print("Chegou aqui, conectou");
  
  int t = 19;
  int u = 76;
  int icC =  5;
  char nome[] = "Geovaneeeeeeeewww";
 
  
  client.print(String("GET /dweet/for/tempupe?temperatura=") + String(t) +
                "&umidade=" + String(u) + "&indice=" + String(icC) + "&frase=" + (nome) +
                " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  delay(6000);
}