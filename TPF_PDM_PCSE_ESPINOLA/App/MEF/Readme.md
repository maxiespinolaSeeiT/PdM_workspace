# App / MEF (Máquina de Estados Principal)

Este directorio contiene la **Máquina de Estados Finita (MEF) principal** del sistema, encargada de coordinar el funcionamiento general de la aplicación.

La MEF gestiona:
- Inicialización del sistema
- Interacción con el usuario (UART)
- Lectura de sensores
- Procesamiento de datos
- Visualización en LCD
- Manejo de errores

---

## Estructura

```plaintext
App/MEF/
├── Inc/
│   └── MEF_main.h      # Definición de estados e interfaz pública
│
├── Src/
│   └── MEF_main.c      # Implementación de la lógica de control
│
└── README.md
```

---

## Descripción general

La MEF actúa como el **cerebro del sistema**, coordinando todos los módulos:

- Sensores:
  - AHT20 (temperatura y humedad)
  - BMP280 (temperatura y presión)

- Interfaces:
  - UART (interacción con usuario)
  - LCD (visualización)

- BSP:
  - I2C (comunicación con periféricos)

---

## Estados de la MEF

```plaintext
INIT          → Inicialización del sistema
IDLE          → Espera de comandos del usuario
READ_SENSOR   → Disparo y lectura de sensores
PROCESS_DATA  → Procesamiento de datos
SHOW_T_P_H    → Visualización de resultados
ERROR_MAIN    → Manejo de errores
REBOOT        → Reinicio del sistema
WAIT_USER     → Espera de acción del usuario tras error
```

---

## Funcionamiento general

### 1. Inicialización (`INIT`)
- Inicializa UART, GPIO, I2C, LCD y sensores
- Verifica que ambos sensores estén operativos
- Muestra mensaje inicial

---

### 2. Espera de comandos (`IDLE`)
- El sistema queda a la espera de comandos por UART
- Comandos disponibles:
  - Temperatura
  - Presión
  - Humedad
  - Reinicio

---

### 3. Lectura de sensores (`READ_SENSOR`)
- Se inicia la medición según el comando recibido
- Se espera a que los sensores indiquen datos listos
- Se validan errores de inicialización

---

### 4. Procesamiento (`PROCESS_DATA`)
- Conversión de datos crudos
- Cálculo de temperatura ambiente:
  
  Promedio entre AHT20 y BMP280 para reducir error

---

### 5. Visualización (`SHOW_T_P_H`)
- Se muestran los datos:
  - Por UART (debug/usuario)
  - En LCD (interfaz visual)

---

### 6. Manejo de errores (`ERROR_MAIN`)
- Se enciende LED de error
- Se notifican errores por UART
- Se detienen los sensores
- Se solicita intervención del usuario

---

### 7. Recuperación (`WAIT_USER` / `REBOOT`)
- El usuario puede reiniciar el sistema
- Se reinicia el bus I2C
- Se reintenta la inicialización completa

---

## Objetivo del diseño

- Centralizar la lógica del sistema
- Coordinar múltiples módulos de forma ordenada
- Evitar bloqueos mediante MEF
- Facilitar debug y mantenimiento

---

## Módulos utilizados

- Drivers:
  - AHT20
  - BMP280
  - LCD

- API:
  - UART
  - GPIO
  - Command Parser

- BSP:
  - I2C

---

## Características destacadas

- Arquitectura modular
- Uso de **MEF no bloqueante**
- Manejo de errores robusto
- Interfaz de usuario por UART
- Visualización en LCD
- Recuperación ante fallos de hardware (I2C)

---

## Notas importantes

- La MEF depende de llamadas periódicas a `MEF_main_update()`
- Los sensores trabajan de forma asincrónica (polling)
- Se implementa timeout y recuperación de bus I2C
- El sistema puede reiniciarse sin resetear el microcontrolador

---

## Autor

Ing. Espínola Maximiliano Ariel
