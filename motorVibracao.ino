void motor_vibracao(){
  int cont;
  
  for (cont = 0; cont < 10; ++cont) {
    digitalWrite(vibrador, HIGH);
    delay(1000); 
    digitalWrite(vibrador, LOW);;
    delay(1000);
  }
}
