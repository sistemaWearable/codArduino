void reqPost() {

 if(WiFi.status()== WL_CONNECTED){   
   HTTPClient http;   

   http.begin("http://192.168.18.11:4000/estatisticas");  
   http.addHeader("Content-Type","application/json");
    
   int ocorrencia1 = http.POST("{\"cod_ocorrencia\":\"1\",\"nome_ocorrencia\":\"Temperatura Corporal\", \"valor_ocorrencia\": "+ String (tempCorporal)"}");   //Send the actual POST request
   int ocorrencia2 = http.POST("{\"cod_ocorrencia\":\"2\",\"nome_ocorrencia\":\"BPM\", \"valor_ocorrencia\": "+ String (BPM)"}");   //Send the actual POST request


   if(ocorrencia1>0 && ocorrencia2>0){

    String response = http.getString();                      

    Serial.println(ocorrencia1);   
    Serial.println(ocorrencia2);   

    Serial.println(response);

   }else{

    Serial.print("Erro ao realizar o POST: ");
    Serial.println(ocorrencia1);
    Serial.println(ocorrencia2);
   }

   http.end();

 }else{

    Serial.println("Erro na conexão");   

 }

  delay(150000);

}
