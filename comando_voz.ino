VR myVR(2,3);

uint8_t records[7]; // save record
uint8_t buf[64];

int vibrador = 9;

#define PARAR_ALARME    (0)

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

void comandoVoz(){
  int ret;
  ret = myVR.recognize(buf, 50);

  if(ret<0){
    switch(buf[1]){
      case PARAR_ALARME:
        if(digitalWrite(vibrador) == HIGH){
          digitalWrite(vibrador, LOW);
        }
        break;
        default:
        Serial.println("Função Record indefinida");
        break;
    }
    printVR(buf);
  }
}
