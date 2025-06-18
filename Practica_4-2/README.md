# **Práctica 4-2 - Sistemas operativos en tiempo real 2**  

## **1. Descripción**  
Esta práctica profundiza en el uso de **FreeRTOS** para la sincronización de tareas mediante semáforos binarios en el **ESP32-S3**. Se implementan dos tareas que controlan un LED: una lo enciende y otra lo apaga, coordinadas por un semáforo para evitar conflictos de acceso al recurso compartido (el pin del LED).  

---

## **2. Objetivos**  
- Implementar **sincronización entre tareas** usando semáforos binarios.  
- Garantizar el acceso exclusivo a un recurso compartido (LED) mediante FreeRTOS.  
- Analizar el comportamiento de las tareas en el monitor serie.  
- Entender el papel de los semáforos en la gestión de concurrencia.  

### **Conceptos clave**  
- **Semáforo binario**: Actúa como un "interruptor" que permite o bloquea el acceso a un recurso.  
- `xSemaphoreTake()`: Adquiere el semáforo (si está disponible).  
- `xSemaphoreGive()`: Libera el semáforo para que otra tarea lo use.  

---

## **3. Materiales**  
- **ESP32-S3**  
- **LED** conectado al pin 2.  
- **Resistencia de 220Ω** (para el LED).   
- **Cable USB-C**.  
 ![Image](https://github.com/user-attachments/assets/860c83dc-7289-4f3f-a3db-98908957f6a6)
---

## **4. Desarrollo**  

### **4.1 Configuración del entorno**  
1. Crear un nuevo proyecto en PlatformIO.  
2. Configurar el archivo `platformio.ini` con el siguiente contenido:
   ```ini
   [env:esp32-s3-devkitm-1]
   platform = espressif32
   board = esp32-s3-devkitm-1
   framework = arduino
   monitor_speed=115200
   ```  

### **4.2 Programación**  
Código en `main.cpp`:  
```cpp
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define LED_PIN 2  // Pin del LED

SemaphoreHandle_t semaforo;  // Semáforo binario

void tareaEncenderLED(void *parameter) {
    for (;;) {
        if (xSemaphoreTake(semaforo, portMAX_DELAY)) {  // Espera el semáforo
            digitalWrite(LED_PIN, HIGH);  // Enciende el LED
            Serial.println("LED ENCENDIDO");
            vTaskDelay(1000 / portTICK_PERIOD_MS);  // Espera 1 segundo
            xSemaphoreGive(semaforo);  // Libera el semáforo
        }
    }
}

void tareaApagarLED(void *parameter) {
    for (;;) {
        if (xSemaphoreTake(semaforo, portMAX_DELAY)) {  // Espera el semáforo
            digitalWrite(LED_PIN, LOW);  // Apaga el LED
            Serial.println("LED APAGADO");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            xSemaphoreGive(semaforo);  // Libera el semáforo
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    semaforo = xSemaphoreCreateBinary();  // Crea el semáforo
    if (semaforo != NULL) {
        xSemaphoreGive(semaforo);  // Inicia liberado
    }
    xTaskCreate(tareaEncenderLED, "Encender LED", 1000, NULL, 1, NULL);
    xTaskCreate(tareaApagarLED, "Apagar LED", 1000, NULL, 1, NULL);
}

void loop() {}  // No se usa
```  

**Explicación:**  
- **Semáforo binario**: Garantiza que solo una tarea (encender o apagar) acceda al LED en un momento dado.  
- **Tareas**:  
  - `tareaEncenderLED`: Toma el semáforo, enciende el LED, espera 1 segundo y lo libera.  
  - `tareaApagarLED`: Toma el semáforo, apaga el LED, espera 1 segundo y lo libera.  
- **Prioridades**: Ambas tareas tienen prioridad `1`, por lo que el planificador las alterna equitativamente.  

### **4.3 Subida y verificación**  
1. Conectar el LED al pin **2** con una resistencia de 220Ω.  
2. Cargar el código y abrir el monitor serie (115200 baudios).  

---

## **5. Resultados**  
- **Salida del monitor serie**:  
  ```plaintext
  LED ENCENDIDO
  LED APAGADO
  LED ENCENDIDO
  LED APAGADO
  ...  
  ```  
- **Comportamiento del LED**: Parpadea cada 1 segundo, alternando entre encendido y apagado.  
- **Sincronización**: El semáforo evita que ambas tareas accedan al LED simultáneamente.  

---


