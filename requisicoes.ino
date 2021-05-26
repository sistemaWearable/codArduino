#include <Arduino_JSON.h>
#include <WiFi.h>
#include <HTTPClient.h>  

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

void setup() {

  Serial.begin(115200);
  delay(4000);   //Delay needed before calling the WiFi.begin

  WiFi.begin(ssid, password); 

  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");

}

void loop() {

 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status

   HTTPClient http;   

   http.begin("http://192.168.18.11:4000/representante");  //Specify destination for HTTP request
   http.addHeader("Content-Type","application/json");             //Specify content-type header
    
   int httpResponseCode = http.POST("{\"nome_rep\":\"Wellington\",\"numero_rep\":\"991083217\"}");   //Send the actual POST request

   if(httpResponseCode>0){

    String response = http.getString();                       //Get the response to the request

    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer

   }else{

    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);

   }

   http.end();  //Free resources

 }else{

    Serial.println("Error in WiFi connection");   

 }

  delay(150000);  //Send a request every 10 seconds

}
