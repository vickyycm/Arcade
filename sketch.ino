
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS 10
#define TFT_DC 9
#define TFT_MOSI 11
#define TFT_SCK 12
#define JOY_X 4
#define JOY_Y 5
#define BTN_JUMP 6
#define BUZZER 7

Adafruit_ILI9341 tft(TFT_CS, TFT_DC);

enum EstadoSistema {
  MENU_PRINCIPAL,
  JUEGO_DONKEY_KONG,
  JUEGO_PACMAN,
  PANTALLA_GAME_OVER,
  PANTALLA_VICTORIA
};

EstadoSistema estadoActual = MENU_PRINCIPAL;

const int UMBRAL_ARRIBA = 3000;
const int UMBRAL_ABAJO = 1000;
const int CENTRO_MIN = 1500;
const int CENTRO_MAX = 2600;

const unsigned long TIEMPO_MENU = 3000;
const unsigned long REBOTE_BOTON = 30;

int juegoSeleccionado = 0;
bool joystickListo = true;

unsigned long inicioPulsacion = 0;
unsigned long ultimoCambioBoton = 0;

bool botonEstable = false;
bool ultimaLecturaBoton = false;
bool pulsacionLargaDetectada = false;
bool esperarSoltarBoton = false;

#include "donkeykong.h"
#include "pacman.h"

void actualizarSeleccionMenu() {
  tft.setTextSize(2);

  tft.drawRect(
    15, 155, 210, 45,
    juegoSeleccionado == 0 ? ILI9341_YELLOW : ILI9341_DARKGREY
  );

  tft.setTextColor(
    juegoSeleccionado == 0 ? ILI9341_YELLOW : ILI9341_WHITE,
    ILI9341_BLACK
  );
  tft.setCursor(40, 170);
  tft.print("DONKEY KONG");

  tft.drawRect(
    15, 210, 210, 45,
    juegoSeleccionado == 1 ? ILI9341_YELLOW : ILI9341_DARKGREY
  );

  tft.setTextColor(
    juegoSeleccionado == 1 ? ILI9341_YELLOW : ILI9341_WHITE,
    ILI9341_BLACK
  );
  tft.setCursor(70, 225);
  tft.print("PAC-MAN");
}

void dibujarMenu() {
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(3);
  tft.setCursor(30, 25);
  tft.print("MINI");

  tft.setCursor(30, 55);
  tft.print("ARCADE");

  tft.setTextSize(1);
  tft.setTextColor(ILI9341_CYAN);
  tft.setCursor(95, 95);
  tft.print("ESP32-S3");

  tft.drawFastHLine(20, 115, 200, ILI9341_BLUE);

  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(52, 130);
  tft.print("SELECCIONAR JUEGO");

  actualizarSeleccionMenu();

  tft.setTextColor(ILI9341_CYAN);
  tft.setCursor(55, 275);
  tft.print("JOYSTICK: ELEGIR");

  tft.setCursor(55, 290);
  tft.print("BOTON: CONFIRMAR");
}

void actualizarMenu() {
  int valorY = analogRead(JOY_Y);

  if (valorY >= CENTRO_MIN && valorY <= CENTRO_MAX) {
    joystickListo = true;
    return;
  }

  if (!joystickListo) return;

  int nuevaSeleccion = juegoSeleccionado;

  if (valorY > UMBRAL_ARRIBA) {
    nuevaSeleccion = 0;
    joystickListo = false;
  } else if (valorY < UMBRAL_ABAJO) {
    nuevaSeleccion = 1;
    joystickListo = false;
  }

  if (nuevaSeleccion != juegoSeleccionado) {
    juegoSeleccionado = nuevaSeleccion;
    actualizarSeleccionMenu();
  }
}

void iniciarJuego() {
  if (juegoSeleccionado == 0) {
    dkIniciar();
  } else {
    estadoActual = JUEGO_PACMAN;
    iniciarPacman();
  }

  esperarSoltarBoton = false;
}

void volverAlMenu() {
  estadoActual = MENU_PRINCIPAL;
  juegoSeleccionado = 0;
  joystickListo = false;

  dibujarMenu();
  esperarSoltarBoton = true;
}

void actualizarBoton() {
  unsigned long ahora = millis();
  bool lectura = digitalRead(BTN_JUMP) == LOW;

  if (lectura != ultimaLecturaBoton) {
    ultimoCambioBoton = ahora;
  }

  ultimaLecturaBoton = lectura;

  if (ahora - ultimoCambioBoton >= REBOTE_BOTON &&
      lectura != botonEstable) {

    botonEstable = lectura;

    if (botonEstable) {
      inicioPulsacion = ahora;
      pulsacionLargaDetectada = false;

    } else if (esperarSoltarBoton) {
      esperarSoltarBoton = false;

    } else if (!pulsacionLargaDetectada) {

      if (estadoActual == MENU_PRINCIPAL) {
        iniciarJuego();

      } else if (
        estadoActual == JUEGO_DONKEY_KONG ||
        estadoActual == PANTALLA_GAME_OVER ||
        estadoActual == PANTALLA_VICTORIA
      ) {
        dkBotonCorto();

      } else if (estadoActual == JUEGO_PACMAN) {
        botonPacman();
      }
    }
  }

  if (
    botonEstable &&
    !esperarSoltarBoton &&
    !pulsacionLargaDetectada &&
    estadoActual != MENU_PRINCIPAL &&
    ahora - inicioPulsacion >= TIEMPO_MENU
  ) {
    pulsacionLargaDetectada = true;
    volverAlMenu();
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(BTN_JUMP, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(JOY_X, INPUT);
  pinMode(JOY_Y, INPUT);

  digitalWrite(BUZZER, LOW);
  analogReadResolution(12);

  SPI.begin(TFT_SCK, -1, TFT_MOSI, TFT_CS);
  tft.begin();
  tft.setRotation(0);

  dibujarMenu();

  Serial.println("Mini Arcade modular v2.1 iniciado");
}

void loop() {
  actualizarBoton();

  switch (estadoActual) {
    case MENU_PRINCIPAL:
      actualizarMenu();
      break;

    case JUEGO_DONKEY_KONG:
      dkActualizar();
      break;

    case JUEGO_PACMAN:
      actualizarPacman();
      break;

    case PANTALLA_GAME_OVER:
      break;

    case PANTALLA_VICTORIA:
      break;
  }
}
