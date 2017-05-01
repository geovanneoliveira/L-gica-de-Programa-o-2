//-----------------------------------------------------------------//
//---IRLED e ReceptorClone--- PINOS COM A PROGRMACAO --------------//
//_________________________________________________________________//
//-----------------------------------------------------------------//
// IR led  ________________________________________________________//
//_____ GND PIN -  ------------------------------------------------//
//_____ S PIN 3 c/ resistor ---------------------------------------//
//Receptor IR  ____________________________________________________//
//-----------------------------------------------------------------//
//_____ GND PIN - -------------------------------------------------//
//_____ 2 PIN 5V --------------------------------------------------//
//_____ S PIN 11 --------------------------------------------------//
//__________________________________________________________________//
//------------------------------------------------------------------//
//******************************************************************//


#include <IRremote.h>

IRsend irsend;

void setup()
{
  Serial.begin(9600);
}


String leStringSerial(){
    String conteudo = "";
    char caractere;
    
    // Enquanto receber algo pela serial
    while(Serial.available() > 0) {
      // Lê byte da serial
      caractere = Serial.read();
      // Ignora caractere de quebra de linha
      if (caractere != '\n'){
        // Concatena valores
        conteudo.concat(caractere);
      }
      // Aguarda buffer serial ler próximo caractere
      delay(10);
  }
    
  Serial.print("Recebi: ");
  Serial.println(conteudo);
    
  return conteudo;
}


void loop() {

    if (Serial.available()){
    
      String token = leStringSerial();
      
      Serial.println(token);
       
       int khz = 32 ; // 38kHz carrier frequency for the NEC protocol
       
            if(token == "on"){
                
                 unsigned int irSignalon[] = {8900,4500,550,1700,550,1700,550,550,600,550,550,600,550,600,550,550,550,1700,550,550,600,1700,550,1650,550,600,550,600,550,550,550,600,550,550,600,550,550,550,600,1700,550,550,550,600,550,600,550,550,550,600,550,600,550,1650,550,600,550,600,550,1700,550,550,550,550,600,600,500,600,550,600,550,550,550,600,550,550,600,550,550,550,600,550,550,600,550,600,550,550,550,600,550,550,600,550,550,600,550,600,500}; //AnalysIR Batch Export (IRremote) - RAW
                                                                                                                     
                 irsend.sendRaw(irSignalon, sizeof(irSignalon) / sizeof(irSignalon[0]), khz); //Note the approach used to automatically calculate the size of the array.
                
                 delay(50); //In this example, the signal will be repeated every 5 seconds, approximately.


            }else if(token == "dw"){
            
                 unsigned int irSignaldw[] = {8950,4450,600,1700,500,1700,550,550,600,550,600,550,550,600,550,550,550,1700,550,600,550,1700,550,1650,600,550,550,600,550,550,550,600,550,600,550,550,550,600,550,600,550,550,550,600,550,550,600,550,550,600,550,600,500,1700,550,600,550,550,600,550,550,550,600,550,550,600,550,550,600,550,550,600,550,550,600,550,550,600,550,550,550,600,550,600,550,600,500,600,550,550,600,550,550,600,550,550,550,600,550}; //AnalysIR Batch Export (IRremote) - RAW
                                                                                                                     
                 irsend.sendRaw(irSignaldw, sizeof(irSignaldw) / sizeof(irSignaldw[0]), khz); //Note the approach used to automatically calculate the size of the array.
                
                 delay(50); //In this example, the signal will be repeated every 5 seconds, approximately.

  
            }else if(token == "up"){

                 unsigned int irSignalup[] = {8950,4500,550,1700,550,1700,550,550,550,600,550,600,550,550,550,550,600,1700,550,600,500,1700,550,1700,550,600,550,550,550,600,550,550,600,550,550,600,550,600,550,550,550,600,550,550,550,600,550,600,550,550,550,600,550,1700,550,550,550,600,550,1700,550,600,550,550,550,600,550,600,500,600,550,600,550,600,500,600,550,550,600,550,550,600,550,550,600,550,550,550,600,600,500,600,550,550,600,550,550,600,550}; //AnalysIR Batch Export (IRremote) - RAW
                                                                                                                     
                 irsend.sendRaw(irSignalup, sizeof(irSignalup) / sizeof(irSignalup[0]), khz); //Note the approach used to automatically calculate the size of the array.
                
                 delay(50); //In this example, the signal will be repeated every 5 seconds, approximately.

            }else{

            Serial.println("INVALID");
       }
    }

}
