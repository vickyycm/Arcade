#pragma once
const int ASC_PISOS[4] = {275, 215, 155, 95};
const int ASC_X[3] = {190, 43, 190};
const int ASC_W = 32;
const int ASC_MW = 14;
const int ASC_MH = 18;
const float ASC_VEL = 0.75f;
const unsigned long ASC_TICK = 20;
const unsigned long ASC_FRAME = 50;
struct AscLift {
  float y;
  int dir;
};
struct AscVisual {
  int x, y, vidas;
  bool visible;
  uint32_t puntos;
  int elevY[3];
  int peligroX[2];
};
AscLift ascLifts[3];
float ascMarioX = 22, ascMarioY = 257, ascVy = 0;
int ascPiso = 0, ascMontado = -1;
bool ascSuelo = true, ascInvulnerable = false, ascPantallaLista = false;
unsigned long ascUltimoTick = 0, ascUltimoFrame = 0, ascGolpeInicio = 0;
unsigned long ascSaltoRegistrado = 0;
bool ascSaltoEnCola = false;
bool ascBotonAnterior = false;
unsigned long ascInicioPulsacion = 0;
bool ascIgnorarSaltoAlSoltar = false;
float ascPeligroX[2] = {80, 160};
int ascPeligroDir[2] = {1, -1};
AscVisual ascAnterior;
bool ascAnteriorValido = false;
void dkAscensoresIniciar() {
  ascMarioX = 22;
  ascMarioY = ASC_PISOS[0] - ASC_MH;
  ascVy = 0;
  ascPiso = 0;
  ascMontado = -1;
  ascSuelo = true;
  ascInvulnerable = false;
  ascPantallaLista = false;
  ascAnteriorValido = false;
  ascUltimoTick = millis();
  ascUltimoFrame = 0;
  ascGolpeInicio = 0;
  ascPeligroX[0] = 80;
  ascPeligroX[1] = 160;
  ascPeligroDir[0] = 1;
  ascPeligroDir[1] = -1;
  for (int i = 0; i < 3; ++i) {
    ascLifts[i].y = ASC_PISOS[i] - 1;
    ascLifts[i].dir = -1;
  }
  dkSaltoPendiente = false;
  ascSaltoEnCola = false;
  ascSaltoRegistrado = 0;
  ascBotonAnterior = digitalRead(BTN_JUMP) == LOW;
  ascInicioPulsacion = millis();
  ascIgnorarSaltoAlSoltar = ascBotonAnterior;
  Serial.println("DK: ascensores iniciados");
}
void dkAscensoresGolpe() {
  if (ascInvulnerable) return;
  vidasMario--;
  dkSonidoImpacto();
  if (vidasMario <= 0) {
    vidasMario = 0;
    estadoActual = PANTALLA_GAME_OVER;
    return;
  }
  ascMarioX = 22;
  ascMarioY = ASC_PISOS[0] - ASC_MH;
  ascVy = 0;
  ascPiso = 0;
  ascMontado = -1;
  ascSuelo = true;
  ascInvulnerable = true;
  ascGolpeInicio = millis();
}
void dkAscensoresPasoLogica() {
  if (estadoActual != JUEGO_DONKEY_KONG) return;
  unsigned long ahora = millis();
  if (ahora - ascUltimoTick < ASC_TICK) return;
  ascUltimoTick = ahora;
  // Leer el boton solo en Ascensores, sin cambiar sketch.ino ni Pac-Man.
  bool botonPresionado = digitalRead(BTN_JUMP) == LOW;
  if (botonPresionado && !ascBotonAnterior) {
    ascInicioPulsacion = ahora;
  }
  if (botonPresionado &&
      ahora - ascInicioPulsacion >= REBOTE_BOTON &&
      !ascIgnorarSaltoAlSoltar) {
    ascSaltoEnCola = true;
    ascSaltoRegistrado = ahora;
    ascIgnorarSaltoAlSoltar = true;
  }
  ascBotonAnterior = botonPresionado;
  // sketch.ino tambien notifica al soltar. Evitar un segundo salto.
  if (dkSaltoPendiente) {
    if (!ascIgnorarSaltoAlSoltar) {
      ascSaltoEnCola = true;
      ascSaltoRegistrado = ahora;
    }
    dkSaltoPendiente = false;
    ascIgnorarSaltoAlSoltar = false;
  }
  if (!botonPresionado && !dkSaltoPendiente &&
      ascIgnorarSaltoAlSoltar &&
      ahora - ascInicioPulsacion > TIEMPO_MENU) {
    ascIgnorarSaltoAlSoltar = false;
  }
  if (ascSaltoEnCola && ahora - ascSaltoRegistrado > 500)
    ascSaltoEnCola = false;
  for (int i = 0; i < 3; ++i) {
    float alto = ASC_PISOS[i + 1] - 1;
    float bajo = ASC_PISOS[i] - 1;
    ascLifts[i].y += ascLifts[i].dir * ASC_VEL;
    if (ascLifts[i].y <= alto) {
      ascLifts[i].y = alto;
      ascLifts[i].dir = 1;
    } else if (ascLifts[i].y >= bajo) {
      ascLifts[i].y = bajo;
      ascLifts[i].dir = -1;
    }
  }
  for (int i = 0; i < 2; ++i) {
    ascPeligroX[i] += ascPeligroDir[i] * 0.9f;
    if (ascPeligroX[i] < 16) {
      ascPeligroX[i] = 16;
      ascPeligroDir[i] = 1;
    }
    if (ascPeligroX[i] > 224) {
      ascPeligroX[i] = 224;
      ascPeligroDir[i] = -1;
    }
  }
  if (ascInvulnerable && ahora - ascGolpeInicio >= 1500)
    ascInvulnerable = false;
  int joy = analogRead(JOY_X);
  int mov = joy > 3000 ? -VELOCIDAD_MARIO :
            joy < 1000 ? VELOCIDAD_MARIO : 0;
  if (ascMontado >= 0) {
    int i = ascMontado;
    ascMarioY = ascLifts[i].y - ASC_MH;
    ascMarioX += mov;
    ascMarioX = constrain(ascMarioX, 10.0f, 216.0f);
    if (ascSaltoEnCola) {
      ascSaltoEnCola = false;
      ascMontado = -1;
      ascSuelo = false;
      ascVy = -5.0f;
      dkSonidoSalto();
    } else if (ascLifts[i].y <= ASC_PISOS[i + 1] + 1 &&
               ascLifts[i].dir == 1) {
      ascPiso = i + 1;
      ascMontado = -1;
      ascSuelo = true;
      ascVy = 0;
      ascMarioY = ASC_PISOS[ascPiso] - ASC_MH;
    }
  } else {
    ascMarioX += mov;
    ascMarioX = constrain(ascMarioX, 10.0f, 216.0f);
    if (ascSuelo && ascSaltoEnCola) {
      ascSaltoEnCola = false;
      ascSuelo = false;
      ascVy = -5.0f;
      dkSonidoSalto();
    }
    if (!ascSuelo) {
      ascMarioY += ascVy;
      ascVy += 0.45f;
      if (ascVy >= 0 &&
          ascMarioY >= ASC_PISOS[ascPiso] - ASC_MH) {
        ascMarioY = ASC_PISOS[ascPiso] - ASC_MH;
        ascVy = 0;
        ascSuelo = true;
      }
    }
    if (ascSuelo && ascPiso < 3) {
      int i = ascPiso;
      float centro = ascMarioX + ASC_MW / 2.0f;
      float pisoElev = ascLifts[i].y;
      // Solo se aborda cuando el elevador esta cerca del piso inferior.
      if (fabsf(centro - ASC_X[i]) <= 13 &&
          fabsf(pisoElev - (ASC_PISOS[i] - 1)) <= 10 &&
          ascLifts[i].dir < 0) {
        ascMontado = i;
        ascSuelo = false;
        ascMarioX = ASC_X[i] - ASC_MW / 2.0f;
        ascMarioY = pisoElev - ASC_MH;
      }
    }
  }
  for (int i = 0; i < 2; ++i) {
    int nivel = i + 1;
    int py = ASC_PISOS[nivel] - 10;
    if (!ascInvulnerable && ascMontado < 0 &&
        ascMarioX + ASC_MW > ascPeligroX[i] - 6 &&
        ascMarioX < ascPeligroX[i] + 6 &&
        ascMarioY + ASC_MH > py - 6 &&
        ascMarioY < py + 6) {
      dkAscensoresGolpe();
      if (estadoActual != JUEGO_DONKEY_KONG) return;
      break;
    }
  }
  if (ascPiso == 3 && ascMontado < 0 &&
      ascSuelo && ascMarioX <= 45) {
    dkPuntuacion += 500;
    dkEtapaActual = DK_ETAPA_REMACHES;
    dkPantallaEtapaPendiente = true;
    Serial.println("DK: ascensores completados");
  }
}
uint16_t ascFondo(int x, int y) {
  if (x < 0 || x >= 240 || y < 0 || y >= 320)
    return ILI9341_BLACK;
  if (y == 31 && x >= 5 && x < 235)
    return ILI9341_CYAN;
  for (int i = 0; i < 4; ++i) {
    if (x >= 10 && x < 230 &&
        y >= ASC_PISOS[i] && y < ASC_PISOS[i] + 8) {
      if (y == ASC_PISOS[i] + 2 && (x - 10) % 18 < 9)
        return ILI9341_YELLOW;
      return ILI9341_DARKGREY;
    }
  }
  for (int i = 0; i < 3; ++i) {
    if (abs(x - ASC_X[i]) <= 17 &&
        y >= ASC_PISOS[i + 1] && y <= ASC_PISOS[i]) {
      if (abs(x - ASC_X[i]) == 17)
        return ILI9341_BLUE;
    }
  }
  if (x >= 15 && x < 46 && y >= 65 && y < 89)
    return ILI9341_GREEN;
  return ILI9341_BLACK;
}
void ascCapturar(AscVisual &s) {
  s.x = (int)roundf(ascMarioX);
  s.y = (int)roundf(ascMarioY);
  s.vidas = vidasMario;
  s.visible = !ascInvulnerable ||
              ((millis() / 150) % 2 == 0);
  s.puntos = dkPuntuacion;
  for (int i = 0; i < 3; ++i)
    s.elevY[i] = (int)roundf(ascLifts[i].y);
  for (int i = 0; i < 2; ++i)
    s.peligroX[i] = (int)roundf(ascPeligroX[i]);
}
uint16_t ascPixel(int x, int y, const AscVisual &s) {
  uint16_t c = ascFondo(x, y);
  for (int i = 0; i < 3; ++i) {
    if (x >= ASC_X[i] - ASC_W / 2 &&
        x < ASC_X[i] + ASC_W / 2 &&
        y >= s.elevY[i] && y < s.elevY[i] + 6)
      c = ILI9341_CYAN;
  }
  for (int i = 0; i < 2; ++i) {
    int dx = x - s.peligroX[i];
    int dy = y - (ASC_PISOS[i + 1] - 10);
    if (dx * dx + dy * dy <= 36)
      c = ILI9341_ORANGE;
  }
  if (s.visible &&
      x >= s.x && x < s.x + ASC_MW &&
      y >= s.y && y < s.y + ASC_MH) {
    const char* const* sprite =
      ((s.x / 5) & 1) ? DK_MARIO_PASO : DK_MARIO_QUIETO;
    char p = sprite[y - s.y][x - s.x];
    if (p != '.') c = dkColorPixel(p);
  }
  return c;
}
void ascRegion(
  int l, int t, int r, int b,
  const AscVisual &s
) {
  l = constrain(l, 0, 240);
  r = constrain(r, 0, 240);
  t = constrain(t, 55, 295);
  b = constrain(b, 55, 295);
  if (l >= r || t >= b) return;
  for (int y = t; y < b; y += BUFFER_ALTO) {
    int h = min(BUFFER_ALTO, b - y);
    for (int x = l; x < r; x += BUFFER_ANCHO) {
      int w = min(BUFFER_ANCHO, r - x);
      for (int yy = 0; yy < h; ++yy)
        for (int xx = 0; xx < w; ++xx)
          bufferRegion[yy * w + xx] =
            ascPixel(x + xx, y + yy, s);
      tft.drawRGBBitmap(x, y, bufferRegion, w, h);
    }
  }
}
void ascDibujarEscenario() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(8, 12);
  tft.print("DONKEY KONG - ASCENSORES");
  for (int y = 55; y < 295; y += BUFFER_ALTO) {
    for (int x = 0; x < 240; x += BUFFER_ANCHO) {
      int w = min(BUFFER_ANCHO, 240 - x);
      int h = min(BUFFER_ALTO, 295 - y);
      for (int yy = 0; yy < h; ++yy)
        for (int xx = 0; xx < w; ++xx)
          bufferRegion[yy * w + xx] =
            ascFondo(x + xx, y + yy);
      tft.drawRGBBitmap(x, y, bufferRegion, w, h);
    }
  }
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(18, 73);
  tft.print("EXIT");
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(8, 301);
  tft.print("3S MENU");
  ascPantallaLista = true;
}
void dkAscensoresRenderizar() {
  if (dkEtapaActual != DK_ETAPA_ASCENSORES ||
      estadoActual != JUEGO_DONKEY_KONG) {
    return;
  }
  if (!ascPantallaLista)
    ascDibujarEscenario();
  unsigned long ahora = millis();
  if (ahora - ascUltimoFrame < ASC_FRAME)
    return;
  ascUltimoFrame = ahora;
  AscVisual actual;
  if (dkMutex != nullptr &&
      xSemaphoreTake(dkMutex, pdMS_TO_TICKS(5)) != pdTRUE) {
    return;
  }
  ascCapturar(actual);
  if (dkMutex != nullptr)
    xSemaphoreGive(dkMutex);
  if (!ascAnteriorValido) {
    ascRegion(0, 55, 240, 295, actual);
    ascAnteriorValido = true;
  } else {
    // Actualizar solo las zonas que cambiaron.
    if (ascAnterior.x != actual.x ||
        ascAnterior.y != actual.y ||
        ascAnterior.visible != actual.visible) {
      int dx = abs(ascAnterior.x - actual.x);
      int dy = abs(ascAnterior.y - actual.y);
      if (dx <= ASC_MW && dy <= ASC_MH) {
        ascRegion(
          min(ascAnterior.x, actual.x) - 1,
          min(ascAnterior.y, actual.y) - 1,
          max(ascAnterior.x, actual.x) + ASC_MW + 1,
          max(ascAnterior.y, actual.y) + ASC_MH + 1,
          actual
        );
      } else {
        ascRegion(
          ascAnterior.x - 1,
          ascAnterior.y - 1,
          ascAnterior.x + ASC_MW + 1,
          ascAnterior.y + ASC_MH + 1,
          actual
        );
        ascRegion(
          actual.x - 1,
          actual.y - 1,
          actual.x + ASC_MW + 1,
          actual.y + ASC_MH + 1,
          actual
        );
      }
    }
    for (int i = 0; i < 3; ++i) {
      if (ascAnterior.elevY[i] == actual.elevY[i])
        continue;
      int x = ASC_X[i];
      ascRegion(
        x - 17,
        min(ascAnterior.elevY[i], actual.elevY[i]) - 1,
        x + 17,
        max(ascAnterior.elevY[i], actual.elevY[i]) + 7,
        actual
      );
    }
    for (int i = 0; i < 2; ++i) {
      if (ascAnterior.peligroX[i] == actual.peligroX[i])
        continue;
      int y = ASC_PISOS[i + 1] - 10;
      ascRegion(
        min(ascAnterior.peligroX[i], actual.peligroX[i]) - 7,
        y - 7,
        max(ascAnterior.peligroX[i], actual.peligroX[i]) + 8,
        y + 8,
        actual
      );
    }
  }
  ascAnterior = actual;
  static int vidasAnteriores = -1;
  static uint32_t puntosAnteriores = 0xFFFFFFFF;
  if (vidasAnteriores != actual.vidas ||
      puntosAnteriores != actual.puntos) {
    tft.fillRect(6, 39, 228, 12, ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(1);
    tft.setCursor(8, 40);
    tft.printf(
      "VIDAS:%d SCORE:%lu",
      actual.vidas,
      (unsigned long)actual.puntos
    );
    vidasAnteriores = actual.vidas;
    puntosAnteriores = actual.puntos;
  }
}
