/*
 * Workshop IOT - HackerSchool
 * 4º Exercício - Controlo na web - solução

 Biblioteca:
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

 // Inicia o servidor
  server.begin();

*/
#include <ESP8266WiFi.h>

//Definição das constantes com o nome da rede e a password
const char* ssid = "Chico";
const char* password = "12345678";

//Estado do LED
int value = LOW;

WiFiServer server(80);
 
void setup() {
  // Inicia a comunicação em série na velocidade 115200
  Serial.begin(115200); 
  delay(10);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Estabelece a ligação ao WiFi
  WiFi.begin(ssid, password);

  //Verifica se a conecção foi estabelecida
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Inicia o servidor
  server.begin();
  Serial.println("Server started");
 
  // Imprime o endereço IP
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  //Controlo do LED
  //Definição do pin como output
  pinMode(LED_BUILTIN, OUTPUT);
  //Liga o LED
  digitalWrite(LED_BUILTIN, LOW);
 
 
}
 
void loop() {
  // Verifica se há algum cliente ligado
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Espera interação do cliente
  Serial.println("Novo Cliente");
  while(!client.available()){
    delay(1);
  }
 
  // Le o botão que o cliente presionou
  String request = client.readStringUntil('\r');

  // Imprime o que o cliente pressionou
  Serial.println(request);
  client.flush();
 
  // Resposta do servidor
  if (request.indexOf("/LED=ON") != -1)  {
    value = LOW;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    value = HIGH;
  }
 
  // Faz a ação pretendida pelo cliente no LED
  digitalWrite(LED_BUILTIN, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Led pin is now: ");
  //Imprime estado do LED
  if(value == HIGH) {
    client.print("Off");
  } else {
    client.print("On");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
 
