#pragma once

//funciones laberinto

const int PM_COLUMNAS = 28;
const int PM_FILAS = 27;
const int PM_CASILLA = 8;                 
const int PM_OFFSET_X = 8;                
const int PM_OFFSET_Y = 40;               
const int PM_ANCHO = PM_COLUMNAS * PM_CASILLA;  
const int PM_FILA_TUNEL = 14;             

//codigos de las casillas
const char PM_PARED = '#';
const char PM_PUNTO = '.';
const char PM_PODER = 'o';
const char PM_VACIO = ' ';
const char PM_PUERTA = '-';

const uint16_t PM_COLOR_PARED = 0x3A9F;

//un laberinto por nivel
const int PM_CANT_LABERINTOS = 5;
const char LABERINTOS[PM_CANT_LABERINTOS][PM_FILAS][PM_COLUMNAS + 1] = {
  //nivel 1
    "############################",
    "#o........................o#",
    "#.#####.############.#####.#",
    "#.#####.############.#####.#",
    "#..........................#",
    "#.####.###.######.###.####.#",
    "#.####.###.######.###.####.#",
    "#..........................#",
    "#.####.##.########.##.####.#",
    "#.####.##.########.##.####.#",
    "#.####.##.########.##.####.#",
    "#......##..........##......#",
    "#.####.##.###--###.##.####.#",
    "#.####.##.#      #.##.####.#",
    "#.####.##.#      #.##.####.#",
    "#.####.##.#      #.##.####.#",
    "#.####.##.########.##.####.#",
    "#..........................#",
    "#.####.###.######.###.####.#",
    "#.####.###.######.###.####.#",
    "#..........................#",
    "#.#####.############.#####.#",
    "#.#####.############.#####.#",
    "#o#####.############.#####o#",
    "#.#####.############.#####.#",
    "#..........................#",
    "############################"
  },
  //nivel 2
  {
    "############################",
    "#..........................#",
    "#.####.##############.####.#",
    "#o####.##############.####o#",
    "#.####.##############.####.#",
    "#..........................#",
    "#.####.##.########.##.####.#",
    "#.####.##.########.##.####.#",
    "#......##..........##......#",
    "######.##.########.##.######",
    "######.##.########.##.######",
    "######.##..........##.######",
    "######.##.###--###.##.######",
    "######.##.#      #.##.######",
    "      .   #      #   .      ",
    "######.##.#      #.##.######",
    "######.##.########.##.######",
    "######.##..........##.######",
    "######.##.########.##.######",
    "#..........................#",
    "#.####.##############.####.#",
    "#o####.##############.####o#",
    "#.####.##############.####.#",
    "#..........................#",
    "#.########################.#",
    "#..........................#",
    "############################"
  },
  //nivel 3
  {
    "############################",
    "#............##............#",
    "#.####.#####.##.#####.####.#",
    "#o####.#####.##.#####.####o#",
    "#.####.#####.##.#####.####.#",
    "#..........................#",
    "#.####.##.########.##.####.#",
    "#.####.##.########.##.####.#",
    "#......##....##....##......#",
    "######.#####.##.#####.######",
    "######.#####.##.#####.######",
    "######.##..........##.######",
    "######.##.###--###.##.######",
    "######.##.#      #.##.######",
    "      .   #      #   .      ",
    "######.##.#      #.##.######",
    "######.##.########.##.######",
    "######.##..........##.######",
    "######.##.########.##.######",
    "#............##............#",
    "#.####.#####.##.#####.####.#",
    "#o..##................##..o#",
    "###.##.##.########.##.##.###",
    "#......##....##....##......#",
    "#.##########.##.##########.#",
    "#..........................#",
    "############################"
  },
  //nivel 4
  {
    "############################",
    "#......#............#......#",
    "#.####.#.####..####.#.####.#",
    "#o####.#.####..####.#.####o#",
    "#..........................#",
    "#.##.#####.######.#####.##.#",
    "#.##.#####.######.#####.##.#",
    "#....##......##......##....#",
    "####.##.####.##.####.##.####",
    "####.##.####.##.####.##.####",
    "#..........................#",
    "#.####.##..........##.####.#",
    "#.####.##.###--###.##.####.#",
    "#......##.#      #.##......#",
    "####.#.##.#      #.##.#.####",
    "####.#.##.#      #.##.#.####",
    "#....#.##.########.##.#....#",
    "#.##...##..........##...##.#",
    "#.##.####.########.####.##.#",
    "#....#.......##.......#....#",
    "####.#.#####.##.#####.#.####",
    "#o.........#....#.........o#",
    "#.####.###.#.##.#.###.####.#",
    "#.####.###...##...###.####.#",
    "#.####.#####.##.#####.####.#",
    "#..........................#",
    "############################"
  },
  //nivel 5
  {
    "############################",
    "#.....#......##......#.....#",
    "#.###.#.####.##.####.#.###.#",
    "#o###.#.####.##.####.#.###o#",
    "#............##............#",
    "#.###.##.##.####.##.##.###.#",
    "#.....##.##......##.##.....#",
    "#####.##.####..####.##.#####",
    "#####.##.####..####.##.#####",
    "#..........................#",
    "#.#####.#.###..###.#.#####.#",
    "#.#####.#..........#.#####.#",
    "#.....#.#.###--###.#.#.....#",
    "#####.#.#.#      #.#.#.#####",
    "      .   #      #   .      ",
    "#####.###.#      #.###.#####",
    "#.....###.########.###.....#",
    "#.###.....#......#.....###.#",
    "#.###.###.#.####.#.###.###.#",
    "#.....###..........###.....#",
    "###.#######.####.#######.###",
    "#o..#.......#..#.......#..o#",
    "#.#.#.#####.#..#.#####.#.#.#",
    "#.#...#.....#..#.....#...#.#",
    "#.#####.#####..#####.#####.#",
    "#..........................#",
    "############################"
  }
};
char laberinto[PM_FILAS][PM_COLUMNAS];
int puntosRestantes = 0;

//funciones de direccion

enum DireccionPM { PM_ARRIBA, PM_IZQUIERDA, PM_ABAJO, PM_DERECHA, PM_QUIETO };

const int PM_DX[5] = { 0, -1, 0, 1, 0 };
const int PM_DY[5] = { -1, 0, 1, 0, 0 };

DireccionPM direccionOpuesta(DireccionPM d) {
  if (d == PM_QUIETO) return PM_QUIETO;
  return (DireccionPM)((d + 2) % 4);
}

//funciones nivel

struct ConfigNivel {
  unsigned long intervaloPacman;     
  unsigned long intervaloFantasma;
  unsigned long intervaloAsustado;
  unsigned long duracionAsustado;    
  int cantidadFantasmas;             
};

const ConfigNivel NIVELES[] = {
  {   16,      19,       32,      7000,      2 },   //1
  {   15,      17,       30,      6000,      3 },   //2
  {   15,      16,       28,      5000,      3 },   //3
  {   14,      15,       26,      4000,      4 },   //4
  {   13,      14,       24,      3000,      4 },   //5
};
const int CANTIDAD_NIVELES = sizeof(NIVELES) / sizeof(NIVELES[0]);

const ConfigNivel &configActual(int nivel) {
  return NIVELES[min(nivel, CANTIDAD_NIVELES - 1)];
}

//funciones de estado y animaciones

enum EstadoPacman {
  PM_LISTO,            
  PM_JUGANDO,
  PM_PAUSA,
  PM_MUERTE,           
  PM_NIVEL_COMPLETO,   
  PM_GAME_OVER,
  PM_VICTORIA          //se completaron todos los niveles
};

EstadoPacman estadoPacman = PM_LISTO;
unsigned long inicioEstadoPM = 0;  

const unsigned long PM_TIEMPO_LISTO = 2000;
const unsigned long PM_TIEMPO_MUERTE = 1500;
const unsigned long PM_TIEMPO_NIVEL = 2500;

//funciones personajes

struct PacmanPM {
  int x, y;
  DireccionPM direccion;    
  DireccionPM deseada;      
  bool bocaAbierta;
  unsigned long ultimoMovimiento;
};

enum ModoFantasma {
  F_EN_CASA,       
  F_DISPERSION,    
  F_PERSECUCION,   
  F_ASUSTADO,      
  F_COMIDO         
};

struct Fantasma {
  int x, y;
  DireccionPM direccion;
  ModoFantasma modo;
  uint16_t color;
  int esquinaCol, esquinaFila;     
  unsigned long tiempoSalida;     
  unsigned long ultimoMovimiento;
};

const int MAX_FANTASMAS = 4;
PacmanPM pacman;
Fantasma fantasmas[MAX_FANTASMAS];

const int PM_INICIO_COL = 13, PM_INICIO_FILA = 17;   
const int PM_SALIDA_COL = 13, PM_SALIDA_FILA = 11;   

const unsigned long PM_TIEMPO_DISPERSION = 7000;
const unsigned long PM_TIEMPO_PERSECUCION = 20000;
bool modoGlobalPersecucion = false;
unsigned long inicioModoGlobal = 0;
unsigned long finAsustado = 0;
int fantasmasComidosSeguidos = 0;  //200 - 400 - 800 - 1600 puntos

//funcion partida y estadisticas para ampliacion del final


struct EstadisticasPartida {
  unsigned long puntaje;
  int nivel;            // 0 = nivel 1
  int vidas;
  int fantasmasComidos;
  unsigned long inicioPartida;
};

EstadisticasPartida partidaPM;
unsigned long puntajeDibujado = 0xFFFFFFFF;  

void reportarEventoPacman(const char *evento) {
  Serial.printf(
    "{\"juego\":\"pacman\",\"evento\":\"%s\",\"puntaje\":%lu,"
    "\"nivel\":%d,\"vidas\":%d,\"fantasmasComidos\":%d,\"segundos\":%lu}\n",
    evento, partidaPM.puntaje, partidaPM.nivel + 1, partidaPM.vidas,
    partidaPM.fantasmasComidos, (millis() - partidaPM.inicioPartida) / 1000
  );
}

//funciones grilla

int pisoCasilla(int v) {
  return v >= 0 ? v / PM_CASILLA : -((-v + PM_CASILLA - 1) / PM_CASILLA);
}

bool enCentroDeCasilla(int x, int y) {
  return x % PM_CASILLA == PM_CASILLA / 2 && y % PM_CASILLA == PM_CASILLA / 2;
}


char casilla(int col, int fila) {
  if (fila < 0 || fila >= PM_FILAS) return PM_PARED;
  if (col < 0 || col >= PM_COLUMNAS) {
    if (fila != PM_FILA_TUNEL) return PM_PARED;
    col = (col + PM_COLUMNAS) % PM_COLUMNAS;
  }
  return laberinto[fila][col];
}

bool esTransitable(int col, int fila) {
  char c = casilla(col, fila);
  return c != PM_PARED && c != PM_PUERTA;
}

bool puedeMoverse(int col, int fila, DireccionPM d) {
  if (d == PM_QUIETO) return false;
  return esTransitable(col + PM_DX[d], fila + PM_DY[d]);
}

void avanzarPixel(int &x, int &y, DireccionPM d) {
  x += PM_DX[d];
  y += PM_DY[d];
  if (x < 0) x += PM_ANCHO;
  if (x >= PM_ANCHO) x -= PM_ANCHO;
}

//funciones dinujo tablero

int pantallaX(int x) { return PM_OFFSET_X + x; }
int pantallaY(int y) { return PM_OFFSET_Y + y; }

void dibujarCasilla(int col, int fila) {
  int px = PM_OFFSET_X + col * PM_CASILLA;
  int py = PM_OFFSET_Y + fila * PM_CASILLA;

  if (col < 0 || col >= PM_COLUMNAS || fila < 0 || fila >= PM_FILAS) {
    if (px >= 0 && px + PM_CASILLA <= 240) {
      tft.fillRect(px, py, PM_CASILLA, PM_CASILLA, ILI9341_BLACK);
    }
    return;
  }

  tft.fillRect(px, py, PM_CASILLA, PM_CASILLA, ILI9341_BLACK);
  char c = laberinto[fila][col];

  if (c == PM_PARED) {
    if (fila > 0 && laberinto[fila - 1][col] != PM_PARED)
      tft.fillRect(px, py + 1, PM_CASILLA, 2, PM_COLOR_PARED);
    if (fila < PM_FILAS - 1 && laberinto[fila + 1][col] != PM_PARED)
      tft.fillRect(px, py + PM_CASILLA - 3, PM_CASILLA, 2, PM_COLOR_PARED);
    if (col > 0 && laberinto[fila][col - 1] != PM_PARED)
      tft.fillRect(px + 1, py, 2, PM_CASILLA, PM_COLOR_PARED);
    if (col < PM_COLUMNAS - 1 && laberinto[fila][col + 1] != PM_PARED)
      tft.fillRect(px + PM_CASILLA - 3, py, 2, PM_CASILLA, PM_COLOR_PARED);
  } else if (c == PM_PUNTO) {
    tft.fillRect(px + 3, py + 3, 2, 2, ILI9341_WHITE);
  } else if (c == PM_PODER) {
    tft.fillCircle(px + 4, py + 4, 3, ILI9341_WHITE);
  } else if (c == PM_PUERTA) {
    tft.drawFastHLine(px, py + 4, PM_CASILLA, ILI9341_PINK);
  }
}

void dibujarLaberinto() {
  for (int f = 0; f < PM_FILAS; f++)
    for (int c = 0; c < PM_COLUMNAS; c++)
      dibujarCasilla(c, f);
}


void restaurarFondo(int x, int y) {
  int c1 = pisoCasilla(x - 4), c2 = pisoCasilla(x + 3);
  int f1 = pisoCasilla(y - 4), f2 = pisoCasilla(y + 3);
  for (int f = f1; f <= f2; f++)
    for (int c = c1; c <= c2; c++)
      dibujarCasilla(c, f);
}

void dibujarPacmanSprite() {
  int sx = pantallaX(pacman.x), sy = pantallaY(pacman.y);
  tft.fillCircle(sx, sy, 3, ILI9341_YELLOW);

  if (pacman.bocaAbierta && pacman.direccion != PM_QUIETO) {
    int dx = PM_DX[pacman.direccion], dy = PM_DY[pacman.direccion];
    tft.fillTriangle(
      sx, sy,
      sx + dx * 4 - dy * 3, sy + dy * 4 - dx * 3,
      sx + dx * 4 + dy * 3, sy + dy * 4 + dx * 3,
      ILI9341_BLACK
    );
  }
}

void dibujarFantasma(const Fantasma &g) {
  int sx = pantallaX(g.x), sy = pantallaY(g.y);
  bool parpadeo = g.modo == F_ASUSTADO &&
                  finAsustado - millis() < 2000 && (millis() / 200) % 2;

  if (g.modo != F_COMIDO) {
    uint16_t color = g.color;
    if (g.modo == F_ASUSTADO) color = parpadeo ? ILI9341_WHITE : ILI9341_BLUE;

    tft.fillCircle(sx, sy - 1, 3, color);                 
    tft.fillRect(sx - 3, sy - 1, 7, 4, color);            
    tft.drawPixel(sx - 2, sy + 3, color);                 
    tft.drawPixel(sx, sy + 3, color);
    tft.drawPixel(sx + 2, sy + 3, color);
  }

  if (g.modo != F_ASUSTADO) {
    tft.drawPixel(sx - 1, sy - 1, ILI9341_WHITE);
    tft.drawPixel(sx + 1, sy - 1, ILI9341_WHITE);
  }
}

void dibujarPersonajes() {
  for (int i = 0; i < configActual(partidaPM.nivel).cantidadFantasmas; i++)
    dibujarFantasma(fantasmas[i]);
  dibujarPacmanSprite();
}

void dibujarMarcador() {
  if (partidaPM.puntaje == puntajeDibujado) return;
  puntajeDibujado = partidaPM.puntaje;

  tft.setTextSize(1);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);  
  tft.setCursor(10, 25);
  tft.printf("SCORE: %06lu", partidaPM.puntaje);
}

void dibujarVidasYNivel() {
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setCursor(130, 25);
  tft.printf("NIVEL:%d VIDAS:%d", partidaPM.nivel + 1, partidaPM.vidas);
}

void mensajeInferior(const char *texto, uint16_t color) {
  tft.fillRect(0, 272, 240, 30, ILI9341_BLACK);
  if (texto[0] == '\0') return;
  tft.setTextSize(2);
  tft.setTextColor(color);
  int ancho = strlen(texto) * 12;
  tft.setCursor((240 - ancho) / 2, 280);
  tft.print(texto);
}

//funciones de musica

void sonidoComer() {
  static bool alterna = false;           
  alterna = !alterna;
  tone(BUZZER, alterna ? 500 : 700, 25);
}

//funciones iniciales

void cargarLaberinto() {
  //cada nivel usa su laberinto 
  int indice = min(partidaPM.nivel, PM_CANT_LABERINTOS - 1);
  puntosRestantes = 0;
  for (int f = 0; f < PM_FILAS; f++) {
    for (int c = 0; c < PM_COLUMNAS; c++) {
      laberinto[f][c] = LABERINTOS[indice][f][c];
      if (laberinto[f][c] == PM_PUNTO || laberinto[f][c] == PM_PODER)
        puntosRestantes++;
    }
  }
}

//reubica los personajes si pierde o pasa
void reubicarPersonajes() {
  unsigned long ahora = millis();

  pacman.x = PM_INICIO_COL * PM_CASILLA + 4;
  pacman.y = PM_INICIO_FILA * PM_CASILLA + 4;
  pacman.direccion = PM_QUIETO;
  pacman.deseada = PM_QUIETO;
  pacman.bocaAbierta = true;
  pacman.ultimoMovimiento = ahora;

  //info de fantasma
  const uint16_t colores[MAX_FANTASMAS] = {
    ILI9341_RED, ILI9341_PINK, ILI9341_CYAN, ILI9341_ORANGE
  };
  const int esquinas[MAX_FANTASMAS][2] = {
    { 25, -2 }, { 2, -2 }, { 27, 28 }, { 0, 28 }
  };
  const int casaCol[MAX_FANTASMAS] = { 13, 12, 15, 14 };

  for (int i = 0; i < MAX_FANTASMAS; i++) {
    Fantasma &g = fantasmas[i];
    g.color = colores[i];
    g.esquinaCol = esquinas[i][0];
    g.esquinaFila = esquinas[i][1];
    g.tiempoSalida = i * 3000UL;   
    g.direccion = PM_IZQUIERDA;
    g.ultimoMovimiento = ahora;

    if (i == 0) {
      g.modo = F_DISPERSION;
      g.x = PM_SALIDA_COL * PM_CASILLA + 4;
      g.y = PM_SALIDA_FILA * PM_CASILLA + 4;
    } else {
      g.modo = F_EN_CASA;
      g.x = casaCol[i] * PM_CASILLA + 4;
      g.y = 14 * PM_CASILLA + 4;
    }
  }

  modoGlobalPersecucion = false;
  inicioModoGlobal = ahora;
  fantasmasComidosSeguidos = 0;
}

void cambiarEstadoPacman(EstadoPacman nuevo) {
  estadoPacman = nuevo;
  inicioEstadoPM = millis();
}

//dibujo de tablero 
void prepararNivel() {
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(78, 4);
  tft.print("PAC-MAN");

  puntajeDibujado = 0xFFFFFFFF;
  dibujarMarcador();
  dibujarVidasYNivel();
  dibujarLaberinto();
  reubicarPersonajes();
  dibujarPersonajes();

  tft.setTextSize(1);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(68, 308);
  tft.print("MANTENER 3S: MENU");

  mensajeInferior("LISTO!", ILI9341_YELLOW);
  cambiarEstadoPacman(PM_LISTO);
}

//Funciones pacman

DireccionPM leerJoystickPM() {
  int x = analogRead(JOY_X);
  int y = analogRead(JOY_Y);
  int desvioX = abs(x - 2048);
  int desvioY = abs(y - 2048);

  if (desvioX >= desvioY) {           
    //gana el eje mas inclinado
    if (x > UMBRAL_ARRIBA) return PM_IZQUIERDA;
    if (x < UMBRAL_ABAJO) return PM_DERECHA;
  } else {
    if (y > UMBRAL_ARRIBA) return PM_ARRIBA;
    if (y < UMBRAL_ABAJO) return PM_ABAJO;
  }
  return PM_QUIETO;
}

void sumarPuntos(unsigned long puntos) {
  partidaPM.puntaje += puntos;
  dibujarMarcador();
}

void activarModoAsustado() {
  unsigned long ahora = millis();
  finAsustado = ahora + configActual(partidaPM.nivel).duracionAsustado;
  fantasmasComidosSeguidos = 0;

  for (int i = 0; i < configActual(partidaPM.nivel).cantidadFantasmas; i++) {
    Fantasma &g = fantasmas[i];
    if (g.modo == F_DISPERSION || g.modo == F_PERSECUCION) {
      g.modo = F_ASUSTADO;
      g.direccion = direccionOpuesta(g.direccion);  
    }
  }
}

//come lo que haya
void comerCasilla(int col, int fila) {
  char c = laberinto[fila][col];
  if (c != PM_PUNTO && c != PM_PODER) return;

  laberinto[fila][col] = PM_VACIO;
  puntosRestantes--;

  if (c == PM_PUNTO) {
    sumarPuntos(10);
    sonidoComer();
  } else {
    sumarPuntos(50);
    tone(BUZZER, 300, 120);
    activarModoAsustado();
  }
}

//SANTI ACA
//redibuja solamente el area del sprite con un buffer de 16 bits.
//el fondo y Pac-Man se componen en memoria antes de enviarse a la TFT.
void dibujarPacmanSinParpadeo(int anteriorX, int anteriorY) {
  const int ancho = 16;
  const int alto = 16;
  int izquierda = min(anteriorX, pacman.x) - 7;
  int arriba = min(anteriorY, pacman.y) - 7;
  GFXcanvas16 lienzo(ancho, alto);
  lienzo.fillScreen(ILI9341_BLACK);

  int colInicio = pisoCasilla(izquierda);
  int filaInicio = pisoCasilla(arriba);
  int colFin = pisoCasilla(izquierda + ancho - 1);
  int filaFin = pisoCasilla(arriba + alto - 1);

  for (int f = filaInicio; f <= filaFin; f++) {
    for (int c = colInicio; c <= colFin; c++) {
      if (f < 0 || f >= PM_FILAS || c < 0 || c >= PM_COLUMNAS) continue;
      int px = c * PM_CASILLA - izquierda;
      int py = f * PM_CASILLA - arriba;
      char cas = laberinto[f][c];
      if (cas == PM_PARED) {
        if (f > 0 && laberinto[f - 1][c] != PM_PARED)
          lienzo.fillRect(px, py + 1, PM_CASILLA, 2, PM_COLOR_PARED);
        if (f < PM_FILAS - 1 && laberinto[f + 1][c] != PM_PARED)
          lienzo.fillRect(px, py + PM_CASILLA - 3, PM_CASILLA, 2, PM_COLOR_PARED);
        if (c > 0 && laberinto[f][c - 1] != PM_PARED)
          lienzo.fillRect(px + 1, py, 2, PM_CASILLA, PM_COLOR_PARED);
        if (c < PM_COLUMNAS - 1 && laberinto[f][c + 1] != PM_PARED)
          lienzo.fillRect(px + PM_CASILLA - 3, py, 2, PM_CASILLA, PM_COLOR_PARED);
      } else if (cas == PM_PUNTO) {
        lienzo.fillRect(px + 3, py + 3, 2, 2, ILI9341_WHITE);
      } else if (cas == PM_PODER) {
        lienzo.fillCircle(px + 4, py + 4, 3, ILI9341_WHITE);
      } else if (cas == PM_PUERTA) {
        lienzo.drawFastHLine(px, py + 4, PM_CASILLA, ILI9341_PINK);
      }
    }
  }

  int sx = pacman.x - izquierda;
  int sy = pacman.y - arriba;
  lienzo.fillCircle(sx, sy, 3, ILI9341_YELLOW);
  if (pacman.bocaAbierta && pacman.direccion != PM_QUIETO) {
    int dx = PM_DX[pacman.direccion];
    int dy = PM_DY[pacman.direccion];
    lienzo.fillTriangle(sx, sy,
      sx + dx * 4 - dy * 3, sy + dy * 4 - dx * 3,
      sx + dx * 4 + dy * 3, sy + dy * 4 + dx * 3,
      ILI9341_BLACK);
  }
  tft.drawRGBBitmap(pantallaX(izquierda), pantallaY(arriba),
                    lienzo.getBuffer(), ancho, alto);
}

void moverPacman() {
  unsigned long ahora = millis();
  if (ahora - pacman.ultimoMovimiento < configActual(partidaPM.nivel).intervaloPacman) return;
  pacman.ultimoMovimiento = ahora;

  DireccionPM pedida = leerJoystickPM();
  if (pedida != PM_QUIETO) pacman.deseada = pedida;

  if (pacman.deseada == direccionOpuesta(pacman.direccion)) {
    pacman.direccion = pacman.deseada;
  }

  if (enCentroDeCasilla(pacman.x, pacman.y)) {
    int col = pacman.x / PM_CASILLA, fila = pacman.y / PM_CASILLA;
    comerCasilla(col, fila);

    if (puedeMoverse(col, fila, pacman.deseada)) pacman.direccion = pacman.deseada;
    if (!puedeMoverse(col, fila, pacman.direccion)) pacman.direccion = PM_QUIETO;
  }

  if (pacman.direccion == PM_QUIETO) return;

  int anteriorX = pacman.x;
  int anteriorY = pacman.y;
  avanzarPixel(pacman.x, pacman.y, pacman.direccion);
  if ((pacman.x + pacman.y) % 4 == 0) pacman.bocaAbierta = !pacman.bocaAbierta;
  dibujarPacmanSinParpadeo(anteriorX, anteriorY);
}

//funciones fantasmas

//depende el modo y depende el fantasma hace algo distinto
void objetivoFantasma(int i, int &objCol, int &objFila) {
  const Fantasma &g = fantasmas[i];
  int pCol = pacman.x / PM_CASILLA, pFila = pacman.y / PM_CASILLA;

  if (g.modo == F_COMIDO) {
    objCol = PM_SALIDA_COL; objFila = PM_SALIDA_FILA;
  } else if (g.modo == F_DISPERSION) {
    objCol = g.esquinaCol; objFila = g.esquinaFila;
  } else if (i == 1) {
    //ROSA: apunta 4 casillas adelante de pacman
    objCol = pCol + PM_DX[pacman.direccion] * 4;
    objFila = pFila + PM_DY[pacman.direccion] * 4;
  } else if (i == 3 && abs(pCol - g.x / PM_CASILLA) + abs(pFila - g.y / PM_CASILLA) < 8) {
    //NARAJA: si esta cerca se va a su esquina
    objCol = g.esquinaCol; objFila = g.esquinaFila;
  } else {
    //ROJO: sigue directo
    objCol = pCol; objFila = pFila;
  }
}

//elige direcciones mas cercas a pacman - no vuelve atras si no esta encerrado
DireccionPM elegirDireccion(int i) {
  Fantasma &g = fantasmas[i];
  int col = g.x / PM_CASILLA, fila = g.y / PM_CASILLA;
  DireccionPM reversa = direccionOpuesta(g.direccion);

  if (g.modo == F_ASUSTADO) {
    //va a cualquier lado cuando esta asustado
    DireccionPM opciones[4];
    int n = 0;
    for (int d = 0; d < 4; d++)
      if ((DireccionPM)d != reversa && puedeMoverse(col, fila, (DireccionPM)d))
        opciones[n++] = (DireccionPM)d;
    return n > 0 ? opciones[random(n)] : reversa;
  }

  int objCol, objFila;
  objetivoFantasma(i, objCol, objFila);

  DireccionPM mejor = reversa;
  long mejorDistancia = 0x7FFFFFFF;
  //desempate arriba - izquierda - abajo - derecha
  for (int d = 0; d < 4; d++) {
    if ((DireccionPM)d == reversa || !puedeMoverse(col, fila, (DireccionPM)d)) continue;
    long dx = col + PM_DX[d] - objCol;
    long dy = fila + PM_DY[d] - objFila;
    long distancia = dx * dx + dy * dy;
    if (distancia < mejorDistancia) {
      mejorDistancia = distancia;
      mejor = (DireccionPM)d;
    }
  }
  return mejor;
}

unsigned long intervaloFantasma(const Fantasma &g) {
  const ConfigNivel &cfg = configActual(partidaPM.nivel);
  if (g.modo == F_ASUSTADO) return cfg.intervaloAsustado;
  if (g.modo == F_COMIDO) return 8;   
  return cfg.intervaloFantasma;
}

void moverFantasma(int i, unsigned long tiempoJuego) {
  Fantasma &g = fantasmas[i];
  unsigned long ahora = millis();

  //es la salida de la casa
  if (g.modo == F_EN_CASA) {
    if (tiempoJuego >= g.tiempoSalida) {
      restaurarFondo(g.x, g.y);
      g.x = PM_SALIDA_COL * PM_CASILLA + 4;
      g.y = PM_SALIDA_FILA * PM_CASILLA + 4;
      g.direccion = PM_IZQUIERDA;
      g.modo = modoGlobalPersecucion ? F_PERSECUCION : F_DISPERSION;
    }
    return;
  }

  if (ahora - g.ultimoMovimiento < intervaloFantasma(g)) return;
  g.ultimoMovimiento = ahora;

  if (enCentroDeCasilla(g.x, g.y)) {
    if (g.modo == F_COMIDO &&
        g.x / PM_CASILLA == PM_SALIDA_COL && g.y / PM_CASILLA == PM_SALIDA_FILA) {
      g.modo = modoGlobalPersecucion ? F_PERSECUCION : F_DISPERSION;
    }
    g.direccion = elegirDireccion(i);
  }

  restaurarFondo(g.x, g.y);
  avanzarPixel(g.x, g.y, g.direccion);
}

//alterna dispersión / persecución y termina asustado
void actualizarModosFantasmas() {
  unsigned long ahora = millis();
  int cantidad = configActual(partidaPM.nivel).cantidadFantasmas;

  unsigned long duracion = modoGlobalPersecucion ? PM_TIEMPO_PERSECUCION : PM_TIEMPO_DISPERSION;
  if (ahora - inicioModoGlobal >= duracion) {
    modoGlobalPersecucion = !modoGlobalPersecucion;
    inicioModoGlobal = ahora;
    for (int i = 0; i < cantidad; i++) {
      Fantasma &g = fantasmas[i];
      if (g.modo == F_DISPERSION || g.modo == F_PERSECUCION) {
        g.modo = modoGlobalPersecucion ? F_PERSECUCION : F_DISPERSION;
      }
    }
  }

  if (finAsustado != 0 && (long)(ahora - finAsustado) >= 0) {
    finAsustado = 0;
    for (int i = 0; i < cantidad; i++) {
      if (fantasmas[i].modo == F_ASUSTADO)
        fantasmas[i].modo = modoGlobalPersecucion ? F_PERSECUCION : F_DISPERSION;
    }
  }
}

//trae cuando un fantasma agarra a pacman
bool revisarChoques() {
  for (int i = 0; i < configActual(partidaPM.nivel).cantidadFantasmas; i++) {
    Fantasma &g = fantasmas[i];
    if (g.modo == F_EN_CASA || g.modo == F_COMIDO) continue;

    int dx = abs(g.x - pacman.x);
    //túnel
    if (dx > PM_ANCHO / 2) dx = PM_ANCHO - dx;   
    if (dx >= 6 || abs(g.y - pacman.y) >= 6) continue;

    if (g.modo == F_ASUSTADO) {
      //se come al fantasma = 200, 400, 800, 1600
      g.modo = F_COMIDO;
      fantasmasComidosSeguidos++;
      partidaPM.fantasmasComidos++;
      sumarPuntos(100UL << min(fantasmasComidosSeguidos, 4));
      tone(BUZZER, 1200, 80);
    } else {
      return true;
    }
  }
  return false;
}

//funciones publicas

void iniciarPacman() {
  partidaPM.puntaje = 0;
  partidaPM.nivel = 0;
  partidaPM.vidas = 3;
  partidaPM.fantasmasComidos = 0;
  partidaPM.inicioPartida = millis();
  finAsustado = 0;

  cargarLaberinto();
  prepararNivel();
  reportarEventoPacman("inicio");
}

//pausa / continua / reinicia
void botonPacman() {
  unsigned long ahora = millis();

  if (estadoPacman == PM_JUGANDO) {
    cambiarEstadoPacman(PM_PAUSA);
    mensajeInferior("PAUSA", ILI9341_CYAN);
  } else if (estadoPacman == PM_PAUSA) {
    //cambio en los relojes para el tiempo en pausa 
    unsigned long pausa = ahora - inicioEstadoPM;
    inicioModoGlobal += pausa;
    if (finAsustado != 0) finAsustado += pausa;
    cambiarEstadoPacman(PM_JUGANDO);
    mensajeInferior("", ILI9341_BLACK);
  } else if (estadoPacman == PM_GAME_OVER || estadoPacman == PM_VICTORIA) {
    iniciarPacman();
  }
}

void actualizarPacman() {
  static unsigned long inicioJuego = 0;   
  static unsigned long tiempoPausado = 0;
  unsigned long ahora = millis();
  unsigned long enEstado = ahora - inicioEstadoPM;

  switch (estadoPacman) {

    case PM_LISTO:
      if (enEstado >= PM_TIEMPO_LISTO) {
        mensajeInferior("", ILI9341_BLACK);
        inicioJuego = ahora;
        tiempoPausado = 0;
        inicioModoGlobal = ahora;
        cambiarEstadoPacman(PM_JUGANDO);
      }
      break;

    case PM_PAUSA:
      tiempoPausado = enEstado;  
      break;

    case PM_JUGANDO: {
      if (tiempoPausado) {        
        inicioJuego += tiempoPausado;
        tiempoPausado = 0;
      }

      moverPacman();
      actualizarModosFantasmas();
      int cantidad = configActual(partidaPM.nivel).cantidadFantasmas;
      for (int i = 0; i < cantidad; i++) moverFantasma(i, ahora - inicioJuego);
      dibujarPersonajes();

      if (revisarChoques()) {
        tone(BUZZER, 150, 600);
        partidaPM.vidas--;
        reportarEventoPacman("vida_perdida");
        cambiarEstadoPacman(PM_MUERTE);
      } else if (puntosRestantes == 0) {
        reportarEventoPacman("nivel_completo");
        mensajeInferior("NIVEL COMPLETO", ILI9341_GREEN);
        cambiarEstadoPacman(PM_NIVEL_COMPLETO);
      }
      break;
    }

    case PM_MUERTE:
      if (enEstado >= PM_TIEMPO_MUERTE) {
        //se borran todos los personajes del lugar q estan
        restaurarFondo(pacman.x, pacman.y);
        for (int i = 0; i < MAX_FANTASMAS; i++) restaurarFondo(fantasmas[i].x, fantasmas[i].y);

        if (partidaPM.vidas <= 0) {
          reportarEventoPacman("game_over");
          mensajeInferior("GAME OVER", ILI9341_RED);
          cambiarEstadoPacman(PM_GAME_OVER);
        } else {
          //los puntos que se comieron quedan y solo vuelven los personajes
          dibujarVidasYNivel();
          reubicarPersonajes();
          dibujarPersonajes();
          mensajeInferior("LISTO!", ILI9341_YELLOW);
          cambiarEstadoPacman(PM_LISTO);
        }
      }
      break;

    case PM_NIVEL_COMPLETO:
      if (enEstado >= PM_TIEMPO_NIVEL) {
        //sube el contador, suben los puntos y se aplica la configuración del nuevo nivel
        if (partidaPM.nivel + 1 >= CANTIDAD_NIVELES) {
          //era el ultimo nivel gana la partida
          reportarEventoPacman("victoria");
          mensajeInferior("GANASTE!", ILI9341_GREEN);
          cambiarEstadoPacman(PM_VICTORIA);
        } else {
          partidaPM.nivel++;
          cargarLaberinto();
          prepararNivel();
        }
      }
      break;

    case PM_GAME_OVER:
    case PM_VICTORIA:
      //espera la el toque corto del boton 
      break;
  }
}