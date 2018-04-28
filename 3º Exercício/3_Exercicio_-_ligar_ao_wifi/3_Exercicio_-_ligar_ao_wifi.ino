/*
 * Workshop IOT -  HackerSchool
 * 3º Exercício - Ligar ao WiFi 

 Bibliteca:
 // Inicia a ligação ao wifi
 WiFi.begin(ssid, password);

 // Retorna o estado do módulo e ligação wifi
   WiFi.status() pode retornar:
    WL_CONNECTED: assigned when connected to a Wi-Fi network
    WL_NO_SHIELD: assigned when no Wi-Fi shield is present
    WL_IDLE_STATUS: a temporary status assigned when WiFi.begin() is called and remains active until the number of attempts expires (resulting in WL_CONNECT_FAILED) or a connection is established (resulting in WL_CONNECTED)
    WL_NO_SSID_AVAIL: assigned when no SSID are available
    WL_SCAN_COMPLETED: assigned when the scan networks is completed
    WL_CONNECT_FAILED: assigned when the connection fails for all the attempts
    WL_CONNECTION_LOST: assigned when the connection is lost
    WL_DISCONNECTED: assigned when disconnected from a network
 WiFi.status()

*/

#include <ESP8266WiFi.h>

//Constantes com o nome da rede wifi e password
const char* ssid = "";
const char* password = "";
 
void setup() {
  // Inicializa o canal de comunicação em série na velocidade 115200
  
  delay(10);

  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Inicia a ligação ao wifi


  // Testa a ligação
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  // Indica para o terminal que a ligação foi bem sucedida
  
 
}

void loop() {

}

