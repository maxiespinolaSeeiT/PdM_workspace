# Driver BMP280

Este directorio contiene el driver para el sensor **BMP280**, utilizado para la medición de **temperatura y presión atmosférica**.

El diseño sigue una arquitectura modular basada en:
- Máquina de Estados Finitos (MEF)
- Capa de abstracción de hardware (PORT)
- Separación entre lógica y acceso a periféricos

---

## Estructura

```plaintext
BMP280/
├── Inc/
│   ├── BMP280_driver.h      # Interfaz del driver (lógica de control)
│   └── BMP280_port.h        # Interfaz de acceso al hardware (I2C)
│
├── Src/
│   ├── BMP280_driver.c      # Implementación de la MEF y procesamiento
│   └── BMP280_port.c        # Implementación del acceso físico (I2C)
│
└── README.md
```

---

## Descripción de módulos

### BMP280_driver
Implementa la lógica completa del sensor mediante una **Máquina de Estados Finitos (MEF)**.

Responsabilidades:
- Inicialización del sensor
- Lectura de coeficientes de calibración internos
- Control de medición en modo *forced*
- Conversión de datos crudos (ADC) a:
  - Temperatura (°C)
  - Presión (Pa)

---

### 🔹 BMP280_port
Encapsula el acceso al hardware (I2C), desacoplando el driver de la HAL.

Responsabilidades:
- Lectura de registros
- Escritura de registros
- Manejo de delays

Permite reutilizar el driver en diferentes plataformas sin modificar la lógica.

---

## ⚙️ Funcionamiento general

1. **Inicialización**
   - Configuración del sensor
   - Lectura de coeficientes de calibración internos

2. **Medición**
   - Se inicia una medición en modo *forced*
   - Se espera la finalización (polling + timeout)

3. **Lectura**
   - Se obtienen datos crudos de presión y temperatura
   - Se aplican fórmulas de compensación del fabricante

4. **Disponibilidad de datos**
   - Los datos quedan listos para ser leídos por la aplicación

---

## Objetivo del diseño

- Separar lógica y hardware
- Facilitar reutilización
- Permitir escalabilidad
- Mantener código limpio y mantenible

---

## Requisitos

- BSP de I2C implementado
- Funciones:
  - `bsp_i2c_write`
  - `bsp_i2c_mem_read`
  - `bsp_i2c_getTick`
  - `bsp_i2c_delay`

---

## Notas importantes

- El driver utiliza **modo FORCED**, optimizando consumo energético
- Se implementa **timeout** para evitar bloqueos
- Los coeficientes de calibración son propios de cada sensor
- El driver no depende directamente de la HAL

---

## API principal

- `BMP_Init()` → Inicializa el sensor  
- `BMP_Start()` → Inicia una medición  
- `BMP280_Update()` → Ejecuta la MEF  
- `BMP280_IsReady()` → Indica si hay datos disponibles  
- `BMP280_GetData()` → Obtiene temperatura y presión  

---

## Autor

Espínola Maximiliano Ariel
