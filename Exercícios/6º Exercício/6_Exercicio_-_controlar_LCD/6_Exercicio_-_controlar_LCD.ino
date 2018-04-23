/*
 * Workshop IOT - HackerSchool
 * 5º Exercício - Controlar LCD - desenhar e escrever - solução
 * 
 * 
 
Biblioteca do ecrã:
 // Inicia a comunicação I2C com o LCD
  display.init();

  //Vira o ecrã ao contrário
  display.flipScreenVertically();

  //Define o contraste do ecrã para o máximo
  display.setContrast(255);
  
  // Limpa o conteúdo do ecrã
  display.clear();
  
  // Define o alinhamento da escrita
  display.setTextAlignment(TEXT_ALIGN_LEFT);
                           TEXT_ALIGN_CENTER
                           TEXT_ALIGN_RIGHT

  // Escreve um String na posição pretendida
  display.drawString(x, y, “string”);

  // Muda o tamanho da letra para 24
  display.setFont(ArialMT_Plain_24);

  // Muda o tamanho da letra para 16
  display.setFont(ArialMT_Plain_16);
  
  // Muda o tamanho da letra para 10 
  display.setFont(ArialMT_Plain_10);
  
  // Desenha um retângulo
  display.drawRect(x, y, largura, altura);

  // Desenha um circulo
  display.drawCircle(x, y, raio);

  // Desenha uma linha horizontal
  display.drawHorizontalLine(x, y, comprimento);

  // Desenha uma linha vertical
  display.drawVerticalLine(x, y, comprimento);

   // Atualiza o ecrã
  display.display();   

  Para mais funções:
  https://github.com/ThingPulse/esp8266-oled-ssd1306

 * 
 * 
 * 
*/
#include "SSD1306.h" // Biblioteca de controlo do LCD SSD1306
#include "OLEDDisplayUi.h" // Biblioteca de UI

// I2C_DISPLAY_ADDRESS, SDA_PIN, SCL_PIN
SSD1306  display(0x3c, D1, D2);
OLEDDisplayUi   ui( &display );

String HS = "HackerSchool";
String IOT = "Workshop IoT";

void setup() {
  // Inicia a comunicação I2C com o LCD
  display.init();

  //Vira o ecrã ao contrário
  display.flipScreenVertically();

  //Define o contraste do ecrã para o máximo
  display.setContrast(255);

  // Define o tamanho da letra para 10 
  display.setFont(ArialMT_Plain_10);

  // Define o alinhamento a meio
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  // Limpa o conteúdo do ecrã
  display.clear();

  // Escreve "HackerSchool"
  display.drawString(64, 18, HS);

  // Atualiza o ecrã
  display.display();

}

void loop() {
  // put your main code here, to run repeatedly:

}
