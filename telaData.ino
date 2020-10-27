void telaData(){
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
}