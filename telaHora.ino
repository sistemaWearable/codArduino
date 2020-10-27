void telaHora(){
  DateTime now = rtc.now();
  
  display.clearDisplay(); 

    delay(3000);
  /*HORA*/
  char buffer [16]; //guarda a quantidade de caracteres a serem usados
  uint8_t segAt, minAt, horaAt;
 // segAt = now.second();
  minAt = now.minute();
  horaAt = now.hour();
  //sprintf =  retorna o número de bytes que são gravados na matriz, sem contar o caractere nulo final.
  sprintf(buffer, "%02d:%02d", horaAt, minAt);

  display.setTextSize(2);
  display.setCursor(15,9);
  display.print(buffer);


  display.display();
  
}