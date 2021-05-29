const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

const char* serverName = "http://192.168.18.11:4000/agenda";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

String sensorReadings;
float sensorReadingsArr[9];

void reqGET() {
  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status()== WL_CONNECTED){
              
      sensorReadings = httpGETRequest(serverName);
      Serial.println(sensorReadings);
      JSONVar myObject = JSON.parse(sensorReadings);
  
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      Serial.print("JSON object = ");
      Serial.println(myObject);
    
      JSONVar keys = myObject.keys();
    
      for (int i = 0; i < keys.length(); i++) {
        JSONVar value = myObject[keys[i]];
        Serial.print(keys[i]);
        Serial.print(" = ");
        Serial.println(value);
        sensorReadingsArr[i] = double(value);
      }
      
      bool hora      = sensorReadingsArr[0];
      bool domingo   = sensorReadingsArr[1];
      bool segunda   = sensorReadingsArr[2];
      bool terca     = sensorReadingsArr[3];
      bool quarta    = sensorReadingsArr[4];
      bool quinta    = sensorReadingsArr[5];
      bool sexta     = sensorReadingsArr[6];
      bool sabado    = sensorReadingsArr[7];
      bool bloqueado = sensorReadingsArr[8];

      if(bloqueado == 1){
        if(domingo == 1 && hora == buffer){
          motor_vibracao();  
        }else if (segunda == 1 && hora == buffer){
          motor_vibracao();
        }else if (terca == 1 && hora == buffer){
          motor_vibracao();
        }else if (quarta == 1 && hora == buffer){
          motor_vibracao();
        }else if (quinta == 1 && hora == buffer){
          motor_vibracao();
        }else if (sexta == 1 && hora == buffer){
          motor_vibracao();
        }else if (sabado == 1 && hora == buffer){
          motor_vibracao();
        }else {
          Serial.println("Não possui alarme agendado entre as semanas.");
        }
      }else{
        Serial.println("Não possui alarme agendado.");
      }
    }
    else {
      Serial.println("WiFi desconectado");
    }
    lastTime = millis();
  }
}

String httpGETRequest(const char* serverName) {
  HTTPClient http;

  http.begin(serverName);

  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Erro: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}
