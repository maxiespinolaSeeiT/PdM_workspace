# Driver LCD (16x2 con PCF8574 - I2C)

Este directorio contiene el driver para una pantalla **LCD 16x2** controlada mediante un **expansor I2C PCF8574**.

El diseño sigue una arquitectura modular basada en:
- Separación entre lógica y hardware (Driver + Port)
- Comunicación I2C mediante BSP
- Manejo en modo de 4 bits

---

## Estructura

```plaintext
LCD/
├── Inc/
│   ├── LCD_driver.h     # Interfaz de alto nivel del LCD
│   └── LCD_port.h       # Interfaz de acceso al hardware (I2C)
│
├── Src/
│   ├── LCD_driver.c     # Implementación del protocolo LCD (4 bits)
│   └── LCD_port.c       # Implementación del acceso físico (I2C)
│
└── README.md
```

---

## Descripción de módulos

### LCD_driver
Implementa la lógica de control del display LCD.

Responsabilidades:
- Inicialización del LCD en modo 4 bits
- Envío de comandos y datos
- Control de cursor
- Escritura de strings

---

### LCD_port
Encapsula el acceso al hardware (I2C), desacoplando el driver de la HAL.

Responsabilidades:
- Inicialización del bus I2C
- Envío de datos al expansor PCF8574
- Manejo de delays

---

## Funcionamiento general

El LCD trabaja en **modo de 4 bits**, por lo que cada byte se envía en dos partes (nibbles):

1. Se envía el nibble alto
2. Se envía el nibble bajo
3. Se genera un pulso en el pin **EN** para validar el dato

El expansor **PCF8574** se encarga de convertir la comunicación I2C en señales paralelas para el LCD.

---

## Mapeo de control (PCF8574)

- `RS` → Selección comando/dato  
- `RW` → Lectura/escritura (no utilizado)  
- `EN` → Enable (pulso de validación)  
- `BL` → Backlight  

---

## API principal

- `LCD_Init()` → Inicializa el display  
- `LCD_Clear()` → Borra la pantalla  
- `LCD_SetCursor(row, col)` → Posiciona el cursor  
- `LCD_WriteString(char *str)` → Escribe un texto  
- `LCD_SendCommand(cmd)` → Envía comando  
- `LCD_SendData(data)` → Envía dato  

---

## Objetivo del diseño

- Separar lógica del hardware
- Facilitar reutilización del driver
- Permitir cambio de plataforma sin modificar la lógica
- Mantener código simple y mantenible

---

## Requisitos

- BSP de I2C implementado
- Funciones requeridas:
  - `bsp_i2c_write`
  - `bsp_i2c_delay`
  - `I2C_init`

---

## Notas importantes

- El LCD utiliza **modo 4 bits** para reducir pines
- Se requiere respetar los **delays de inicialización**
- La dirección I2C del módulo puede variar (por defecto `0x27` modificar line 15 ADDR_I2C_LCD_PCF8574T si necesita otra dirección)
- No se utiliza lectura del LCD (RW siempre en escritura)

---

## Autor

Ing. Espínola Maximiliano Ariel
