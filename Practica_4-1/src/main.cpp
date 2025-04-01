#include <Arduino.h>

// Prototipo de la función anotherTask
void anotherTask(void * parameter);

void setup() {
    Serial.begin(115200);  // Corrige la velocidad a 115200 (112500 no es estándar)

    /* Creamos una nueva tarea */
    xTaskCreate(
        anotherTask,   /* Función de la tarea */
        "another Task", /* Nombre de la tarea */
        10000,         /* Tamaño de la pila */
        NULL,          /* Parámetro de la tarea */
        1,             /* Prioridad de la tarea */
        NULL           /* Manejador de la tarea */
    );
}

/* La función loop() es ejecutada en la tarea principal de Arduino */
void loop() {
    Serial.println("This is ESP32 Task");
    delay(1000);
}

/* Función de la tarea adicional */
void anotherTask(void * parameter) {
    for(;;) { // Bucle infinito
        Serial.println("This is another Task");
        delay(1000);
    }
    // Borramos la tarea si finaliza (esto nunca sucede por el bucle infinito)
    vTaskDelete(NULL);
}