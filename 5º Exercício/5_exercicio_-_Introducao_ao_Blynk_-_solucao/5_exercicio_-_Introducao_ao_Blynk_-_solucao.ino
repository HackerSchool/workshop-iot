/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  This sketch shows how to read values from Virtual Pins

  App project setup:
    Slider widget (0...100) on Virtual Pin V1
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "355c34dade3248c3948e990932ed3224";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Chico";
char pass[] = "12345678";

int pinValueV1;
int sliderValue;

// Esta funçãp é chamada sempre que a app escreve no pin V0
BLYNK_WRITE(V0) {
  pinValueV1 = param.asInt(); // Atribui o valor do pino V0 para uma variavel
  Serial.println(pinValueV1);
}


// Esta funçãp é chamada sempre que a app escreve no pin V1
BLYNK_WRITE(V1) {
  sliderValue = param.asInt(); // Atribui o valor do pino V0 para uma variavel

  // Processa a nova informação
  Serial.println(sliderValue);

}

void setup()
{
  // Debug console
  Serial.begin(9600);

  // Inicializa o LED integrado como um pino de saída
  pinMode(LED_BUILTIN, OUTPUT); 

  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  
  // processa o valor do pin V1
  if(pinValueV1 == 0) {
    digitalWrite(LED_BUILTIN, LOW); // Liga o LED 
  }
  else {
    digitalWrite(LED_BUILTIN, HIGH); // Desliga o LED
  }
}

