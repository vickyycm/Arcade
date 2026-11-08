

#pragma once

const int REM_PISOS[4] = {275, 215, 155, 95};

const int REM_ESC_X[3] = {195, 35, 195};

const int REM_REM_X[2] = {76, 157};

const int REM_MW = 14, REM_MH = 18;

const unsigned long REM_TICK = 20, REM_FRAME = 40;

const float REM_ENEMIGO_VEL = 0.85f;

struct RemVisual
{

    int x, y, vidas;

    bool visible;

    uint32_t puntos;

    bool remache[4][2];

    int enemigoX[3];
};

float remX = 22, remY = 257, remVy = 0;

int remPiso = 0, remDestino = 0;

bool remSuelo = true, remEscalera = false, remInvulnerable = false;

bool remPantallaLista = false, remAnteriorValido = false;

bool remHudPendiente = true;

bool remVictoriaPendiente = false, remDerrotaPendiente = false;

bool remRescateActivo = false, remRescateEscenaLista = false;

unsigned long remRescateInicio = 0, remRescateUltimoFrame = 0;

bool remacheActivo[4][2];

float remEnemigoX[3];

int remEnemigoDir[3];

unsigned long remUltimoTick = 0, remUltimoFrame = 0;

unsigned long remInicioGolpe = 0, remSaltoRegistrado = 0;

unsigned long remProteccionHueco = 0;

bool remSaltoEnCola = false;

RemVisual remAnterior;

int remSueloY(int piso) { return REM_PISOS[piso] - REM_MH; }

void dkRemachesIniciar()
{

    remX = 22;

    remY = remSueloY(0);

    remVy = 0;

    remPiso = 0;

    remDestino = 0;

    remSuelo = true;

    remEscalera = false;

    remInvulnerable = false;

    remVictoriaPendiente = false;

    remDerrotaPendiente = false;

    remRescateActivo = false;

    remRescateEscenaLista = false;

    remRescateInicio = 0;

    remRescateUltimoFrame = 0;

    remPantallaLista = false;

    remAnteriorValido = false;

    remHudPendiente = true;

    remSaltoEnCola = false;

    remUltimoTick = millis();

    remUltimoFrame = 0;

    remInicioGolpe = 0;

    remProteccionHueco = 0;

    for (int n = 0; n < 4; ++n)

        for (int j = 0; j < 2; ++j)

            remacheActivo[n][j] = true;

    for (int i = 0; i < 3; ++i)
    {

        remEnemigoX[i] = i % 2 ? 190 : 45;

        remEnemigoDir[i] = i % 2 ? -1 : 1;
    }

    dkSaltoPendiente = false;

    Serial.println("DK: remaches iniciados");
}

bool remHayPiso(int nivel, float centroX)
{

    if (nivel <= 0)
        return true;

    for (int j = 0; j < 2; ++j)

        if (!remacheActivo[nivel][j] &&

            fabsf(centroX - REM_REM_X[j]) <= 6.0f)

            return false;

    return true;
}

void dkRemachesGolpe()
{

    if (remInvulnerable || remDerrotaPendiente)
        return;

    --vidasMario;

    dkSonidoImpacto();

    if (vidasMario <= 0)
    {

        vidasMario = 0;

        remDerrotaPendiente = true;

        return;
    }

    remX = 22;

    remY = remSueloY(0);

    remPiso = 0;

    remVy = 0;

    remSuelo = true;

    remEscalera = false;

    remInvulnerable = true;

    remInicioGolpe = millis();

    remSaltoEnCola = false;
}

void dkRemachesPasoLogica()
{

    if (estadoActual != JUEGO_DONKEY_KONG ||

        remRescateActivo || remVictoriaPendiente || remDerrotaPendiente)
        return;

    unsigned long ahora = millis();

    if (ahora - remUltimoTick < REM_TICK)
        return;

    remUltimoTick = ahora;

    if (dkSaltoPendiente)
    {

        remSaltoEnCola = true;

        remSaltoRegistrado = ahora;

        dkSaltoPendiente = false;
    }

    if (remSaltoEnCola && ahora - remSaltoRegistrado > 180)

        remSaltoEnCola = false;

    if (remInvulnerable && ahora - remInicioGolpe >= 1500)

        remInvulnerable = false;

    int joyX = analogRead(JOY_X);

    int joyY = analogRead(JOY_Y);

    int mov = joyX > 3000 ? -VELOCIDAD_MARIO :

              joyX < 1000 ? VELOCIDAD_MARIO
                          : 0;

    if (remEscalera)
    {

        float objetivo = remSueloY(remDestino);

        if (remY > objetivo)
            remY = max(objetivo, remY - 2.0f);

        else if (remY < objetivo)
            remY = min(objetivo, remY + 2.0f);

        if (remY == objetivo)
        {

            remPiso = remDestino;

            remEscalera = false;

            remSuelo = true;

            remVy = 0;
        }

        remSaltoEnCola = false;
    }
    else
    {

        bool usarEscalera = false;

        if (remSuelo)
        {

            if (joyY > UMBRAL_ARRIBA && remPiso < 3)
            {

                if (fabsf(remX + REM_MW / 2.0f - REM_ESC_X[remPiso]) <= 15)
                {

                    remDestino = remPiso + 1;

                    usarEscalera = true;
                }
            }
            else if (joyY < UMBRAL_ABAJO && remPiso > 0)
            {

                if (fabsf(remX + REM_MW / 2.0f - REM_ESC_X[remPiso - 1]) <= 15)
                {

                    remDestino = remPiso - 1;

                    usarEscalera = true;
                }
            }
        }

        if (usarEscalera)
        {

            remX = REM_ESC_X[min(remPiso, remDestino)] - REM_MW / 2.0f;

            remEscalera = true;

            remSuelo = false;

            remVy = 0;

            remSaltoEnCola = false;
        }
        else
        {

            remX = constrain(remX + mov, 10.0f, 216.0f);

            if (remSuelo && remSaltoEnCola)
            {

                remSaltoEnCola = false;

                remSuelo = false;

                remVy = -5.0f;

                dkSonidoSalto();
            }

            if (remSuelo && ahora - remProteccionHueco > 140 &&

                !remHayPiso(remPiso, remX + REM_MW / 2.0f))
            {

                remSuelo = false;

                remVy = 0.4f;
            }

            if (!remSuelo)
            {

                float anteriorY = remY;

                remY += remVy;

                remVy += 0.45f;

                if (remVy >= 0)
                {

                    // Aterrizaje en el piso superior actual o en los inferiores.

                    for (int n = 3; n >= 0; --n)
                    {

                        float suelo = remSueloY(n);

                        if (anteriorY <= suelo && remY >= suelo &&

                            remHayPiso(n, remX + REM_MW / 2.0f))
                        {

                            remY = suelo;

                            remPiso = n;

                            remVy = 0;

                            remSuelo = true;

                            break;
                        }
                    }
                }

                if (remY > 297)
                    dkRemachesGolpe();
            }
        }
    }

    for (int n = 0; n < 4; ++n)
    {

        if (remEscalera || remPiso != n || !remSuelo)
            continue;

        for (int j = 0; j < 2; ++j)
        {

            if (remacheActivo[n][j] &&

                fabsf(remX + REM_MW / 2.0f - REM_REM_X[j]) <= 8)
            {

                remacheActivo[n][j] = false;

                remProteccionHueco = ahora;

                dkPuntuacion += 100;

                musicaEfecto(880, 45);
            }
        }
    }

    for (int i = 0; i < 3; ++i)
    {

        remEnemigoX[i] += remEnemigoDir[i] * REM_ENEMIGO_VEL;

        if (remEnemigoX[i] < 18)
        {

            remEnemigoX[i] = 18;

            remEnemigoDir[i] = 1;
        }
        else if (remEnemigoX[i] > 222)
        {

            remEnemigoX[i] = 222;

            remEnemigoDir[i] = -1;
        }

        int nivel = i + 1;

        int ey = REM_PISOS[nivel] - 10;

        if (!remInvulnerable &&

            remX + REM_MW > remEnemigoX[i] - 6 &&

            remX < remEnemigoX[i] + 6 &&

            remY + REM_MH > ey - 6 && remY < ey + 6)
        {

            dkRemachesGolpe();

            break;
        }
    }

    bool quedan = false;

    for (int n = 0; n < 4; ++n)

        for (int j = 0; j < 2; ++j)

            if (remacheActivo[n][j])
                quedan = true;

    if (!quedan)
    {

        remRescateActivo = true;

        remRescateEscenaLista = false;

        remRescateInicio = ahora;

        remRescateUltimoFrame = 0;

        dkSaltoPendiente = false;

        dkPuntuacion += 1000;

        Serial.println("DK: remaches completados");
    }
}

uint16_t remFondo(int x, int y, const RemVisual &s)
{

    if (x < 0 || x >= 240 || y < 0 || y >= 320)
        return ILI9341_BLACK;

    if (y == 31 && x >= 5 && x < 235)
        return ILI9341_CYAN;

    for (int n = 0; n < 4; ++n)
    {

        if (x >= 10 && x < 230 && y >= REM_PISOS[n] && y < REM_PISOS[n] + 8)
        {

            for (int j = 0; j < 2; ++j)

                if (!s.remache[n][j] && abs(x - REM_REM_X[j]) <= 6)

                    return ILI9341_BLACK;

            if (y == REM_PISOS[n] + 2 && (x - 10) % 18 < 9)

                return ILI9341_YELLOW;

            return ILI9341_DARKGREY;
        }
    }

    for (int n = 0; n < 3; ++n)
    {

        int ex = REM_ESC_X[n];

        int superior = REM_PISOS[n + 1] + 8;

        int inferior = REM_PISOS[n];

        if (y >= superior && y < inferior)
        {

            if (x == ex - 7 || x == ex + 7)
                return ILI9341_CYAN;

            if (x >= ex - 7 && x <= ex + 7 && y >= superior + 4 &&

                (y - superior - 4) % 9 == 0)
                return ILI9341_CYAN;
        }
    }

    for (int n = 0; n < 4; ++n)

        for (int j = 0; j < 2; ++j)
        {

            if (s.remache[n][j] && abs(x - REM_REM_X[j]) <= 4 &&

                y >= REM_PISOS[n] - 7 && y < REM_PISOS[n])

                return ILI9341_WHITE;
        }

    return ILI9341_BLACK;
}

void remCapturar(RemVisual &s)
{

    s.x = (int)roundf(remX);

    s.y = (int)roundf(remY);

    s.vidas = vidasMario;

    s.visible = !remInvulnerable || ((millis() / 150) % 2 == 0);

    s.puntos = dkPuntuacion;

    for (int n = 0; n < 4; ++n)

        for (int j = 0; j < 2; ++j)

            s.remache[n][j] = remacheActivo[n][j];

    for (int i = 0; i < 3; ++i)

        s.enemigoX[i] = (int)roundf(remEnemigoX[i]);
}

uint16_t remPixel(int x, int y, const RemVisual &s)
{

    uint16_t c = remFondo(x, y, s);

    for (int i = 0; i < 3; ++i)
    {

        int dx = x - s.enemigoX[i];

        int dy = y - (REM_PISOS[i + 1] - 10);

        if (dx * dx + dy * dy <= 36)
            c = ILI9341_ORANGE;
    }

    if (s.visible && x >= s.x && x < s.x + REM_MW &&

        y >= s.y && y < s.y + REM_MH)
    {

        const char *const *sprite = ((s.x / 5) & 1)

                                        ? DK_MARIO_PASO
                                        : DK_MARIO_QUIETO;

        char p = sprite[y - s.y][x - s.x];

        if (p != '.')
            c = dkColorPixel(p);
    }

    return c;
}

void remRegion(int l, int t, int r, int b, const RemVisual &s)
{

    l = constrain(l, 0, 240);

    r = constrain(r, 0, 240);

    t = constrain(t, 55, 295);

    b = constrain(b, 55, 295);

    if (l >= r || t >= b)
        return;

    for (int y = t; y < b; y += BUFFER_ALTO)
    {

        int h = min(BUFFER_ALTO, b - y);

        for (int x = l; x < r; x += BUFFER_ANCHO)
        {

            int w = min(BUFFER_ANCHO, r - x);

            for (int yy = 0; yy < h; ++yy)

                for (int xx = 0; xx < w; ++xx)

                    bufferRegion[yy * w + xx] = remPixel(x + xx, y + yy, s);

            tft.drawRGBBitmap(x, y, bufferRegion, w, h);
        }
    }
}

void remEscenario(const RemVisual &s)
{

    tft.fillScreen(ILI9341_BLACK);

    tft.setTextSize(1);

    tft.setTextColor(ILI9341_YELLOW);

    tft.setCursor(8, 12);

    tft.print("DONKEY KONG - REMACHES");

    remRegion(0, 55, 240, 295, s);

    tft.setTextColor(ILI9341_WHITE);

    tft.setCursor(8, 301);

    tft.print("3S MENU");

    remPantallaLista = true;
}

// Animacion final: ejecutada desde el nucleo de renderizado, sin delay().

void remDibujarDonkey(int x, int y)
{

    tft.fillRoundRect(x + 4, y + 4, 34, 29, 6, ILI9341_MAROON);

    tft.fillRoundRect(x + 10, y, 22, 16, 5, ILI9341_MAROON);

    tft.fillRect(x + 14, y + 11, 15, 7, ILI9341_ORANGE);

    tft.fillRect(x + 14, y + 6, 3, 3, ILI9341_WHITE);

    tft.fillRect(x + 25, y + 6, 3, 3, ILI9341_WHITE);

    tft.fillRect(x + 2, y + 25, 9, 11, ILI9341_MAROON);

    tft.fillRect(x + 32, y + 25, 9, 11, ILI9341_MAROON);
}

void remDibujarMarioFinal(int x, int y)
{

    tft.fillRect(x + 3, y, 14, 5, ILI9341_RED);

    tft.fillRect(x + 5, y + 5, 12, 8, ILI9341_ORANGE);

    tft.fillRect(x + 2, y + 13, 17, 11, ILI9341_BLUE);

    tft.fillRect(x + 2, y + 24, 7, 5, ILI9341_RED);

    tft.fillRect(x + 12, y + 24, 7, 5, ILI9341_RED);
}

void remDibujarPauline(int x, int y)
{

    tft.fillRect(x + 4, y, 12, 5, ILI9341_MAROON);

    tft.fillRect(x + 5, y + 5, 10, 9, ILI9341_ORANGE);

    tft.fillRect(x + 2, y + 14, 16, 13, ILI9341_MAGENTA);

    tft.fillRect(x, y + 25, 20, 5, ILI9341_MAGENTA);
}

void remDibujarCorazon(int cx, int cy)
{

    tft.fillCircle(cx - 5, cy - 3, 6, ILI9341_RED);

    tft.fillCircle(cx + 5, cy - 3, 6, ILI9341_RED);

    tft.fillTriangle(cx - 11, cy, cx + 11, cy, cx, cy + 13, ILI9341_RED);
}

void remAnimarRescate()
{

    unsigned long ahora = millis();

    unsigned long t = ahora - remRescateInicio;

    if (t >= 6000UL)
    {

        remRescateActivo = false;

        remVictoriaPendiente = true;

        return;
    }

    if (remRescateUltimoFrame != 0 && ahora - remRescateUltimoFrame < 55UL)

        return;

    remRescateUltimoFrame = ahora;

    if (!remRescateEscenaLista)
    {

        tft.fillScreen(ILI9341_BLACK);

        tft.setTextSize(1);

        tft.setTextColor(ILI9341_YELLOW);

        tft.setCursor(49, 20);

        tft.print("RESCATE DE PAULINE");

        tft.fillRect(15, 226, 210, 8, ILI9341_DARKGREY);

        tft.fillRect(15, 234, 210, 3, ILI9341_YELLOW);

        remRescateEscenaLista = true;
    }

    // Se borran solamente las zonas de los personajes.

    tft.fillRect(15, 57, 210, 169, ILI9341_BLACK);

    tft.fillRect(15, 237, 210, 25, ILI9341_BLACK);

    if (t < 1700UL)
    {

        int caida = (int)(t * 160UL / 1700UL);

        remDibujarDonkey(98, 61 + caida);

        remDibujarMarioFinal(35, 196);

        remDibujarPauline(182, 196);

        tft.setTextColor(ILI9341_WHITE);

        tft.setCursor(70, 248);

        tft.print("DONKEY KONG CAE");
    }
    else if (t < 3600UL)
    {

        int marioX = 35 + (int)((t - 1700UL) * 130UL / 1900UL);

        remDibujarMarioFinal(marioX, 196);

        remDibujarPauline(182, 196);

        tft.setTextColor(ILI9341_WHITE);

        tft.setCursor(65, 248);

        tft.print("MARIO VA A PAULINE");
    }
    else
    {

        remDibujarMarioFinal(160, 196);

        remDibujarPauline(182, 196);

        remDibujarCorazon(180, 161);

        tft.setTextColor(ILI9341_MAGENTA);

        tft.setCursor(67, 248);

        tft.print("PAULINE RESCATADA");
    }
}

void remPantallaVictoria()
{

    dkSonidoVictoria();

    tft.fillScreen(ILI9341_BLACK);

    tft.setTextSize(2);

    tft.setTextColor(ILI9341_YELLOW);

    tft.setCursor(54, 50);

    tft.print("VICTORIA!");

    tft.setTextSize(1);

    tft.setTextColor(ILI9341_MAGENTA);

    tft.setCursor(64, 104);

    tft.print("PAULINE RESCATADA");

    tft.setTextColor(ILI9341_CYAN);

    tft.setCursor(63, 151);

    tft.print("DONKEY KONG COMPLETO");

    tft.setTextColor(ILI9341_WHITE);

    tft.setCursor(52, 200);

    tft.printf("SCORE: %lu", (unsigned long)dkPuntuacion);

    tft.setCursor(62, 225);

    tft.printf("VIDAS: %d", vidasMario);

    tft.setCursor(29, 275);

    tft.print("BOTON: VOLVER A JUGAR");

    estadoActual = PANTALLA_VICTORIA;
}

void remPantallaDerrota()
{

    tft.fillScreen(ILI9341_BLACK);

    tft.setTextSize(2);

    tft.setTextColor(ILI9341_RED);

    tft.setCursor(65, 105);

    tft.print("GAME OVER");

    tft.setTextSize(1);

    tft.setTextColor(ILI9341_WHITE);

    tft.setCursor(52, 175);

    tft.printf("SCORE: %lu", (unsigned long)dkPuntuacion);

    tft.setCursor(30, 250);

    tft.print("BOTON: REINICIAR");

    estadoActual = PANTALLA_GAME_OVER;
}

void dkRemachesRenderizar()
{

    if (dkEtapaActual != DK_ETAPA_REMACHES ||

        estadoActual != JUEGO_DONKEY_KONG)
        return;

    if (remRescateActivo)
    {

        remAnimarRescate();

        return;
    }

    if (remVictoriaPendiente)
    {

        remVictoriaPendiente = false;

        remPantallaVictoria();

        return;
    }

    if (remDerrotaPendiente)
    {

        remDerrotaPendiente = false;

        remPantallaDerrota();

        return;
    }

    unsigned long ahora = millis();

    if (ahora - remUltimoFrame < REM_FRAME)
        return;

    remUltimoFrame = ahora;

    RemVisual actual;

    if (dkMutex != nullptr &&

        xSemaphoreTake(dkMutex, pdMS_TO_TICKS(5)) != pdTRUE)
        return;

    remCapturar(actual);

    if (dkMutex != nullptr)
        xSemaphoreGive(dkMutex);

    if (!remPantallaLista)
        remEscenario(actual);

    else
    {

        if (remAnterior.x != actual.x || remAnterior.y != actual.y ||

            remAnterior.visible != actual.visible)
        {

            int dx = abs(remAnterior.x - actual.x);

            int dy = abs(remAnterior.y - actual.y);

            if (dx <= REM_MW && dy <= REM_MH)
            {

                remRegion(min(remAnterior.x, actual.x) - 1,

                          min(remAnterior.y, actual.y) - 1,

                          max(remAnterior.x, actual.x) + REM_MW + 1,

                          max(remAnterior.y, actual.y) + REM_MH + 1, actual);
            }
            else
            {

                remRegion(remAnterior.x - 1, remAnterior.y - 1,

                          remAnterior.x + REM_MW + 1,

                          remAnterior.y + REM_MH + 1, actual);

                remRegion(actual.x - 1, actual.y - 1,

                          actual.x + REM_MW + 1,

                          actual.y + REM_MH + 1, actual);
            }
        }

        for (int n = 0; n < 4; ++n)

            for (int j = 0; j < 2; ++j)

                if (remAnterior.remache[n][j] != actual.remache[n][j])
                {

                    int x = REM_REM_X[j], y = REM_PISOS[n];

                    remRegion(x - 9, y - 9, x + 10, y + 9, actual);
                }

        for (int i = 0; i < 3; ++i)

            if (remAnterior.enemigoX[i] != actual.enemigoX[i])
            {

                int y = REM_PISOS[i + 1] - 10;

                remRegion(min(remAnterior.enemigoX[i], actual.enemigoX[i]) - 7,

                          y - 7,

                          max(remAnterior.enemigoX[i], actual.enemigoX[i]) + 8,

                          y + 8, actual);
            }
    }

    remAnterior = actual;

    remAnteriorValido = true;

    static int vidasPrevias = -1;

    static uint32_t puntosPrevios = 0xFFFFFFFF;

    if (vidasPrevias != actual.vidas || puntosPrevios != actual.puntos ||

        remHudPendiente)
    {

        tft.fillRect(6, 39, 228, 12, ILI9341_BLACK);

        tft.setTextSize(1);

        tft.setTextColor(ILI9341_WHITE);

        tft.setCursor(8, 40);

        tft.printf("VIDAS:%d SCORE:%lu", actual.vidas,

                   (unsigned long)actual.puntos);

        vidasPrevias = actual.vidas;

        puntosPrevios = actual.puntos;

        remHudPendiente = false;
    }
}
