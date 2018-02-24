#include "parametrosConfig.h"
#include "definicaoEstacao.h"
#include <ESP8266WiFi.h>
#include <JsonListener.h>

/* OLED */
// Biblioteca de controlo do LCD SSD1306
#include "SSD1306.h"
// I2C_DISPLAY_ADDRESS, SDA_PIN, SCL_PIN
SSD1306  display(0x3c, D1, D2);
// Biblioteca de UI
#include "OLEDDisplayUi.h"
OLEDDisplayUi   ui( &display );


/* Wunderground */
#include "WundergroundClient.h"
#include "WeatherStationFonts.h"
#include "WeatherStationImages.h"
WundergroundClient wunderground(SISTEMA_METRICO); 

/* Ticker */
#include <Ticker.h>
Ticker ticker;

/* Time Client */
#include "TimeClient.h"
TimeClient timeClient(UTC_OFFSET);

/*
 * Declarações de funções
 */
void pesquisaWifi(OLEDDisplay *display);
void atualizaInformacao(OLEDDisplay *display);
void barraDeProgresso(OLEDDisplay *display, int percentagem, String etiqueta);
void PreparadoParaAtulizacaoMetereologica();
void barraDeProgresso(OLEDDisplay *display, int percentagem, String etiqueta);
void DesenhaPaginaMetereologia(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void desenharTempoData(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void DesenhaPrevisaoMetereologica(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void desenharHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state);
void desenhaDetalhesPrevisao(OLEDDisplay *display, int x, int y, int indiceDia);

/* frames */
FrameCallback frames[] = {desenharTempoData, DesenhaPaginaMetereologia, DesenhaPrevisaoMetereologica};
int numeroDeFrames = 3;

/* overlay */
OverlayCallback overlays[] = { desenharHeaderOverlay };
int numeroDeOverlays = 1;

void setup() {
  // Inicia a comunicação em série na velocidade 115200
  Serial.begin(115200);
  delay(100);

  // Inicializa o display
  display.init();
  // Limpa o ecrã
  display.clear();
  // Atualiza o ecrã
  display.display();

  // Vira o ecrã ao contrário
  display.flipScreenVertically();

  // Define a fonte que se quer utilizar
  display.setFont(ArialMT_Plain_10);
  // Define o alinhamento do texto para central
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  // Define o contraste do LCD para o máximo
  display.setContrast(255);

  // Imprime na consola o nome e password da rede WiFi que se quer ligar
  Serial.println("Ligando ao Wifi");
  Serial.print("SSID: ");
  Serial.println(WIFI_NOME);
  Serial.print("Password: ");
  Serial.println(WIFI_PASS);

  // Liga-se a rede WiFi
  WiFi.begin(WIFI_NOME, WIFI_PASS);

  // Indica à quanto tempo se está a espera que o WiFi ligue
  int contador = 0;

  // Verifica a ligação ao WiFi
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    // Limpa o ecrã
    display.clear();
    // Informa o utilizador que a estação metereológica se está a conetar ao WiFi
    display.drawString(64, 10, "Conectando-se ao WiFi");
    // Alterna entre os simbolos de espera de modo a dar a indicação que sistema não está bloqueado
    display.drawXbm(46, 30, 8, 8, contador % 3 == 0 ? activeSymbole : inactiveSymbole);
    display.drawXbm(60, 30, 8, 8, contador % 3 == 1 ? activeSymbole : inactiveSymbole);
    display.drawXbm(74, 30, 8, 8, contador % 3 == 2 ? activeSymbole : inactiveSymbole);

    // Imprime no display as informações do WiFi que se está a ligar    
    display.drawString(64, 40, WIFI_NOME);
    display.drawString(64, 50, WIFI_PASS);

    // Atualiza o ecrã
    display.display();

    // Se o WiFi não estiver conetado ao fim de 30 segundos, aparece no ecrã o nome das redes disponíveis
    if(contador > 30) {
      Serial.println("Desligue o aparelho e volte a ligar mais perto do Router");
      Serial.println("Verifique no código se o nome do WiFi e password estão corretas");
      //Pesquisa as redes wifi e mostra-as no Serial e no LCD
      pesquisaWifi(&display);
    }
    //Incrementa a tentativa de ligar ao WIFI
    contador++;
  }

  // Indica o utilizador que a ligação foi estabelecida
  Serial.println();
  Serial.println();
  Serial.println("Ligação Bem sucedida");

  // Define 30 frames por segundo como taxa de atualização do ecrã
  ui.setTargetFPS(30);

  // Informa a biblioteca do UI quais sãos os simbolos de espera e utilização
  ui.setActiveSymbol(emptySymbol);  
  ui.setInactiveSymbol(emptySymbol);
  
  ui.disableIndicator();

  // Define o vetor de ponteiros com o nome das funções que desenham os frames e overlay
  ui.setFrames(frames, numeroDeFrames);
  ui.setOverlays(overlays, numeroDeOverlays);

  // Inicializa o UI
  ui.init();

  Serial.println("");
  atualizaInformacao(&display);

  // Função que informa o sistema para correr a função de atualização a períodos de tempo definidos
  ticker.attach(SEC_INTERVALO_ATUALIZACAO, PreparadoParaAtulizacaoMetereologica);

  
}

void loop() {
  //Vira o ecrã ao contrário
  display.flipScreenVertically();
  
  //Se já desenhou o frame anterior e quer receber nova atualização metereológica
  if(AtualizacaoMetereologica && ui.getUiState()->frameState == FIXED) {
    //Atuliza a informação metereológica
    atualizaInformacao(&display);
  }

  int tempoDeAtualizacao =ui.update();

  if (tempoDeAtualizacao > 0) {
    //IMPLEMENTAR AQUI OUTRAS FUNCIONALIDADES
    delay(tempoDeAtualizacao);
  }
}

/* Pesquisa de redes wifi 
 */
void pesquisaWifi(OLEDDisplay *display) {
  int j;
  // Coloca o módulo de Wifi em modo SPA - workstation
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  Serial.println("pesquisando Redes");

  // Imprime as redes disponiveis
  display->clear();
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  String text = "Redes dispoíveis:";
  int textWidth = display->getStringWidth(text);
  display->drawString(3, 1, text);
  display->display();
  String rede;
  String number;
  
  // WiFi.scanNetworks retorna o número de redes WiFi encontradas
  int n = WiFi.scanNetworks();
  Serial.println("Pesquisa Completa");
  if (n == 0) {
    Serial.println("Sem redes encontradas!");
    rede = "Sem redes encontradas!";
    textWidth = display->getStringWidth(rede);
    display->drawString(6, 14, rede);
    display->display();
  }
  else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Escreve o nome das redes encontradas
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);

      //Imprime as redes encontradas no ecrã LCD
      if(i*10+14<68) {
        rede = WiFi.SSID(i);
        j=i+1;
        number = j;
        
        display->drawString(6, 14 + i*10, number); //Desenha o digito
        textWidth = display->getStringWidth(number); // calcular o temanho do digito
        display->drawString(6 + textWidth, 14 + i*10, ": "); //Desenha ": "
        textWidth = textWidth + display->getStringWidth(number); // Calcula o tamanho de "x: "
        display->drawString(6 + textWidth, 14 + i*10, rede);
        display->display();
      }
    }
  }
  Serial.println("");

  // Espera 15 segundos e volta a fazer scan
  delay(15000);
}

/*******************************************************
 * Desenha uma barra com a percentagem da atualização
 *******************************************************/
void barraDeProgresso(OLEDDisplay *display, int percentagem, String etiqueta) {
  display->clear();
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  display->drawString(64, 10, etiqueta);
  display->drawProgressBar(2, 28, 124, 10, percentagem);
  display->display();
}

/****************************************
 * Atualiza a informação do ecra
 ****************************************/
void atualizaInformacao(OLEDDisplay *display) {
  //Vira o ecrã ao contrário
  display->flipScreenVertically();
  
  barraDeProgresso(display, 10, "Atualizando o tempo...");
  timeClient.updateTime();
  barraDeProgresso(display, 40, "Atualizando metereologia...");
  wunderground.updateConditions(WUNDERGRROUND_API_KEY, WUNDERGRROUND_LINGUA, WUNDERGROUND_PAIS, WUNDERGROUND_CIDADE);
  barraDeProgresso(display, 70, "Atualizando previsão...");
  wunderground.updateForecast(WUNDERGRROUND_API_KEY, WUNDERGRROUND_LINGUA, WUNDERGROUND_PAIS, WUNDERGROUND_CIDADE);
  lastUpdate = timeClient.getFormattedTime();
  AtualizacaoMetereologica = false;
  barraDeProgresso(display, 100, "Atualizacao Terminada");
  delay(1000);
}

/***************************************************
* Desenhar página da hora
****************************************************/
void desenharTempoData(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) 
{
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  String date = wunderground.getDate();
  int textWidth = display->getStringWidth(date);
  display->drawString(64 + x, 5 + y, date);
  display->setFont(ArialMT_Plain_24);
  String time = timeClient.getFormattedTime();
  textWidth = display->getStringWidth(time);
  display->drawString(64 + x, 15 + y, time);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
}

/***************************************************
* Desenha a página do tempo atual
****************************************************/
void DesenhaPaginaMetereologia(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) 
{
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(60 + x, 5 + y, wunderground.getWeatherText());

  display->setFont(ArialMT_Plain_24);
  String temp = wunderground.getCurrentTemp() + "°C";
  display->drawString(60 + x, 15 + y, temp);
  int tempWidth = display->getStringWidth(temp);

  display->setFont(Meteocons_Plain_42);
  String weatherIcon = wunderground.getTodayIcon();
  int weatherIconWidth = display->getStringWidth(weatherIcon);
  display->drawString(32 + x - weatherIconWidth / 2, 05 + y, weatherIcon);
}

/***************************************************
* Desenha página de previsão
****************************************************/
void DesenhaPrevisaoMetereologica(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) 
{
  desenhaDetalhesPrevisao(display, x, y, 0);
  desenhaDetalhesPrevisao(display, x + 44, y, 2);
  desenhaDetalhesPrevisao(display, x + 88, y, 4);
}

/***************************************************
* Desenha detalhes da previsão metereologica
****************************************************/
void desenhaDetalhesPrevisao(OLEDDisplay *display, int x, int y, int indiceDia) 
{
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  String day = wunderground.getForecastTitle(indiceDia).substring(0, 3);
  day.toUpperCase();
  display->drawString(x + 20, y, day);

  display->setFont(Meteocons_Plain_21);
  display->drawString(x + 20, y + 12, wunderground.getForecastIcon(indiceDia));

  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 20, y + 34, wunderground.getForecastLowTemp(indiceDia) + "|" + wunderground.getForecastHighTemp(indiceDia));
  display->setTextAlignment(TEXT_ALIGN_LEFT);
}

/***************************************************
* Desenha o header
****************************************************/
void desenharHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) 
{
  // Seleciona a cor branca (caso se use ecrãs multicolour)
  display->setColor(WHITE);
  display->setFont(ArialMT_Plain_10);
  
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  // Define uma string e vai buscar a informação do tempo atual
  String tempOut = "Tem: "+ wunderground.getCurrentTemp() + "°C";
  display->drawString(0, 54, tempOut);
  
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  // Escreve o número do frame atual
  display->drawString(128, 54, String(state->currentFrame + 1) + "/" + String(numeroDeFrames));

  display->drawHorizontalLine(0, 52, 128);
}


/***************************************************
* Indica o sistema que já passou o tempo necessário e que deve atualizar a informação
*   A função é simples pois ela corre com recurso a uma interrupção, que não convém que atrapalhe o regular funcionamento do processador
****************************************************/
void PreparadoParaAtulizacaoMetereologica() 
{
  Serial.println("Setting readyForUpdate to true");
  AtualizacaoMetereologica = true;
}

