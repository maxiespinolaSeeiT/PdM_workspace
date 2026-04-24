# Drivers / API

Este directorio contiene la **capa API del sistema**, encargada de proporcionar funciones de más alto nivel sobre el BSP para ser utilizadas por la aplicación.

La API actúa como intermediario entre:
- La **MEF principal (App)**
- El **BSP (hardware)**

---

## Estructura

```plaintext
Drivers/API/
├── Inc/
│   ├── API_cmdparser.h     # Parser de comandos por UART
│   ├── API_common_types.h  # Tipos de datos comunes
│   ├── API_GPIO.h          # Funciones de control de GPIO
│   ├── API_I2C.h           # Funciones de alto nivel para I2C
│   └── API_uart.h          # Funciones de comunicación UART
│
├── Src/
│   ├── API_cmdparser.c     # Implementación del parser (MEF)
│   ├── API_GPIO.c          # Implementación de control GPIO
│   ├── API_I2C.c           # Información y diagnóstico del I2C
│   └── API_uart.c          # Implementación de comunicación UART
│
└── README.md
```

---

## Descripción de módulos

### API_cmdparser
Implementa una **Máquina de Estados Finita (MEF)** para interpretar comandos recibidos por UART.

Responsabilidades:
- Recepción de datos por UART
- Tokenización de comandos
- Parsing de strings
- Ejecución de comandos

Comandos soportados:
- `TEMP?` → Temperatura  
- `PRES?` → Presión  
- `HUM?` → Humedad  
- `HELP?` → Ayuda  
- `MENU` → Menú  
- `REBOOT` → Reinicio  
- `RESTART` → Reinicio tras error  
- `BAUD?` → Configuración UART  
- `I2C?` → Configuración I2C  

---

### API_uart
Proporciona funciones de alto nivel para comunicación UART.

Responsabilidades:
- Envío de strings
- Recepción de datos
- Configuración de baudrate
- Manejo de buffers

---

### API_GPIO
Abstrae el control de GPIO.

Responsabilidades:
- Inicialización de GPIO
- Manejo de LED de error

---

### API_I2C
Proporciona herramientas de diagnóstico del bus I2C.

Responsabilidades:
- Mostrar configuración del I2C
- Escaneo de dispositivos conectados

---

### API_common_types
Define tipos de datos comunes para todo el sistema.

Ejemplos:
- `bool_t`
- `tick_t`

---

## Funcionamiento general

La API transforma las funciones de bajo nivel del BSP en interfaces más amigables para la aplicación.

Ejemplo:

- BSP → `bsp_uart_write()`
- API → `uartSendString()`

Esto permite que la MEF trabaje a mayor nivel sin depender del hardware.

---

## Objetivo del diseño

- Abstraer el acceso al hardware
- Simplificar la lógica de aplicación
- Reutilizar código
- Mantener una arquitectura limpia

---

## Máquina de estados del parser

```plaintext
CMD_IDLE       → Estado inicial
CMD_RECEIVING  → Recepción de datos
CMD_PROCESS    → Procesamiento del comando
CMD_EXEC       → Ejecución
CMD_ERROR      → Manejo de errores
```

---

## Características destacadas

- Parser case-insensitive (no distingue mayúsculas/minúsculas)
- Manejo de overflow de buffer
- Soporte de múltiples comandos
- Echo de caracteres por UART
- Diagnóstico de hardware por comandos

---

## Notas importantes

- El parser requiere llamadas periódicas a `cmdPoll()`
- La recepción es no bloqueante (byte a byte)
- El buffer de comandos tiene tamaño limitado (`64 bytes`)
- Se soportan hasta `3 tokens` por comando

---

## 👨‍💻 Autor

Ing Espínola Maximiliano Ariel
