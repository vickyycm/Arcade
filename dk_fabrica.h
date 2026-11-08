#pragma once
const int FAB_Y[4] = {275, 215, 155, 95};
const int FAB_ESC_X[3] = {195, 35, 195};
const int FAB_MARIO_W = 14;
const int FAB_MARIO_H = 18;
const int FAB_ENEMIGOS = 3;
const unsigned long FAB_TICK = 20;
const unsigned long FAB_FRAME = 50;
const unsigned long FAB_INVULNERABLE = 1500;
const unsigned long FAB_INTERVALO_ENEMIGO = 2800;
const unsigned long FAB_ESPERA_INICIAL = 3000;
const float FAB_VELOCIDAD_ENEMIGO = 1.2f;
const float FAB_VELOCIDAD_CINTA = 0.55f;
struct FabObstaculo {
  float x;
  int nivel;
  int sentido;
  bool activo;
  bool puntuado;
};
struct FabVisual {
  int x;
  int y;
  int vidas;
  bool visible;
  uint32_t puntos;
  int enemigoX[FAB_ENEMIGOS];
  bool enemigoActivo[FAB_ENEMIGOS];
};
FabObstaculo fabObstaculos[FAB_ENEMIGOS];
int fabX = 22;
int fabY = 257;
int fabNivel = 0;
int fabDestino = 0;
float fabVy = 0;
float fabDesplazamientoCinta = 0.0f;
bool fabEnSuelo = true;
bool fabSubiendo = false;
bool fabVisible = true;
bool fabInvulnerable = false;
bool fabPantallaLista = false;
bool fabAnteriorValido = false;
bool fabJugable = false;
unsigned long fabInicioJugable = 0;
unsigned long fabUltimoPaso = 0;
unsigned long fabUltimoDibujo = 0;
unsigned long fabInicioGolpe = 0;
unsigned long fabUltimoEnemigo[FAB_ENEMIGOS];
FabVisual fabAnterior;
int fabSuelo(int nivel) {
  return FAB_Y[nivel] - FAB_MARIO_H;
}
int fabSentidoCinta(int nivel) {
  return (nivel % 2 == 0) ? -1 : 1;
}
void fabCrearEnemigo(int nivel) {
  FabObstaculo &e = fabObstaculos[nivel];
  e.nivel = nivel;
  e.sentido = fabSentidoCinta(nivel);
  e.x = e.sentido > 0 ? 12.0f : 220.0f;
  e.activo = true;
  e.puntuado = false;
}
void dkFabricaIniciar() {
  fabX = 22;
  fabNivel = 0;
  fabDestino = 0;
  fabY = fabSuelo(0);
  fabVy = 0;
  fabDesplazamientoCinta = 0.0f;
  fabEnSuelo = true;
  fabSubiendo = false;
  fabVisible = true;
  fabInvulnerable = false;
  fabPantallaLista = false;
  fabAnteriorValido = false;
  fabJugable = false;
  fabInicioJugable = 0;
  fabUltimoPaso = millis();
  fabUltimoDibujo = 0;
  fabInicioGolpe = 0;
  for (int i = 0; i < FAB_ENEMIGOS; ++i) {
    fabObstaculos[i].activo = false;
    fabObstaculos[i].puntuado = false;
    fabObstaculos[i].nivel = i;
    fabObstaculos[i].sentido = fabSentidoCinta(i);
    fabObstaculos[i].x = 0;
    fabUltimoEnemigo[i] = millis();
  }
  dkSaltoPendiente = false;
  Serial.println("DK: etapa 2 fabrica iniciada");
}
void dkFabricaGolpe() {
  if (fabInvulnerable ||
      estadoActual != JUEGO_DONKEY_KONG) {
    return;
  }
  --vidasMario;
  dkSonidoImpacto();
  if (vidasMario <= 0) {
    vidasMario = 0;
    estadoActual = PANTALLA_GAME_OVER;
    return;
  }
  fabX = 22;
  fabNivel = 0;
  fabDestino = 0;
  fabY = fabSuelo(0);
  fabVy = 0;
  fabDesplazamientoCinta = 0.0f;
  fabEnSuelo = true;
  fabSubiendo = false;
  fabInvulnerable = true;
  fabInicioGolpe = millis();
  fabVisible = true;
}
void dkFabricaPasoLogica() {
  if (estadoActual != JUEGO_DONKEY_KONG) {
    return;
  }
  if (!fabJugable) return;
  unsigned long ahora = millis();
  if (ahora - fabUltimoPaso < FAB_TICK) {
    return;
  }
  fabUltimoPaso = ahora;
  if (fabInvulnerable) {
    unsigned long t = ahora - fabInicioGolpe;
    if (t >= FAB_INVULNERABLE) {
      fabInvulnerable = false;
      fabVisible = true;
    } else {
      fabVisible = ((t / 150) % 2 == 0);
    }
  }
  int xJoy = analogRead(JOY_X);
  int yJoy = analogRead(JOY_Y);
  int movX = 0;
  // Velocidad manual original: 2 pixeles por actualizacion.
  if (xJoy > 3000) {
    movX = -2;
  } else if (xJoy < 1000) {
    movX = 2;
  }
  if (fabSubiendo) {
    int meta = fabSuelo(fabDestino);
    if (fabY > meta) {
      fabY = max(meta, fabY - 2);
    } else if (fabY < meta) {
      fabY = min(meta, fabY + 2);
    }
    if (fabY == meta) {
      fabNivel = fabDestino;
      fabSubiendo = false;
      fabEnSuelo = true;
      fabVy = 0;
      fabDesplazamientoCinta = 0.0f;
    }
    dkSaltoPendiente = false;
  } else {
    bool iniciarEscalera = false;
    if (fabEnSuelo) {
      if (yJoy > UMBRAL_ARRIBA && fabNivel < 3) {
        int escalera = FAB_ESC_X[fabNivel];
        if (abs(fabX + FAB_MARIO_W / 2 - escalera) <= 15) {
          fabDestino = fabNivel + 1;
          iniciarEscalera = true;
        }
      } else if (yJoy < UMBRAL_ABAJO && fabNivel > 0) {
        int escalera = FAB_ESC_X[fabNivel - 1];
        if (abs(fabX + FAB_MARIO_W / 2 - escalera) <= 15) {
          fabDestino = fabNivel - 1;
          iniciarEscalera = true;
        }
      }
    }
    if (iniciarEscalera) {
      int indice = min(fabNivel, fabDestino);
      fabX = FAB_ESC_X[indice] - FAB_MARIO_W / 2;
      fabSubiendo = true;
      fabEnSuelo = false;
      fabVy = 0;
      fabDesplazamientoCinta = 0.0f;
      dkSaltoPendiente = false;
    } else {
      int arrastre = 0;
      // La cinta transportadora arrastra a Mario
      // solamente mientras esta apoyado en ella.
      if (fabEnSuelo) {
        fabDesplazamientoCinta +=
          FAB_VELOCIDAD_CINTA * fabSentidoCinta(fabNivel);
        arrastre = (int)fabDesplazamientoCinta;
        fabDesplazamientoCinta -= arrastre;
      } else {
        fabDesplazamientoCinta = 0.0f;
      }
      // Movimiento manual y arrastre se suman.
      fabX = constrain(fabX + movX + arrastre, 10, 216);
      if (fabEnSuelo && dkSaltoPendiente) {
        fabVy = -5.0f;
        fabEnSuelo = false;
        fabDesplazamientoCinta = 0.0f;
        dkSonidoSalto();
      }
      dkSaltoPendiente = false;
      if (!fabEnSuelo) {
        fabVy += 0.45f;
        fabY += (int)roundf(fabVy);
        int suelo = fabSuelo(fabNivel);
        if (fabVy >= 0 && fabY >= suelo) {
          fabY = suelo;
          fabVy = 0;
          fabEnSuelo = true;
        }
      } else {
        fabY = fabSuelo(fabNivel);
      }
    }
  }
  // Un obstaculo independiente por plataforma:
  // primera, segunda y tercera.
  for (int i = 0; i < FAB_ENEMIGOS; ++i) {
    FabObstaculo &e = fabObstaculos[i];
    if (ahora - fabInicioJugable < FAB_ESPERA_INICIAL) continue;
    if (!e.activo) {
      if (ahora - fabUltimoEnemigo[i] >= FAB_INTERVALO_ENEMIGO) {
        fabCrearEnemigo(i);
        fabUltimoEnemigo[i] = ahora;
      }
      continue;
    }
    float xAnterior = e.x;
    e.x += e.sentido * FAB_VELOCIDAD_ENEMIGO;
    if (e.x < 5 || e.x > 231) {
      e.activo = false;
      fabUltimoEnemigo[i] = ahora;
      continue;
    }
    int ey = FAB_Y[e.nivel] - 11;
    bool colision =
      fabX + FAB_MARIO_W > (int)e.x - 5 &&
      fabX < (int)e.x + 6 &&
      fabY + FAB_MARIO_H > ey - 5 &&
      fabY < ey + 6;
    if (!fabInvulnerable && colision) {
      dkFabricaGolpe();
      if (estadoActual != JUEGO_DONKEY_KONG) {
        return;
      }
      break;
    }
    // Sumar una sola vez si Mario esquivo el obstaculo
    // en su propia plataforma, sin haber recibido el golpe.
    if (!e.puntuado && e.nivel == fabNivel && !fabSubiendo &&
        estadoActual == JUEGO_DONKEY_KONG) {
      float centroMario = fabX + FAB_MARIO_W / 2.0f;
      bool cruzo = e.sentido > 0
        ? (xAnterior < centroMario && e.x >= centroMario)
        : (xAnterior > centroMario && e.x <= centroMario);
      if (cruzo) {
        e.puntuado = true;
        dkPuntuacion += 100;
      }
    }
  }
  // Salida a la izquierda de la cuarta plataforma.
  if (fabNivel == 3 &&
      fabEnSuelo &&
      !fabSubiendo &&
      fabX <= 45) {
    dkEtapaActual = DK_ETAPA_ASCENSORES;
    dkPantallaEtapaPendiente = true;
    dkSaltoPendiente = false;
    Serial.println("DK: etapa 2 completada");
  }
}
uint16_t fabColorFondo(int x, int y) {
  if (x < 0 || x >= 240 || y < 0 || y >= 320) {
    return ILI9341_BLACK;
  }
  if (y == 31 && x >= 5 && x < 235) {
    return ILI9341_CYAN;
  }
  for (int n = 0; n < 4; ++n) {
    if (x >= 10 && x < 230 &&
        y >= FAB_Y[n] &&
        y < FAB_Y[n] + 8) {
      if (y == FAB_Y[n] + 2 &&
          ((x - 13 + 1700) % 17) < 8) {
        return n % 2 ? ILI9341_RED : ILI9341_YELLOW;
      }
      return ILI9341_DARKGREY;
    }
  }
  for (int n = 0; n < 3; ++n) {
    int ex = FAB_ESC_X[n];
    int superior = FAB_Y[n + 1] + 8;
    int inferior = FAB_Y[n];
    if (y >= superior && y < inferior) {
      if (x == ex - 7 || x == ex + 7) {
        return ILI9341_CYAN;
      }
      if (x >= ex - 7 && x <= ex + 7 &&
          y >= superior + 4 &&
          (y - superior - 4) % 9 == 0) {
        return ILI9341_CYAN;
      }
    }
  }
  // EXIT en el extremo izquierdo superior.
  if (x >= 15 && x < 48 &&
      y >= 65 && y < 89) {
    return ILI9341_GREEN;
  }
  return ILI9341_BLACK;
}
void fabCapturar(FabVisual &s) {
  s.x = fabX;
  s.y = fabY;
  s.vidas = vidasMario;
  s.visible = fabVisible;
  s.puntos = dkPuntuacion;
  for (int i = 0; i < FAB_ENEMIGOS; ++i) {
    s.enemigoActivo[i] = fabObstaculos[i].activo;
    s.enemigoX[i] = (int)roundf(fabObstaculos[i].x);
  }
}
uint16_t fabColorPixel(int x, int y, const FabVisual &s) {
  uint16_t color = fabColorFondo(x, y);
  for (int i = 0; i < FAB_ENEMIGOS; ++i) {
    if (!s.enemigoActivo[i]) {
      continue;
    }
    int dx = x - s.enemigoX[i];
    int dy = y - (FAB_Y[i] - 11);
    if (dx * dx + dy * dy <= 25) {
      color = ILI9341_ORANGE;
      if ((dx == -2 || dx == 2) && dy == -1) {
        color = ILI9341_BLACK;
      }
    }
  }
  if (s.visible &&
      x >= s.x &&
      x < s.x + FAB_MARIO_W &&
      y >= s.y &&
      y < s.y + FAB_MARIO_H) {
    const char* const* sprite =
      ((s.x / 5) & 1) ?
      DK_MARIO_PASO : DK_MARIO_QUIETO;
    char pixel = sprite[y - s.y][x - s.x];
    if (pixel != '.') {
      color = dkColorPixel(pixel);
    }
  }
  return color;
}
void fabDibujarRegion(
  int izquierda,
  int arriba,
  int derecha,
  int abajo,
  const FabVisual &s
) {
  izquierda = constrain(izquierda, 0, 240);
  derecha = constrain(derecha, 0, 240);
  arriba = constrain(arriba, 55, 295);
  abajo = constrain(abajo, 55, 295);
  if (izquierda >= derecha || arriba >= abajo) {
    return;
  }
  for (int y = arriba; y < abajo; y += BUFFER_ALTO) {
    int alto = min(BUFFER_ALTO, abajo - y);
    for (int x = izquierda; x < derecha; x += BUFFER_ANCHO) {
      int ancho = min(BUFFER_ANCHO, derecha - x);
      for (int yy = 0; yy < alto; ++yy) {
        for (int xx = 0; xx < ancho; ++xx) {
          bufferRegion[yy * ancho + xx] =
            fabColorPixel(x + xx, y + yy, s);
        }
      }
      tft.drawRGBBitmap(
        x, y, bufferRegion, ancho, alto
      );
    }
  }
}
void dkFabricaEscenario(const FabVisual &s) {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(8, 12);
  tft.print("DONKEY KONG - FABRICA 50M");
  fabDibujarRegion(0, 55, 240, 295, s);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(19, 73);
  tft.print("EXIT");
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(8, 301);
  tft.print("3S MENU");
  fabPantallaLista = true;
}
void dkFabricaDibujar() {
  if (estadoActual != JUEGO_DONKEY_KONG ||
      dkEtapaActual != DK_ETAPA_FABRICA) {
    return;
  }
  unsigned long ahora = millis();
  if (fabPantallaLista &&
      ahora - fabUltimoDibujo < FAB_FRAME) {
    return;
  }
  fabUltimoDibujo = ahora;
  FabVisual actual;
  if (dkMutex != nullptr &&
      xSemaphoreTake(dkMutex, pdMS_TO_TICKS(5)) != pdTRUE) {
    return;
  }
  fabCapturar(actual);
  if (dkMutex != nullptr) {
    xSemaphoreGive(dkMutex);
  }
  bool primerDibujo = !fabPantallaLista;
  if (primerDibujo) {
    dkFabricaEscenario(actual);
    fabAnteriorValido = true;
  } else if (!fabAnteriorValido) {
    fabDibujarRegion(0, 55, 240, 295, actual);
    fabAnteriorValido = true;
  } else {
    if (fabAnterior.x != actual.x ||
        fabAnterior.y != actual.y ||
        fabAnterior.visible != actual.visible) {
      int dx = abs(fabAnterior.x - actual.x);
      int dy = abs(fabAnterior.y - actual.y);
      if (dx <= FAB_MARIO_W && dy <= FAB_MARIO_H) {
        fabDibujarRegion(
          min(fabAnterior.x, actual.x) - 1,
          min(fabAnterior.y, actual.y) - 1,
          max(fabAnterior.x, actual.x) + FAB_MARIO_W + 1,
          max(fabAnterior.y, actual.y) + FAB_MARIO_H + 1,
          actual
        );
      } else {
        fabDibujarRegion(
          fabAnterior.x - 1,
          fabAnterior.y - 1,
          fabAnterior.x + FAB_MARIO_W + 1,
          fabAnterior.y + FAB_MARIO_H + 1,
          actual
        );
        fabDibujarRegion(
          actual.x - 1,
          actual.y - 1,
          actual.x + FAB_MARIO_W + 1,
          actual.y + FAB_MARIO_H + 1,
          actual
        );
      }
    }
    // Redibujado localizado de obstaculos.
    for (int i = 0; i < FAB_ENEMIGOS; ++i) {
      bool antes = fabAnterior.enemigoActivo[i];
      bool despues = actual.enemigoActivo[i];
      if (!antes && !despues) {
        continue;
      }
      if (antes && despues &&
          fabAnterior.enemigoX[i] == actual.enemigoX[i]) {
        continue;
      }
      int y = FAB_Y[i] - 11;
      int izquierda = 240;
      int derecha = 0;
      if (antes) {
        izquierda = min(
          izquierda, fabAnterior.enemigoX[i] - 7
        );
        derecha = max(
          derecha, fabAnterior.enemigoX[i] + 8
        );
      }
      if (despues) {
        izquierda = min(
          izquierda, actual.enemigoX[i] - 7
        );
        derecha = max(
          derecha, actual.enemigoX[i] + 8
        );
      }
      fabDibujarRegion(
        izquierda, y - 7,
        derecha, y + 8,
        actual
      );
    }
  }
  static int vidasAnteriores = -1;
  static uint32_t puntosAnteriores = 0xFFFFFFFF;
  if (vidasAnteriores != actual.vidas ||
      puntosAnteriores != actual.puntos ||
      primerDibujo) {
    tft.fillRect(6, 39, 228, 12, ILI9341_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(8, 40);
    tft.printf(
      "VIDAS:%d SCORE:%lu",
      actual.vidas,
      (unsigned long)actual.puntos
    );
    vidasAnteriores = actual.vidas;
    puntosAnteriores = actual.puntos;
  }
  fabAnterior = actual;
  // La fisica y los enemigos se habilitan solo despues
  // de finalizar el primer dibujo completo.
  if (primerDibujo) {
    if (dkMutex == nullptr ||
        xSemaphoreTake(dkMutex, portMAX_DELAY) == pdTRUE) {
      fabInicioJugable = millis();
      fabUltimoPaso = fabInicioJugable;
      for (int i = 0; i < FAB_ENEMIGOS; ++i)
        fabUltimoEnemigo[i] = fabInicioJugable;
      fabJugable = true;
      if (dkMutex != nullptr) xSemaphoreGive(dkMutex);
    }
  }
}
