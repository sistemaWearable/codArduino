#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>
#include "RTClib.h"

//Definindo o tamanho da tela em pixels
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
//Declaração para o SH1106 conectar ao I2C (pinos SDA e SCL)
#define OLED_RESET 4

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};

#define botao 2 //Definindo o botão
int pressionado = 0; //Define se o botão está pressionado ou não

float tempCorporal;

void setup() {
  Serial.begin(9600);
  delay(3000);

  mlx.begin();
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  //botao
  pinMode(botao,INPUT_PULLUP);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // 
  }

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE, BLACK);
  //delay(2000); // Pause for 2 seconds

  // Draw a single pixel in white
  //display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  /*display.display();
  delay(2000);*/

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
        display.setTextSize(2);
        display.writeFillRect(0,50,128,16,BLACK);
        display.setCursor(0,10);
        display.print(analogRead(32));
        display.display();
        pressionado=0; 
      }

    Serial.println(pressionado);
    delay(400);
  }
}
