/*
 * Workshop IOT -  HackerSchool
 * 1º Exercício - Colocar LED incluído na placa a piscar - solução

  Biblioteca:
  // Inicializa o pino como input, output e pull-up
  pinMode(LED, TIPE);     

  // Define um valor lógico nos pinos digitais
  digitalWrite(PIN, Valor lógico);

  // Para o microcontrolador durante x milissegundos
  delay(milissegundos);                      


 *
 */


// Função de inicialização
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Inicializa o pino LED_BUILTIN como output
}

// Função que corre para sempre
void loop() {
  digitalWrite(LED_BUILTIN, LOW);   // Liga o LED (Devido às propriedades da placa ESP-12, colocar o nível lógica a '0', liga o LED)
  delay(1000);                      // Espera 1 segundo
  
  digitalWrite(LED_BUILTIN, HIGH);  // Desliga o LED (agora será necessário colocar o nível lógica a '1' para desligar o LED)
  delay(2000);                      // Espera 2 segundos
}
