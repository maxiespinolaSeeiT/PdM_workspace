# BSP (Board Support Package)

Este directorio contiene la implementación del **BSP (Board Support Package)**, encargado de abstraer el acceso al hardware del microcontrolador.

El BSP actúa como intermediario entre:
- Los **drivers de alto nivel** (sensores, LCD, etc.)
- La **HAL de STM32**

---

## Estructura

```plaintext
BSP/
├── Inc/
│   ├── BSP_GPIO.h      # Configuración de pines GPIO
│   ├── BSP_I2C.h       # Interfaz de comunicación I2C
│   └── BSP_UART.h      # Interfaz de comunicación UART
│
├── Src/
│   ├── BSP_GPIO.c      # Implementación de GPIO
│   ├── BSP_I2C.c       # Implementación de I2C (HAL)
│   └── BSP_UART.c      # Implementación de UART (HAL)
│
└── README.md
```

---

## Descripción de módulos

### BSP_GPIO
Configura y controla los pines digitales del microcontrolador.

Responsabilidades:
- Inicialización de GPIO
- Manejo de LED de estado (LD2)
- Indicación de errores

---

### BSP_I2C
Encapsula el manejo del periférico I2C utilizando HAL.

Responsabilidades:
- Inicialización del bus I2C
- Lectura y escritura de datos
- Lectura de registros (memoria)
- Manejo de delays y tiempo (tick)
- Verificación de dispositivos conectados
- Recuperación del bus (restart)

---

### BSP_UART
Encapsula la comunicación serie mediante USART.

Responsabilidades:
- Inicialización de UART
- Envío y recepción de datos
- Configuración de baudrate
- Limpieza de buffer
- Monitoreo del estado de transmisión

---

## Funcionalidad general

El BSP proporciona funciones de bajo nivel utilizadas por:
- Drivers (LCD, sensores)
- API de aplicación

Toda interacción con hardware debe realizarse a través de esta capa.

---

## Objetivo del diseño

- Desacoplar la lógica de aplicación del hardware
- Centralizar el acceso a periféricos
- Facilitar portabilidad a otras plataformas
- Mejorar mantenibilidad y escalabilidad

---

## Periféricos implementados

- GPIO (LED de estado)
- I2C (comunicación con sensores y LCD)
- UART (comunicación serie)

---

## Notas importantes

- El BSP utiliza la **HAL de STM32**
- No contiene lógica de aplicación
- Todos los drivers dependen del BSP y no directamente de la HAL
- Se incluye manejo básico de errores (LED + bloqueo en I2C)

---

## Manejo de errores

- En UART: se indica mediante el LED LD2  
- En I2C: se utiliza `Error_Handler()` (bloqueante)  
- Posibilidad de reinicio del periférico I2C en caso de bloqueo del bus  

---

## Autor

Ing. Espínola Maximiliano Ariel
