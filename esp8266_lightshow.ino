#define PORT_RED 0
#define PORT_GREEN 4
#define PORT_BLUE 5
#define SEQ_LENGTH 100
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "functions.h"
#include "index.h"
#include <iostream>
#include <string>

const char* ssid = "A1-424ee1";
const char* password = "2s3l32pl1l1c";
ESP8266WebServer server(80);
IPAddress address(10, 0, 0, 42);
IPAddress gateway(10, 0, 0, 138);
IPAddress subnet(255, 255, 255, 0);

unsigned short color_counter = 0;
unsigned long int timer = 0;
unsigned short color_change_delay = 1;
unsigned int gradient_length;

COLOR color_seq[SEQ_LENGTH + 1];
COLOR gradient[SEQ_LENGTH / 10];

void setup(){
	pinMode(PORT_RED, OUTPUT);
	pinMode(PORT_GREEN, OUTPUT);
	pinMode(PORT_BLUE, OUTPUT);

	//Wi-Fi Setup
	Serial.begin(115200);
	Serial.print("Connecting to ");
  	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while(WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	if (!WiFi.config(address, gateway, subnet)) {
    	Serial.println("STA Failed to configure");
  	}

	Serial.println("");
	Serial.println("WiFi connected.");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

	server.on("/", handleRoot);
	server.on("/actionpage", handleForm);

	server.begin();
}

void loop(){
	server.handleClient();
	if(color_seq[color_counter].EOA)
    color_counter = 0;

  	if(millis() - timer >= color_change_delay){
    	color_counter++;
    	timer = millis();
  	}

  	PWM(PORT_RED, PORT_GREEN, PORT_BLUE, color_seq[color_counter]);
}

void handleForm(){
  if(server.arg("submit")){
    color_counter = 0;
    String colormode = server.arg("colormode");
    gradient_length = toInteger(server.arg("gradientlength"));

    if(gradient_length > (SEQ_LENGTH / 10)){
      gradient_length = (SEQ_LENGTH / 10);
    }

    String colorname;
    for(int i = 0; i < gradient_length; i++){
      colorname = "cw";
      colorname.concat(i);
      convertRGB(&gradient[i], server.arg(colorname));
#ifdef DEBUG
        Serial.print("Color "); Serial.print(i);
        Serial.print("R: "); Serial.print(gradient[i].red);
        Serial.print(" G: "); Serial.print(gradient[i].green);
        Serial.print(" B: "); Serial.println(gradient[i].blue);
#endif
    }
      color_change_delay = toInteger(server.arg("delayinput"));
      setColorSequenze(colormode, gradient, gradient_length, color_seq);
#ifdef DEBUG
      Serial.println("Length: ");
      Serial.println(gradient_length);
      for(int i = 0; i < SEQ_LENGTH; i++){
        Serial.print("Color "); Serial.print(i);
        Serial.print("R: "); Serial.print(colorSeq[i].red);
        Serial.print(" G: "); Serial.print(colorSeq[i].green);
        Serial.print(" B: "); Serial.print(colorSeq[i].blue);
        Serial.print("  EOA: ");Serial.println(colorSeq[i].EOA);
      }
#endif
    timer = millis();
  }
  String s = "<script>window.onload(location.replace(\"/\"))</script>";
  server.send(200, "text/html", s);
}

void handleRoot(){
	String s = HTML_CONTENT;
	server.send(200, "text/html", s);
}
