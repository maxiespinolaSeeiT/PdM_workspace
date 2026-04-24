# Driver AHT20

Este directorio contiene el driver para el sensor de temperatura y humedad **AHT20**, implementado siguiendo una arquitectura modular basada en:

- Máquina de Estados Finitos (MEF)
- Capa de abstracción de hardware (PORT)
- Separación entre lógica y acceso a periféricos

```plaintext
AHT20/
├── Inc/
│   ├── ATH20_MEF_driver.h   # Interfaz del driver (lógica de control)
│   └── ATH20_port.h         # Interfaz de acceso al hardware (I2C)
│
├── Src/
│   ├── ATH20_MEF_driver.c   # Implementación de la MEF del sensor
│   └── ATH20_port.c         # Implementación del acceso físico (I2C)
│
└── README.md
```

## Descripción de módulos

### ATH20_MEF_driver
Implementa la lógica de funcionamiento del sensor mediante una máquina de estados finitos.

Responsabilidades:
- Inicialización del sensor
- Secuencia de medición
- Lectura de datos
- Conversión de datos crudos a magnitudes físicas

---

### ATH20_port
Encapsula el acceso al hardware, desacoplando el driver de la HAL.

Responsabilidades:
- Comunicación I2C con el sensor
- Envío de comandos
- Lectura de registros

Permite cambiar de hardware o HAL sin modificar la lógica del driver.

---

## Funcionamiento general

1. Se inicializa el driver
2. La MEF gestiona:
   - Estado de espera
   - Disparo de medición
   - Espera de conversión
   - Lectura de datos
3. Los datos se procesan y quedan disponibles para la aplicación

---

## Objetivo del diseño

- Separar lógica y hardware
- Facilitar reutilización del driver
- Permitir escalabilidad
- Mejorar mantenibilidad

---

## Requisitos

- Peripheral I2C configurado
- BSP funcional para I2C
- Temporización (delay o tick)

---

## Notas

- El driver no accede directamente a la HAL
- Toda interacción con hardware se realiza mediante la capa `port`
- Diseñado para sistemas embebidos basados en STM32

---

## Autor

Ing. Espínola Maximiliano Ariel
