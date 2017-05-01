// Programa : Teste Controle Remoto IR  
// Autor : Arduino e Cia  
  
#include <IRremote.h>  
  
int RECV_PIN = 11;  


  
IRrecv irrecv(RECV_PIN);  
decode_results results;  
  
void setup()  
{    
  Serial.begin(9600);  
  irrecv.enableIRIn(); // Inicializa o receptor IR  
  pinMode(10,OUTPUT);
}  
   
void loop()  
{  
  digitalWrite(10,HIGH);
 
    Serial.print("Valor lido : ");  
    Serial.println(results.value, HEX);  
    delay(500);
  irrecv.resume(); //Le o próximo valor  
 
}  
