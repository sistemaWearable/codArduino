#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>
#include "RTClib.h"
#include <Arduino_JSON.h>
#include <WiFi.h>
#include <HTTPClient.h>  
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"


//Definindo o tamanho da tela em pixels
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
//Declaração para o SH1106 conectar ao I2C (pinos SDA e SCL)
#define OLED_RESET 4
#define botao 12 //Definindo o botão


int pressionado = 0; //Define se o botão está pressionado ou não
int BPM;
float tempCorporal;
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};


Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);
  delay(3000);

  /*Inicia o Wi-fi*/

  WiFi.begin(ssid, password); 

  while (WiFi.status() != WL_CONNECTED) { //Verifica a conexão
    delay(1000);
    Serial.println("Conectando ao Wifi..");
  }

  Serial.println("Wifi Conectado");

  /*comando de voz*/
  
  myVR.begin(9600);

  if(myVR.clear() == 0){
    Serial.println("Reconhecedor apagado.");
  }else{
    Serial.println("VoiceRecognitionModule não foi encontrado.");
    Serial.println("Por favor, cheque a conexão e reinicie o Arduino.");
    while(1);
  }
  
  if(myVR.load((uint8_t)PARAR_ALARME) >= 0){
    Serial.println("onRecord carregado");
  }
  
  if(myVR.load((uint8_t)PARAR_ALARME) >= 0){
    Serial.println("offRecord carregado");
  }

  /*Inicia o mlx (sensor de temperatura corporal)*/

  mlx.begin();


  /*Procura se o rtc está ativo*/
  
  if (! rtc.begin()) {
    Serial.println("RTC não foi encontrado");
    while (1);
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  //botao
  pinMode(botao,INPUT_PULLUP);

  //motor vibracao
  pinMode(vibrador, OUTPUT);
  digitalWrite(vibrador, LOW);

  //configurações displat oled
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao localizar o display OLED"));
    for(;;); // 
  }

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE, BLACK);
}

void loop() {
  if (digitalRead(botao) == HIGH){
    pressionado++;
      if(pressionado==1){
        DateTime now = rtc.now();

        display.clearDisplay(); 
          
        /*DATA*/
        display.setTextSize(2);
        display.setCursor(0,0);
        display.print(daysOfTheWeek[now.dayOfTheWeek()]);
        
        char currentDate [16];
        uint8_t thisDay, thisMonth ;
        thisDay = now.day();
        thisMonth = now.month();
        sprintf (currentDate, "%02d/%02d/", thisDay, thisMonth); 
        
        display.setTextSize(2);
        display.setCursor(0,22);
        display.print(currentDate);
        
        display.setTextSize(2);
        display.setCursor(68,22);
        display.print(now.year(), DEC);
      
        display.display();
      }else if(pressionado==2){
        DateTime now = rtc.now();

        display.clearDisplay(); 
      
        /*HORA*/
        char buffer [16]; //guarda a quantidade de caracteres a serem usados
        uint8_t segAt, minAt, horaAt;
       // segAt = now.second();
        minAt = now.minute();
        horaAt = now.hour();
        //sprintf =  retorna o número de bytes que são gravados na matriz, sem contar o caractere nulo final.
        sprintf(buffer, "%02d:%02d", horaAt, minAt);
        display.setTextSize(2);
        display.setCursor(0,10);
        display.print("Hora:");
        display.setCursor(0,32);
        display.print(buffer);
        display.display();
      }else if(pressionado==3){
        display.clearDisplay();
         
        display.setTextSize(2);
        display.writeFillRect(0,50,128,16,BLACK);
        display.setCursor(0,10);
        display.print("Temp. Amb: ");
        display.setCursor(0,32);
        display.print(rtc.getTemperature());
        display.display();
      }else if(pressionado==4){
        display.clearDisplay(); 

        tempCorporal = mlx.readObjectTempC();
        
        display.setTextSize(2);
        display.writeFillRect(0,50,128,16,BLACK);
        display.setCursor(0,10);
        display.print("Temp. Corp: ");
        display.setCursor(0,32);
        display.print(tempCorporal);
        display.display();
      }else if(pressionado==5){
        display.clearDisplay();

        BPM = analogRead(0);
         
        display.setTextSize(2);
        display.writeFillRect(0,50,128,16,BLACK);
        display.setCursor(0,10);
        display.print("BPM: ");
        display.setCursor(0,32);
        display.print(BPM);
        display.display();
        pressionado=0; 
      }

    Serial.println(pressionado);
    delay(400);
  }

  reqPOST();
  reqGET();
  comandoVoz();
}
