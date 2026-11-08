#pragma once

enum DkEtapa {
  DK_ETAPA_BARRILES = 1,
  DK_ETAPA_FABRICA = 2,
  DK_ETAPA_ASCENSORES = 3,
  DK_ETAPA_REMACHES = 4
};

volatile DkEtapa dkEtapaActual = DK_ETAPA_BARRILES;
volatile bool dkPantallaEtapaPendiente = false;

#include "dk_barriles.h"
#include "dk_fabrica.h"
#include "dk_ascensores.h"
#include "dk_remaches.h"

void dkIniciar() {
  dkEtapaActual = DK_ETAPA_BARRILES;
  dkPantallaEtapaPendiente = false;
  dkBarrilesIniciar();
  dkPuntuacion = 0;
  dkPuntuacionRender = 0;
  vidasMario = VIDAS_INICIALES;
  dkSaltoPendiente = false;
  estadoActual = JUEGO_DONKEY_KONG;
  Serial.println("DK: nueva partida - puntuacion 0");
}

void dkPasoLogica() {
  if (estadoActual != JUEGO_DONKEY_KONG) return;
  if (dkEtapaActual == DK_ETAPA_BARRILES) {
    dkBarrilesPasoLogica();
    return;
  }
  if (dkPantallaEtapaPendiente || dkMutex == nullptr) return;
  if (xSemaphoreTake(dkMutex, pdMS_TO_TICKS(5)) != pdTRUE) return;
  if (!dkPantallaEtapaPendiente) {
    if (dkEtapaActual == DK_ETAPA_FABRICA)
      dkFabricaPasoLogica();
    else if (dkEtapaActual == DK_ETAPA_ASCENSORES)
      dkAscensoresPasoLogica();
    else if (dkEtapaActual == DK_ETAPA_REMACHES)
      dkRemachesPasoLogica();
  }
  xSemaphoreGive(dkMutex);
}

void dkRenderizar() {
  if (dkEtapaActual == DK_ETAPA_BARRILES) {
    dkBarrilesRenderizar();
    return;
  }
  if (dkEtapaActual == DK_ETAPA_FABRICA) {
    if (dkPantallaEtapaPendiente) {
      if (dkMutex != nullptr) xSemaphoreTake(dkMutex, portMAX_DELAY);
      if (dkEtapaActual == DK_ETAPA_FABRICA && dkPantallaEtapaPendiente) {
        uint32_t puntajeAnterior = dkPuntuacion;
        int vidasAnteriores = vidasMario;
        dkFabricaIniciar();
        dkPuntuacion = puntajeAnterior;
        vidasMario = vidasAnteriores;
        dkPantallaEtapaPendiente = false;
      }
      if (dkMutex != nullptr) xSemaphoreGive(dkMutex);
    }
    dkFabricaDibujar();
    return;
  }
  if (dkEtapaActual == DK_ETAPA_ASCENSORES) {
    if (dkPantallaEtapaPendiente) {
      if (dkMutex != nullptr) xSemaphoreTake(dkMutex, portMAX_DELAY);
      if (dkEtapaActual == DK_ETAPA_ASCENSORES && dkPantallaEtapaPendiente) {
        uint32_t puntajeAnterior = dkPuntuacion;
        int vidasAnteriores = vidasMario;
        dkAscensoresIniciar();
        dkPuntuacion = puntajeAnterior;
        vidasMario = vidasAnteriores;
        dkPantallaEtapaPendiente = false;
      }
      if (dkMutex != nullptr) xSemaphoreGive(dkMutex);
    }
    dkAscensoresRenderizar();
    return;
  }
  if (dkEtapaActual == DK_ETAPA_REMACHES) {
    if (dkPantallaEtapaPendiente) {
      if (dkMutex != nullptr) xSemaphoreTake(dkMutex, portMAX_DELAY);
      if (dkEtapaActual == DK_ETAPA_REMACHES && dkPantallaEtapaPendiente) {
        uint32_t puntajeAnterior = dkPuntuacion;
        int vidasAnteriores = vidasMario;
        dkRemachesIniciar();
        dkPuntuacion = puntajeAnterior;
        vidasMario = vidasAnteriores;
        dkPantallaEtapaPendiente = false;
      }
      if (dkMutex != nullptr) xSemaphoreGive(dkMutex);
    }
    dkRemachesRenderizar();
  }
}

void dkActualizar() {
  dkRenderizar();
}

void dkBotonCorto() {
  if (estadoActual == PANTALLA_GAME_OVER ||
      estadoActual == PANTALLA_VICTORIA) {
    dkIniciar();
    return;
  }
  if (estadoActual == JUEGO_DONKEY_KONG) {
    dkSaltoPendiente = true;
  }
}
