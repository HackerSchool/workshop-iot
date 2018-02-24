/*
 * Workshop IOT - HackerSchool
 * 7º Exercício - Controlo LCD - frames e overlays - solução
 * 

Repositório de Funcções:
https://github.com/ThingPulse/esp8266-oled-ssd1306

  //Define o número de frames por segundo (atualizações do ecrã) - o normal é 30 ou 60
  ui.setTargetFPS(numero de frames);

  // Define o simbolo que indica qual é o frame atual
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);

  // Define o sitio do indicador de frame
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(RIGHT);

  // Define onde é que o primeiro frame está localizado na barra de frames
  ui.setIndicatorDirection(LEFT_RIGHT);

  // Tipos de transição entre frames
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  // Seleciona os frames
  ui.setFrames(frames, numeroDeFrames);

  // Adiciona os overlays
  ui.setOverlays(overlays, numeroDeOverlays);

  // Inicializa o display e o UI
  ui.init();

 * 
*/

#include "SSD1306.h" // Biblioteca de controlo do LCD SSD1306
#include "OLEDDisplayUi.h" // Biblioteca de UI

// Incluir ficheiro de imagens
#include "imagem.h"

// I2C_DISPLAY_ADDRESS, SDA_PIN, SCL_PIN
SSD1306  display(0x3c, D1, D2);
OLEDDisplayUi   ui( &display );

// Declaração das funções
void desenharOverlay(OLEDDisplay *display, OLEDDisplayUiState* state);
void desenhaFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void desenhaFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);

// Vetor guarda um ponteiro para as funções que irão desenhar cada um dos frames
FrameCallback frames[] = { desenhaFrame1, desenhaFrame2 };

// Número de frames no nosso UI
int numeroDeFrames = 2;

// Vetor que guarda um ponteiro para as funções que desenham os overlays
OverlayCallback overlays[] = { desenharOverlay };

// Número de frames no nosso UI
int numeroDeOverlays = 1;


// Função que desenha o overlay
void desenharOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  // Alinha o texto à esquerda
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  // Define o tipo de letra
  display->setFont(ArialMT_Plain_10);
  // Desenha uma linha horizontal
  display->drawHorizontalLine(0, 52, 128);
  // Escreve Overlay
  display->drawString(0, 52, "Overlay");
}

// Função que desenha o primeiro frame
void desenhaFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  // Desenha o  logotipo da HS
  display->drawXbm(x, y, HS_Width, HS_Height, HS_logo_bitmap);
}

// Função que desenha o segundo frame
void desenhaFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  // Alinha o texto à esquerda
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  // Define o tipo de letra
  display->setFont(ArialMT_Plain_10);
  // Função que escreve uma string em várias linhas sem se preocupar com a formatação
  display->drawStringMaxWidth(0 + x, 0 + y, 128, "Neste fantástico workshop da HackerSchool estou a dar os meus primeiros passos em IoT!");
}

void setup() {
  // Inicia a comunicação em série na velocidade 115200
  Serial.begin(115200);

  //Define o número de frames por segundo (atualizações do ecrã) - o normal é 30 ou 60
  ui.setTargetFPS(30);

  // Define o simbolo que indica qual é o frame atual
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);

  // Define o sitio do indicador de frame
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(RIGHT);

  // Define onde é que o primeiro frame está localizado na barra de frames
  ui.setIndicatorDirection(LEFT_RIGHT);

  // Tipos de transição entre frames
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  // Seleciona os frames
  ui.setFrames(frames, numeroDeFrames);

  // Adiciona os overlays
  ui.setOverlays(overlays, numeroDeOverlays);

  // Inicializa o display e o UI
  ui.init();

  //Vira o ecrã ao contrário
  display.flipScreenVertically();

}


void loop() {
  // Inteiro que guarda quantos ms faltam até o ecrã ser atualizado
  int remainingTimeBudget = ui.update();

  // Se ainda existir tempo entre as atualizações do ecrã, podemos colocar o arduino a fazer mais coisas
  if (remainingTimeBudget > 0) {
    // Colocar o arduino a fazer mais coisas

    // Atraso o microprocessador para manter o numero de frames por segundo desejado
    delay(remainingTimeBudget);
  }
}
