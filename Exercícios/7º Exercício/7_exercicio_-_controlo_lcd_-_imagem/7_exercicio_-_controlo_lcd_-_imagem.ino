/*
 * Workshop IOT - HackerSchool
 * 6º Exercício - Controlo LCD - imagens
 * 
 
 Biblioteca:
 void drawXbm(int16_t x, int16_t y, int16_t width, int16_t height, const char* xbm);
 
 * 
*/

#include "SSD1306.h" // Biblioteca de controlo do LCD SSD1306
#include "OLEDDisplayUi.h" // Biblioteca de UI

// Incluir ficheiro de imagens
#include "Imagem.h"

// I2C_DISPLAY_ADDRESS, SDA_PIN, SCL_PIN
SSD1306  display(0x3c, D1, D2);
OLEDDisplayUi   ui( &display );

void setup() {
  // Inicia a comunicação I2C com o LCD
  display.init();

  //Vira o ecrã ao contrário
  display.flipScreenVertically();

  //Define o contraste do ecrã para o máximo
  display.setContrast(255);

  // Limpa o conteúdo do ecrã
  display.clear();

  // Desenha a imagem

  // Atualiza o ecrã
  display.display();

}

void loop() {
  // put your main code here, to run repeatedly:

}
