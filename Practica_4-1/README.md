# **Práctica 4-1 - Sistemas operativos en tiempo real**  

## **1. Descripción**  
Esta práctica introduce el uso de **FreeRTOS** en el **ESP32-S3** para gestionar múltiples tareas de manera concurrente mediante un sistema operativo en tiempo real (RTOS). Se implementarán dos tareas: una en el bucle principal (`loop`) y otra creada manualmente, demostrando cómo el planificador de FreeRTOS distribuye el tiempo de la CPU entre ellas.  

---

## **2. Objetivos**  
- Comprender el funcionamiento básico de **FreeRTOS** en ESP32.  
- Crear y gestionar tareas concurrentes.  
- Analizar la salida del puerto serie para observar la ejecución paralela de tareas.  
- Diferenciar entre el uso de `delay()` y `vTaskDelay()`.  

## **3. Resumen teórico**  
FreeRTOS permite la **multitarea** en el ESP32, asignando tiempo de CPU a diferentes tareas según su prioridad. Cada tarea puede:  
- Ejecutarse en un núcleo específico (`xTaskCreatePinnedToCore`).  
- Ser eliminada (`vTaskDelete`).  
- Tener prioridades (0-24, donde 24 es la máxima).  
En esta práctica, se usa `xTaskCreate()` para generar una tarea adicional que coexiste con el `loop()` principal.  

---

## **4. Materiales**  
- **ESP32-S3**  
- **Cable USB tipo C**  
 
---

## **5. Desarrollo**  

### **5.1 Preparación del entorno**  
1. Crear un nuevo proyecto en PlatformIO
2. Configurar el archivo `platformio.ini` con el siguente contenido:  
   ```ini
   [env:esp32-s3-devkitm-1]
   platform = espressif32
   board = esp32-s3-devkitm-1
   framework = arduino
   monitor_speed = 115200
   ```  

### **4.2 Programación**  
Código en `main.cpp`:  
```cpp
#include <Arduino.h>

void anotherTask(void * parameter); // Prototipo

void setup() {
    Serial.begin(115200);
    xTaskCreate(
        anotherTask,    // Función de la tarea
        "another Task", // Nombre
        10000,          // Tamaño de pila (bytes)
        NULL,           // Parámetros
        1,              // Prioridad
        NULL            // Manejador
    );
}

void loop() {
    Serial.println("This is ESP32 Task");
    delay(1000);
}

void anotherTask(void * parameter) {
    for(;;) {
        Serial.println("This is another Task");
        delay(1000);
    }
    vTaskDelete(NULL); // Nunca se ejecuta (bucle infinito)
}
```  

**Explicación:**  
- **`setup()`**: Inicia el puerto serie y crea una nueva tarea (`anotherTask`) con prioridad `1`.  
- **`loop()`**: Tarea principal que imprime un mensaje cada segundo.  
- **`anotherTask()`**: Tarea secundaria con un bucle infinito que también imprime mensajes.  

### **4.3 Subida y monitorización**  
1. Compilar y cargar el código en el ESP32-S3.  
2. Abrir el **monitor serie** (115200 baudios) para observar las salidas.  

---

## **5. Resultados**  
- **Salida del puerto serie**:  
  ```plaintext
  This is ESP32 Task
  This is another Task
  This is ESP32 Task
  This is another Task
  ...  
  ```  
- **Funcionamiento**:  
  Ambas tareas se ejecutan **concurrentemente**, compartiendo el tiempo de CPU. FreeRTOS alterna entre ellas gracias al `delay(1000)`, que cede el control al planificador.  

---

