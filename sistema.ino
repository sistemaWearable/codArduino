#include <SPI.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_SH1106.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h> //biblioteca sensor de temperatura
#include <Wire.h>
#include "RTClib.h"

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};

//Definindo o tamanho da tela em pixels
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

//Declaração para o SH1106 conectar ao I2C (pinos SDA e SCL)
#define OLED_RESET 4
//Adafruit_SH1106 display(OLED_RESET);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//#define botao 18
//int b1 = 0;
int botaoStatus = 1;
int  tela = 1;
boolean mudouTela = true;
unsigned long millisSegundos = millis();

void setup() {
  Serial.begin(9600);
  delay(3000); //espera até o console abrir

  pinMode(botao, INPUT_PULLUP); //define que o botão será uma entrada digital
  delay(1000);
  digitalWrite(botao, HIGH);

  //reconhecimento do módulo RTC
  if(! rtc.begin()){
    Serial.println("RTC não localizado");
    while (1);
  }
  if(rtc.begin()){
    Serial.println("RTC encontrado");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  
  }
  delay(300);

  mlx.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Inicializa com o I2C no endereço 0x3C (SH1106_SWITCHCAPVCC - valor constante que especifica o tipo de fonte de energia para a biblioteca)
  display.clearDisplay(); // limpa o display 
  display.display();  //disponibiliza todas as mudanças no hardware de exibição

/*  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo2, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(3000);*/

}

void loop() {
  DateTime now = rtc.now();

  botaoStatus = digitalRead(botao); //faz a leitura do botão

/*  if(botaoStatus == LOW){  //se o botão for pressionado a tela muda e é acrescentado +1 na variável mudouTela
    mudouTela = true;
    tela++;
  }else{
    mudouTela = false; 
  }

  //se a quantidade de tela exibida for maior que, retorna para a primeira tela
  if(tela > 4){
    tela = 1;
  }*/


    telaHora();
    delay(3000);
    telaData();  
    delay(3000);
    temperaturaCorporal();  
    delay(3000);
    temperaturaAmbiente();
    delay(3000);

  //se mudouTela for verdadeiro, atualiza para a tela posterior
  if(mudouTela == true){
    switch(tela){
      case 1: {telaHora();}
      break;
      case 2: {telaData();}
      break;
      case 3: {temperaturaCorporal();}
      break;
      case 4: {temperaturaAmbiente();}
      break;
    }
  }
}

/*
void telaTemperatura(){
  DateTime now = rtc.now();
  
  display.clearDisplay(); 
  

  display.setTextColor(WHITE, BLACK);
  display.drawRect(68, 22, 3, 3, WHITE); //desenha o "º"
  draw_text(0,0, "Temperatura: ", 1);
  draw_text(72, 22, "C", 2);
  
  display.setTextSize(2);
  display.setCursor(0, 22);
  display.print(rtc.getTemperature());

  display.display();
}*/



void draw_text(byte x_pos, byte y_pos, char *text, byte text_size) {
  display.setCursor(x_pos, y_pos);
  display.setTextSize(text_size);
  display.print(text);
  display.display();
}