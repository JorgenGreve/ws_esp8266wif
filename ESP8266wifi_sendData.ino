/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

*/


#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "secrets.h"

//***************************************************
// REMEMBER TO UPDATE SECRETS.H WITH WIFI SETTINGS  *
//***************************************************

// Replace with your network credentials
const char* ssid     = SECRET_SSID;   // SSID (see secrets.h)
const char* password = SECRET_PASS;   // Pswrd (see secrets.h)

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://weather.jorgre.dk/projects/weatherstation/post-esp-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

String sensorName = "Windspeed";
String dataIn;
String data;

int httpResponseCode;

unsigned int timeNow = 0;
unsigned int lastTime = 0;
unsigned int delayed = 5000;
unsigned int dataReady = 0;

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}


void loop() {  

  while(Serial.available()) {
    data = Serial.readString(); // read the incoming data as string
    dataReady = 1;
  }


  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED)
  { 
    WiFiClient client;
    HTTPClient http;
      
    if(dataReady == 1)
    {
      data.remove(0,1);   // Removes the 'W' from the windspeed data

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
      
      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      
      // Prepare your HTTP POST request data
      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName + "&value1=" + String(data) + "";
  
      Serial.println(httpRequestData);
      
      // Send HTTP POST request
      httpResponseCode = http.POST(httpRequestData);
     
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
  
      // Free up resources
      http.end();

      dataReady = 0;
    }
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}
