#pragma once

const int MARIO_ANCHO = 14;
const int MARIO_ALTO = 18;
const int MARIO_MIN_X = 10;
const int MARIO_MAX_X = 216;
const int VELOCIDAD_MARIO = 2;

const unsigned long INTERVALO_MARIO = 20;
const unsigned long INTERVALO_FISICA = 20;
const unsigned long INTERVALO_ESCALERA = 30;

const float FUERZA_SALTO = -5.0f;
const float GRAVEDAD = 0.45f;

const int CANTIDAD_PLATAFORMAS = 4;
const int PLATAFORMA_IZQ = 10;
const int PLATAFORMA_DER = 229;
const int GROSOR_PLATAFORMA = 7;
const int INCLINACION = 12;
const int HUECO = 16;

const int plataformasBase[4] = {275, 215, 155, 95};
const int escalerasX[3] = {190, 38, 190};

const int VELOCIDAD_ESCALERA = 2;
const int TOLERANCIA_ESCALERA = 12;

const int MAX_BARRILES = 12;
const int RADIO_BARRIL = 6;
const int VELOCIDAD_BARRIL = 2;
const int SEPARACION_BARRILES = 55;

const unsigned long INTERVALO_BARRIL = 40;
const unsigned long INTERVALO_APARICION = 1500;

const int BUFFER_ANCHO = 32;
const int BUFFER_ALTO = 32;

uint16_t bufferRegion[BUFFER_ANCHO * BUFFER_ALTO];

const int VIDAS_INICIALES = 3;
const unsigned long TIEMPO_INVULNERABILIDAD = 1500;
const unsigned long INTERVALO_PARPADEO = 150;

struct Barril {
  bool activo;
  bool cayendo;
  int x;
  int y;
  int nivel;
  int direccion;
};

Barril barriles[MAX_BARRILES];

int marioX = 24;
float marioY = 257.0f;
float velocidadVertical = 0.0f;

bool marioEnSuelo = true;
bool marioEscalando = false;
bool marioVisible = true;

int plataformaActual = 0;
int plataformaDestino = 0;
int direccionEscalera = 0;

int vidasMario = VIDAS_INICIALES;
bool marioInvulnerable = false;
unsigned long inicioInvulnerabilidad = 0;

unsigned long ultimoMovimientoMario = 0;
unsigned long ultimaFisica = 0;
unsigned long ultimoMovimientoEscalera = 0;
unsigned long ultimoMovimientoBarriles = 0;
unsigned long ultimaAparicionBarril = 0;

int direccionPlataforma(int nivel) {
  return nivel % 2 == 1 ? 1 : -1;
}

int inicioPlataforma(int nivel) {
  return direccionPlataforma(nivel) < 0
    ? PLATAFORMA_IZQ + HUECO
    : PLATAFORMA_IZQ;
}

int finPlataforma(int nivel) {
  return direccionPlataforma(nivel) > 0
    ? PLATAFORMA_DER - HUECO
    : PLATAFORMA_DER;
}

int alturaPlataforma(int nivel, int x) {
  x = constrain(x, PLATAFORMA_IZQ, PLATAFORMA_DER);

  long desplazamiento =
    (long)(x - PLATAFORMA_IZQ) * INCLINACION /
    (PLATAFORMA_DER - PLATAFORMA_IZQ);

  if (direccionPlataforma(nivel) > 0) {
    return plataformasBase[nivel] -
           INCLINACION / 2 + desplazamiento;
  }

  return plataformasBase[nivel] +
         INCLINACION / 2 - desplazamiento;
}

int posicionMarioEnPlataforma(int nivel, int x) {
  return alturaPlataforma(
    nivel, x + MARIO_ANCHO / 2
  ) - MARIO_ALTO;
}

int posicionBarrilEnPlataforma(int nivel, int x) {
  return alturaPlataforma(nivel, x) - RADIO_BARRIL;
}

int escaleraSuperior(int indice) {
  return alturaPlataforma(
    indice + 1,
    escalerasX[indice] + MARIO_ANCHO / 2
  ) + GROSOR_PLATAFORMA;
}

int escaleraInferior(int indice) {
  return alturaPlataforma(
    indice,
    escalerasX[indice] + MARIO_ANCHO / 2
  );
}

bool pixelEscalera(int x, int y) {
  for (int i = 0; i < 3; i++) {
    int ex = escalerasX[i];
    int y1 = escaleraSuperior(i);
    int y2 = escaleraInferior(i);

    if (x < ex || x > ex + 14 ||
        y < y1 || y > y2) {
      continue;
    }

    bool lateral =
      y < y2 && (x == ex || x == ex + 14);

    bool peldano = (y - y1) % 8 == 0;

    if (lateral || peldano) {
      return true;
    }
  }

  return false;
}

uint16_t colorEscenario(int x, int y) {
  if (pixelEscalera(x, y)) {
    return ILI9341_CYAN;
  }

  int dx = x - 197;
  int dy = y - 72;

  if (dx >= -6 && dx <= 6 &&
      dy >= -6 && dy <= 6 &&
      dx * dx + dy * dy <= 36) {
    return ILI9341_YELLOW;
  }

  if (x >= 192 && x < 202 &&
      y >= 78 && y < 95) {
    return ILI9341_MAGENTA;
  }

  if (x >= 20 && x < 44 &&
      y >= 75 && y < 95) {
    int cx = x;
    int cy = y;
    bool esquina = false;

    if (x < 24 && y < 79) {
      cx = 24;
      cy = 79;
      esquina = true;
    } else if (x >= 40 && y < 79) {
      cx = 39;
      cy = 79;
      esquina = true;
    } else if (x < 24 && y >= 91) {
      cx = 24;
      cy = 90;
      esquina = true;
    } else if (x >= 40 && y >= 91) {
      cx = 39;
      cy = 90;
      esquina = true;
    }

    if (!esquina ||
        (x - cx) * (x - cx) +
        (y - cy) * (y - cy) <= 16) {
      return ILI9341_ORANGE;
    }
  }

  for (int i = 0; i < CANTIDAD_PLATAFORMAS; i++) {
    if (x < inicioPlataforma(i) ||
        x > finPlataforma(i)) {
      continue;
    }

    int altura = alturaPlataforma(i, x);

    if (y >= altura &&
        y < altura + GROSOR_PLATAFORMA) {
      return ILI9341_RED;
    }
  }

  return ILI9341_BLACK;
}

bool puntoMario(
  int x,
  int y,
  uint16_t &color
) {
  if (!marioVisible) return false;

  int lx = x - marioX;
  int ly = y - (int)marioY;

  if (lx < 0 || lx >= MARIO_ANCHO ||
      ly < 0 || ly >= MARIO_ALTO) {
    return false;
  }

  if (ly < 5) {
    color = ILI9341_RED;
    return true;
  }

  if (lx >= 1 && lx <= 12 && ly >= 4) {
    color = ILI9341_BLUE;
    return true;
  }

  return false;
}

bool puntoBarril(
  int x,
  int y,
  const Barril &b
) {
  if (!b.activo) return false;

  int dx = x - b.x;
  int dy = y - b.y;

  if (dx < -RADIO_BARRIL ||
      dx > RADIO_BARRIL ||
      dy < -RADIO_BARRIL ||
      dy > RADIO_BARRIL) {
    return false;
  }

  return dx * dx + dy * dy <=
         RADIO_BARRIL * RADIO_BARRIL;
}

void renderizarRegion(
  int izquierda,
  int arriba,
  int derecha,
  int abajo
) {
  izquierda = max(0, izquierda);
  arriba = max(55, arriba);
  derecha = min(240, derecha);
  abajo = min(295, abajo);

  if (izquierda >= derecha ||
      arriba >= abajo) {
    return;
  }

  for (int bloqueY = arriba;
       bloqueY < abajo;
       bloqueY += BUFFER_ALTO) {

    int alto = min(
      BUFFER_ALTO, abajo - bloqueY
    );

    for (int bloqueX = izquierda;
         bloqueX < derecha;
         bloqueX += BUFFER_ANCHO) {

      int ancho = min(
        BUFFER_ANCHO, derecha - bloqueX
      );

      int indices[MAX_BARRILES];
      int cantidad = 0;

      for (int i = 0; i < MAX_BARRILES; i++) {
        const Barril &b = barriles[i];

        if (!b.activo) continue;

        if (b.x + RADIO_BARRIL < bloqueX ||
            b.x - RADIO_BARRIL >= bloqueX + ancho ||
            b.y + RADIO_BARRIL < bloqueY ||
            b.y - RADIO_BARRIL >= bloqueY + alto) {
          continue;
        }

        indices[cantidad++] = i;
      }

      bool marioEnRegion =
        marioVisible &&
        marioX < bloqueX + ancho &&
        marioX + MARIO_ANCHO > bloqueX &&
        (int)marioY < bloqueY + alto &&
        (int)marioY + MARIO_ALTO > bloqueY;

      for (int fy = 0; fy < alto; fy++) {
        int y = bloqueY + fy;

        for (int fx = 0; fx < ancho; fx++) {
          int x = bloqueX + fx;

          uint16_t color = colorEscenario(x, y);

          for (int j = 0; j < cantidad; j++) {
            if (puntoBarril(
              x, y, barriles[indices[j]]
            )) {
              color = ILI9341_ORANGE;
              break;
            }
          }

          uint16_t colorPersonaje;

          if (marioEnRegion &&
              puntoMario(x, y, colorPersonaje)) {
            color = colorPersonaje;
          }

          bufferRegion[fy * ancho + fx] = color;
        }
      }

      tft.drawRGBBitmap(
        bloqueX,
        bloqueY,
        bufferRegion,
        ancho,
        alto
      );
    }
  }
}

void renderizarMarioActual() {
  renderizarRegion(
    marioX,
    (int)marioY,
    marioX + MARIO_ANCHO,
    (int)marioY + MARIO_ALTO
  );
}

void moverSpriteMario(
  int xAnterior,
  int yAnterior,
  int xNuevo,
  int yNuevo
) {
  int izquierda = min(xAnterior, xNuevo);
  int arriba = min(yAnterior, yNuevo);
  int derecha =
    max(xAnterior, xNuevo) + MARIO_ANCHO;
  int abajo =
    max(yAnterior, yNuevo) + MARIO_ALTO;

  marioX = xNuevo;
  marioY = (float)yNuevo;

  renderizarRegion(
    izquierda, arriba, derecha, abajo
  );
}

void actualizarMarcadorVidas() {
  tft.fillRect(
    175, 38, 65, 10,
    ILI9341_BLACK
  );

  tft.setTextSize(1);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(175, 38);
  tft.print("VIDAS: ");
  tft.print(vidasMario);
}

void reiniciarBarriles() {
  for (int i = 0; i < MAX_BARRILES; i++) {
    barriles[i].activo = false;
    barriles[i].cayendo = false;
    barriles[i].x = 0;
    barriles[i].y = 0;
    barriles[i].nivel = 0;
    barriles[i].direccion = 0;
  }

  ultimoMovimientoBarriles = millis();
  ultimaAparicionBarril = millis();
}

bool espacioLibre(
  int x,
  int y,
  int nivel,
  int ignorar
) {
  for (int i = 0; i < MAX_BARRILES; i++) {
    if (i == ignorar ||
        !barriles[i].activo) {
      continue;
    }

    const Barril &otro = barriles[i];

    if (otro.nivel != nivel ||
        otro.cayendo) {
      continue;
    }

    if (abs(x - otro.x) < SEPARACION_BARRILES &&
        abs(y - otro.y) <= 18) {
      return false;
    }
  }

  return true;
}

void crearBarril() {
  int xInicial = 55;
  int yInicial =
    posicionBarrilEnPlataforma(3, xInicial);

  if (!espacioLibre(
    xInicial, yInicial, 3, -1
  )) {
    return;
  }

  for (int i = 0; i < MAX_BARRILES; i++) {
    if (barriles[i].activo) continue;

    barriles[i].activo = true;
    barriles[i].cayendo = false;
    barriles[i].x = xInicial;
    barriles[i].y = yInicial;
    barriles[i].nivel = 3;
    barriles[i].direccion =
      direccionPlataforma(3);
    return;
  }
}

void moverBarril(
  Barril &b,
  int indice
) {
  if (!b.activo) return;

  if (b.cayendo) {
    if (b.nivel == 0) {
      b.y += VELOCIDAD_BARRIL;

      if (b.y - RADIO_BARRIL > 292) {
        b.activo = false;
      }

      return;
    }

    int siguienteNivel = b.nivel - 1;

    int yDestino =
      posicionBarrilEnPlataforma(
        siguienteNivel, b.x
      );

    if (b.y + VELOCIDAD_BARRIL >= yDestino) {
      if (espacioLibre(
        b.x, yDestino, siguienteNivel, indice
      )) {
        b.y = yDestino;
        b.nivel = siguienteNivel;
        b.direccion =
          direccionPlataforma(siguienteNivel);
        b.cayendo = false;
      } else {
        b.y = yDestino - RADIO_BARRIL * 2;
      }

      return;
    }

    b.y += VELOCIDAD_BARRIL;
    return;
  }

  int siguienteX =
    b.x + b.direccion * VELOCIDAD_BARRIL;

  if (!espacioLibre(
    siguienteX,
    posicionBarrilEnPlataforma(
      b.nivel, siguienteX
    ),
    b.nivel,
    indice
  )) {
    return;
  }

  b.x = siguienteX;
  b.y =
    posicionBarrilEnPlataforma(b.nivel, b.x);

  bool bordeDerecho =
    b.direccion > 0 &&
    b.x >= finPlataforma(b.nivel);

  bool bordeIzquierdo =
    b.direccion < 0 &&
    b.x <= inicioPlataforma(b.nivel);

  if (bordeDerecho || bordeIzquierdo) {
    b.cayendo = true;
  }
}

void actualizarBarriles() {
  unsigned long ahora = millis();

  bool generar =
    ahora - ultimaAparicionBarril >=
    INTERVALO_APARICION;

  bool mover =
    ahora - ultimoMovimientoBarriles >=
    INTERVALO_BARRIL;

  if (!generar && !mover) return;

  Barril anteriores[MAX_BARRILES];

  for (int i = 0; i < MAX_BARRILES; i++) {
    anteriores[i] = barriles[i];
  }

  if (generar) {
    ultimaAparicionBarril = ahora;
    crearBarril();
  }

  if (mover) {
    ultimoMovimientoBarriles = ahora;

    for (int i = 0; i < MAX_BARRILES; i++) {
      moverBarril(barriles[i], i);
    }
  }

  for (int i = 0; i < MAX_BARRILES; i++) {
    const Barril &antes = anteriores[i];
    const Barril &despues = barriles[i];

    if (!antes.activo &&
        !despues.activo) {
      continue;
    }

    if (antes.activo &&
        despues.activo &&
        antes.x == despues.x &&
        antes.y == despues.y) {
      continue;
    }

    int izquierda = 240;
    int arriba = 320;
    int derecha = 0;
    int abajo = 0;

    if (antes.activo) {
      izquierda = min(
        izquierda,
        antes.x - RADIO_BARRIL
      );
      arriba = min(
        arriba,
        antes.y - RADIO_BARRIL
      );
      derecha = max(
        derecha,
        antes.x + RADIO_BARRIL + 1
      );
      abajo = max(
        abajo,
        antes.y + RADIO_BARRIL + 1
      );
    }

    if (despues.activo) {
      izquierda = min(
        izquierda,
        despues.x - RADIO_BARRIL
      );
      arriba = min(
        arriba,
        despues.y - RADIO_BARRIL
      );
      derecha = max(
        derecha,
        despues.x + RADIO_BARRIL + 1
      );
      abajo = max(
        abajo,
        despues.y + RADIO_BARRIL + 1
      );
    }

    renderizarRegion(
      izquierda,
      arriba,
      derecha,
      abajo
    );
  }
}

void iniciarSalto() {
  if (!marioEnSuelo ||
      marioEscalando) {
    return;
  }

  velocidadVertical = FUERZA_SALTO;
  marioEnSuelo = false;
  ultimaFisica = millis();
}

bool iniciarEscalada(int direccion) {
  if (!marioEnSuelo ||
      marioEscalando) {
    return false;
  }

  int indiceEscalera;
  int destino;

  if (direccion > 0) {
    if (plataformaActual >=
        CANTIDAD_PLATAFORMAS - 1) {
      return false;
    }

    indiceEscalera = plataformaActual;
    destino = plataformaActual + 1;

  } else if (direccion < 0) {
    if (plataformaActual <= 0) {
      return false;
    }

    indiceEscalera = plataformaActual - 1;
    destino = plataformaActual - 1;

  } else {
    return false;
  }

  int posicionEscalera =
    escalerasX[indiceEscalera];

  if (abs(marioX - posicionEscalera) >
      TOLERANCIA_ESCALERA) {
    return false;
  }

  int xAnterior = marioX;
  int yAnterior = (int)marioY;

  moverSpriteMario(
    xAnterior,
    yAnterior,
    posicionEscalera,
    yAnterior
  );

  marioEscalando = true;
  marioEnSuelo = false;

  plataformaDestino = destino;
  direccionEscalera = direccion;

  velocidadVertical = 0.0f;
  ultimoMovimientoEscalera = millis();

  return true;
}

void actualizarEscalada() {
  if (!marioEscalando) return;

  unsigned long ahora = millis();

  if (ahora - ultimoMovimientoEscalera <
      INTERVALO_ESCALERA) {
    return;
  }

  ultimoMovimientoEscalera = ahora;

  int yAnterior = (int)marioY;
  int yDestino =
    posicionMarioEnPlataforma(
      plataformaDestino, marioX
    );

  int yNuevo = yAnterior;

  if (direccionEscalera > 0) {
    yNuevo -= VELOCIDAD_ESCALERA;

    if (yNuevo <= yDestino) {
      yNuevo = yDestino;
    }
  } else {
    yNuevo += VELOCIDAD_ESCALERA;

    if (yNuevo >= yDestino) {
      yNuevo = yDestino;
    }
  }

  if (yNuevo != yAnterior) {
    moverSpriteMario(
      marioX,
      yAnterior,
      marioX,
      yNuevo
    );
  }

  if (yNuevo == yDestino) {
    plataformaActual = plataformaDestino;
    marioEscalando = false;
    marioEnSuelo = true;
    direccionEscalera = 0;
    velocidadVertical = 0.0f;

    ultimaFisica = ahora;
    ultimoMovimientoMario = ahora;
  }
}

void actualizarMario() {
  if (marioEscalando) {
    actualizarEscalada();
    return;
  }

  unsigned long ahora = millis();

  if (marioEnSuelo) {
    int valorY = analogRead(JOY_Y);

    if (valorY > UMBRAL_ARRIBA) {
      if (iniciarEscalada(1)) return;
    } else if (valorY < UMBRAL_ABAJO) {
      if (iniciarEscalada(-1)) return;
    }
  }

  bool moverHorizontal =
    ahora - ultimoMovimientoMario >=
    INTERVALO_MARIO;

  bool moverVertical =
    ahora - ultimaFisica >=
    INTERVALO_FISICA;

  if (!moverHorizontal &&
      !moverVertical) {
    return;
  }

  int xAnterior = marioX;
  int yAnterior = (int)marioY;

  int nuevaX = marioX;
  float nuevaY = marioY;

  if (moverHorizontal) {
    ultimoMovimientoMario = ahora;

    int valorX = analogRead(JOY_X);

    if (valorX > 3000) {
      nuevaX -= VELOCIDAD_MARIO;
    } else if (valorX < 1000) {
      nuevaX += VELOCIDAD_MARIO;
    }

    nuevaX = constrain(
      nuevaX,
      MARIO_MIN_X,
      MARIO_MAX_X
    );
  }

  if (moverVertical) {
    ultimaFisica = ahora;

    if (!marioEnSuelo) {
      nuevaY += velocidadVertical;
      velocidadVertical += GRAVEDAD;
    }
  }

  if (marioEnSuelo) {
    nuevaY =
      posicionMarioEnPlataforma(
        plataformaActual, nuevaX
      );
  } else {
    int suelo =
      posicionMarioEnPlataforma(
        plataformaActual, nuevaX
      );

    if (velocidadVertical >= 0 &&
        nuevaY >= suelo) {
      nuevaY = suelo;
      velocidadVertical = 0.0f;
      marioEnSuelo = true;
    }
  }

  int yNuevo = (int)nuevaY;

  if (nuevaX != xAnterior ||
      yNuevo != yAnterior) {
    moverSpriteMario(
      xAnterior,
      yAnterior,
      nuevaX,
      yNuevo
    );
  }

  marioY = nuevaY;
}

bool colisionMarioBarril(const Barril &b) {
  if (!b.activo) return false;

  int izquierda = marioX + 1;
  int derecha = marioX + MARIO_ANCHO - 2;
  int arriba = (int)marioY;
  int abajo = (int)marioY + MARIO_ALTO - 1;

  int cercanoX = constrain(
    b.x, izquierda, derecha
  );

  int cercanoY = constrain(
    b.y, arriba, abajo
  );

  long dx = b.x - cercanoX;
  long dy = b.y - cercanoY;

  return dx * dx + dy * dy <=
         RADIO_BARRIL * RADIO_BARRIL;
}

void dibujarGameOver() {
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(3);
  tft.setCursor(30, 85);
  tft.print("GAME OVER");

  tft.drawFastHLine(
    30, 135, 180, ILI9341_RED
  );

  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(53, 165);
  tft.print("SIN VIDAS");

  tft.setTextSize(1);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(38, 235);
  tft.print("BOTON: REINTENTAR");

  tft.setTextColor(ILI9341_CYAN);
  tft.setCursor(42, 255);
  tft.print("MANTENER 3S: MENU");
}

void reiniciarPosicionMario() {
  int xAnterior = marioX;
  int yAnterior = (int)marioY;

  marioX = 24;
  plataformaActual = 0;
  plataformaDestino = 0;
  marioY =
    posicionMarioEnPlataforma(0, marioX);

  velocidadVertical = 0.0f;
  marioEnSuelo = true;
  marioEscalando = false;
  direccionEscalera = 0;

  marioVisible = true;

  unsigned long ahora = millis();

  ultimoMovimientoMario = ahora;
  ultimaFisica = ahora;
  ultimoMovimientoEscalera = ahora;

  renderizarRegion(
    xAnterior,
    yAnterior,
    xAnterior + MARIO_ANCHO,
    yAnterior + MARIO_ALTO
  );

  renderizarMarioActual();
}

void recibirImpacto() {
  if (marioInvulnerable ||
      estadoActual != JUEGO_DONKEY_KONG) {
    return;
  }

  vidasMario--;
  actualizarMarcadorVidas();

  Serial.print("Impacto. Vidas restantes: ");
  Serial.println(vidasMario);

  if (vidasMario <= 0) {
    vidasMario = 0;
    estadoActual = PANTALLA_GAME_OVER;
    dibujarGameOver();
    return;
  }

  reiniciarPosicionMario();

  marioInvulnerable = true;
  inicioInvulnerabilidad = millis();
}

void comprobarColisiones() {
  if (marioInvulnerable) return;

  for (int i = 0; i < MAX_BARRILES; i++) {
    if (colisionMarioBarril(barriles[i])) {
      recibirImpacto();
      return;
    }
  }
}

void actualizarInvulnerabilidad() {
  if (!marioInvulnerable) return;

  unsigned long transcurrido =
    millis() - inicioInvulnerabilidad;

  bool nuevaVisibilidad;

  if (transcurrido >=
      TIEMPO_INVULNERABILIDAD) {
    marioInvulnerable = false;
    nuevaVisibilidad = true;
  } else {
    nuevaVisibilidad =
      (transcurrido / INTERVALO_PARPADEO) % 2 == 0;
  }

  if (nuevaVisibilidad != marioVisible) {
    marioVisible = nuevaVisibilidad;
    renderizarMarioActual();
  }
}

void dibujarDonkeyKong() {
  tft.fillScreen(ILI9341_BLACK);

  marioX = 24;
  plataformaActual = 0;
  plataformaDestino = 0;
  marioY =
    posicionMarioEnPlataforma(0, marioX);

  velocidadVertical = 0.0f;
  marioEnSuelo = true;
  marioEscalando = false;
  marioVisible = true;
  direccionEscalera = 0;

  vidasMario = VIDAS_INICIALES;
  marioInvulnerable = false;
  inicioInvulnerabilidad = 0;

  ultimoMovimientoMario = millis();
  ultimaFisica = millis();
  ultimoMovimientoEscalera = millis();

  reiniciarBarriles();

  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(48, 12);
  tft.print("DONKEY KONG");

  tft.setTextSize(1);
  tft.setTextColor(ILI9341_WHITE);

  tft.setCursor(10, 38);
  tft.print("SCORE: 0000");

  actualizarMarcadorVidas();

  for (int nivel = 0;
       nivel < CANTIDAD_PLATAFORMAS;
       nivel++) {

    for (int x = inicioPlataforma(nivel);
         x <= finPlataforma(nivel);
         x++) {

      int y = alturaPlataforma(nivel, x);

      tft.drawFastVLine(
        x,
        y,
        GROSOR_PLATAFORMA,
        ILI9341_RED
      );
    }
  }

  for (int i = 0; i < 3; i++) {
    int y1 = escaleraSuperior(i);
    int y2 = escaleraInferior(i);

    tft.drawFastVLine(
      escalerasX[i],
      y1,
      y2 - y1,
      ILI9341_CYAN
    );

    tft.drawFastVLine(
      escalerasX[i] + 14,
      y1,
      y2 - y1,
      ILI9341_CYAN
    );

    for (int y = y1; y <= y2; y += 8) {
      tft.drawFastHLine(
        escalerasX[i],
        y,
        15,
        ILI9341_CYAN
      );
    }
  }

  tft.fillRoundRect(
    20, 75, 24, 20, 4,
    ILI9341_ORANGE
  );

  tft.fillCircle(
    197, 72, 6,
    ILI9341_YELLOW
  );

  tft.fillRect(
    192, 78, 10, 17,
    ILI9341_MAGENTA
  );

  renderizarMarioActual();

  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.setCursor(48, 305);
  tft.print("MANTENER 3S: MENU");
}

void actualizarDonkeyKong() {
  actualizarMario();
  actualizarBarriles();

  actualizarInvulnerabilidad();
  comprobarColisiones();
}


void dkIniciar() {
  estadoActual = JUEGO_DONKEY_KONG;
  dibujarDonkeyKong();
}

void dkActualizar() {
  actualizarDonkeyKong();
}

void dkBotonCorto() {
  if (estadoActual == JUEGO_DONKEY_KONG) {
    iniciarSalto();
  } else if (estadoActual == PANTALLA_GAME_OVER) {
    dkIniciar();
  }
}

