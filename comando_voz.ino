#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

/**        
  Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
*/
VR myVR(2,3);

uint8_t records[7]; // save record
uint8_t buf[64];

int vibrador = 9;

#define PARAR_ALARME    (0)
#define TAREFA_FEITA   (1) 

void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void setup()
{
  myVR.begin(9600);
  
  Serial.begin(115200);
  Serial.println("Vibrador está ativo");
  
  pinMode(vibrador, OUTPUT);
    
  if(myVR.clear() == 0){
    Serial.println("Reconhecedor apagado.");
  }else{
    Serial.println("VoiceRecognitionModule não foi encontrado.");
    Serial.println("Por favor, cheque a conexão e reinicie o Arduino.");
    while(1);
  }
  
  if(myVR.load((uint8_t)PARAR_ALARME) >= 0){
    Serial.println("onRecord loaded");
  }
  
  if(myVR.load((uint8_t)TAREFA_FEITA) >= 0){
    Serial.println("offRecord loaded");
  }
}

void loop()
{
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret<0){
    switch(buf[1]){
      case PARAR_ALARME:
        digitalWrite(vibrador, LOW);
        break;
      case TAREFA_FEITA:
        digitalWrite(vibrador, LOW);
        break;
      default:
        Serial.println("Função Record indefinida");
        break;
    }
    printVR(buf);
  }
}
