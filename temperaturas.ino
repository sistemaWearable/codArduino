float tempAmbiente;
float tempCorporal;

void temperaturaCorporal(){
  display.clearDisplay(); 

  tempCorporal = mlx.readObjectTempC();

  display.setTextColor(WHITE, BLACK);
  display.drawRect(68, 22, 3, 3, WHITE); //desenha o "º"
  draw_text(0,0, "Temp. Corporal: ", 1);
  draw_text(72, 22, "C", 2);
  
  display.setTextSize(2);
  display.setCursor(0, 22);
  display.print(tempCorporal);

  display.display();    
}

void temperaturaAmbiente(){
  display.clearDisplay();
  
  tempAmbiente = mlx.readAmbientTempC();

  display.setTextColor(WHITE, BLACK);
  display.drawRect(68, 22, 3, 3, WHITE); //desenha o "º"
  draw_text(0,0, "Temp. Ambiente: ", 1);
  draw_text(72, 22, "C", 2);
  
  display.setTextSize(2);
  display.setCursor(0, 22);
  display.print(tempAmbiente);

  display.display(); 
}