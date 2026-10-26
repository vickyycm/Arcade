# 🕹️ Arcade – Mini Consola de Videojuegos con ESP32

Este proyecto fue desarrollado como parte del **segundo parcial de la materia Integración Tecnológica**, con el objetivo de diseñar e implementar una mini consola de videojuegos utilizando un microcontrolador ESP32 y componentes electrónicos.

La propuesta consiste en una consola interactiva que permite seleccionar y jugar dos videojuegos clásicos, **Donkey Kong y Pac-Man**, mediante un joystick y una pantalla TFT. El sistema integra hardware y software para ofrecer una experiencia de juego inspirada en las máquinas arcade tradicionales.

---

## 🎯 Objetivo del proyecto

Desarrollar un prototipo de videojuegos basado en tecnología embebida, aplicando conceptos de programación, control de periféricos, lectura de entradas y representación gráfica en una pantalla.

---

## ✨ Características

- Menú principal para seleccionar entre distintos videojuegos.
- Implementación de los juegos Donkey Kong y Pac-Man.
- Control de movimiento mediante joystick analógico.
- Pantalla TFT para mostrar los gráficos y la interfaz de los juegos.
- Botón de interacción para saltar, confirmar selecciones y controlar la navegación.
- Buzzer para la reproducción de sonidos.
- Gestión de estados para controlar el menú y los juegos.
- Actualización dinámica de los elementos gráficos durante la partida.

---

## 🎮 Juegos incluidos

### 🦍 Donkey Kong

Versión del clásico juego de plataformas en la que el jugador controla al personaje, se desplaza entre plataformas y escaleras y debe evitar los barriles que aparecen durante la partida.

### 👻 Pac-Man

Versión del clásico videojuego de laberintos, adaptada a la pantalla de la consola y controlada mediante el joystick.

---

## 🧠 Tecnologías utilizadas

- ESP32
- Arduino / C++
- Arduino IDE
- Pantalla TFT con controlador ILI9341
- Librería Adafruit GFX
- Librería Adafruit ILI9341
- Joystick analógico
- Botón físico
- Buzzer

---

## 🔌 Componentes de hardware

| Componente | Función |
|---|---|
| ESP32 | Ejecuta la lógica de programación y controla los periféricos. |
| Pantalla TFT ILI9341 | Muestra el menú, los escenarios y los elementos gráficos de los juegos. |
| Joystick analógico | Permite navegar por el menú y controlar los movimientos. |
| Botón físico | Permite confirmar acciones e interactuar durante las partidas. |
| Buzzer | Permite generar señales sonoras. |
| Cables de conexión | Permiten conectar los componentes al microcontrolador. |

---

## ⚙️ Funcionamiento del sistema

1. Al encender la consola, se inicializa la pantalla y se muestra el menú principal.
2. El usuario utiliza el joystick para seleccionar el videojuego.
3. Al confirmar la selección, se inicia el juego correspondiente.
4. Durante la partida, el sistema lee las entradas del joystick y del botón para actualizar el estado del juego.
5. La pantalla se actualiza para representar los movimientos, escenarios y elementos de cada videojuego.
6. El usuario puede regresar al menú principal mediante la interacción con el botón.

---

## 🏗️ Arquitectura del programa

El código está organizado en funciones que separan las responsabilidades principales del sistema:

- **Gestión del menú:** permite seleccionar el videojuego.
- **Control de estados:** determina si el sistema se encuentra en el menú o ejecutando uno de los juegos.
- **Lectura de entradas:** procesa los movimientos del joystick y las pulsaciones del botón.
- **Renderizado gráfico:** dibuja los elementos y escenarios en la pantalla TFT.
- **Lógica de juego:** administra los movimientos, las colisiones y las actualizaciones propias de cada videojuego.

---

## 🚀 Cómo ejecutar el proyecto

1. Clonar el repositorio:

   ```bash
   git clone https://github.com/vickyycm/Arcade.git
   ```

2. Abrir el proyecto en el entorno de desarrollo compatible con Arduino.

3. Instalar las librerías necesarias:
   - Adafruit GFX Library
   - Adafruit ILI9341

4. Conectar los componentes respetando la asignación de pines definida en el código.

5. Seleccionar la placa ESP32-S3 correspondiente y el puerto de conexión.

6. Compilar y cargar el programa en el microcontrolador.

---

## 📚 Contexto académico

Proyecto desarrollado para el segundo parcial de la materia **Integración Tecnológica**, integrando programación en C++, sistemas embebidos, control de periféricos e interacción entre hardware y software.

La documentación del proyecto contempla el funcionamiento del prototipo, sus componentes, la lógica de programación y los recursos necesarios para su simulación y reproducción.

---
