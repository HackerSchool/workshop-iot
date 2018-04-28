/*
 * Workshop IOT -  HackerSchool
 * 2º Exercício - Escrever para a consola

  Biblioteca:
 // Inicia o canal de comunicação em Série
 Serial.begin(velocidade)

 // Escreve para a consola e dá "enter" no final
 Serial.println("String")

 *
 */

// Função de inicialização
void setup() {
  // Inicializa a comunicação em série no canal 115200
  
  pinMode(LED_BUILTIN, OUTPUT);     // Inicializa o pino LED_BUILTIN como output
}

// Função que corre para sempre
void loop() {
  //Escreve para a consola "LED on"
  
  digitalWrite(LED_BUILTIN, LOW);   // Liga o LED (Devido às propriedades da placa ESP-12, colocar o nível lógica a '0', liga o LED)
  delay(1000);                      // Espera 1 segundo
  
  //Escreve para a consola "LED off"

  digitalWrite(LED_BUILTIN, HIGH);  // Desliga o LED (agora será necessário colocar o nível lógica a '1' para desligar o LED)
  delay(2000);                      // Espera 2 segundos
}
