# Práctica 3 – API de retardos no bloqueantes

## Autor
Maximiliano Espínola

## Descripción
En esta práctica se desarrolla una API para implementar retardos no bloqueantes en lenguaje C.  
El objetivo es permitir la ejecución de múltiples tareas temporizadas sin detener la ejecución del programa principal, evitando el uso de funciones de retardo bloqueantes.

La API utiliza una estructura de datos y funciones auxiliares para gestionar el tiempo mediante el contador de ticks del sistema.

---

## Estructura de la API

La API está compuesta por los siguientes archivos:

- API/Inc/delay.h
- API/Src/delay.c

En estos archivos se definen la estructura del retardo y las funciones necesarias para su manejo.

---

## Estructura de datos

La estructura utilizada para los retardos es:

```c
typedef struct {
    tick_t startTime;
    tick_t duration;
    bool_t running;
} delay_t;

```

## Descripción de los campos
- startTime: Guarda el tiempo en el que inicia el retardo.
- duration: Duración del retardo en ticks.
- running: Indica si el retardo está corriendo o no.

## Funciones de la API 

# delayInit()

 Inicializa la estructura de retardo.

```c
void delayInit(delay_t * delay, tick_t duration);
```
Inicializa el retardo, establece la duración y lo deja listo para comenzar.

# delayRead()

Verifica si el retardo finalizó.
```c
bool_t delayRead(delay_t * delay);
```
Devuelve true cuando el retardo terminó.
Devuelve false cuando el retardo todavía está en curso.
Reinicia automáticamente el retardo cuando termina.

# delayWrite()

Permite modificar la duración del retardo.

```c
void delayWrite(delay_t * delay, tick_t duration);
```

Cambia el tiempo de duración del retardo.

# delayIsRunning()
Permite verificar si el retardo está en curso

```c
bool_t delayIsRunning( delay_t* delay )
```
Devuelve true cuando está corriendo y false cuando ya finalizó.

## Ejemplo de uso

```c
#include "delay.h"

delay_t delayLed;

int main(void)
{
    delayInit(&delayLed, 1000);

    while(1)
    {
        if(delayRead(&delayLed))
        {
            HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        }
    }
}
```

En este ejemplo el LED cambia de estado cada 1000 ms sin bloquear la ejecución del programa.

Objetivo del retardo no bloqueante

El uso de retardos no bloqueantes permite:

Ejecutar múltiples tareas en paralelo
No detener el microcontrolador con delays
Mejorar la eficiencia del programa
Implementar sistemas embebidos multitarea simples
