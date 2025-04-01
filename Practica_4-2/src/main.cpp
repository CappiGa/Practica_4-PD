#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define LED_PIN 2  // Pin donde está conectado el LED

SemaphoreHandle_t semaforo;  // Declaración del semáforo

void tareaEncenderLED(void *parameter) {
    for (;;) {
        if (xSemaphoreTake(semaforo, portMAX_DELAY)) {  // Espera a que el semáforo esté disponible
            digitalWrite(LED_PIN, HIGH);  // Enciende el LED
            Serial.println("LED ENCENDIDO");
            vTaskDelay(1000 / portTICK_PERIOD_MS);  // Espera 1 segundo
            xSemaphoreGive(semaforo);  // Libera el semáforo para que la otra tarea pueda ejecutarse
        }
    }
}

void tareaApagarLED(void *parameter) {
    for (;;) {
        if (xSemaphoreTake(semaforo, portMAX_DELAY)) {  // Espera a que el semáforo esté disponible
            digitalWrite(LED_PIN, LOW);  // Apaga el LED
            Serial.println("LED APAGADO");
            vTaskDelay(1000 / portTICK_PERIOD_MS);  // Espera 1 segundo
            xSemaphoreGive(semaforo);  // Libera el semáforo para que la otra tarea pueda ejecutarse
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);

    semaforo = xSemaphoreCreateBinary();  // Creación del semáforo

    if (semaforo != NULL) {
        xSemaphoreGive(semaforo);  // Habilita el semáforo al inicio
    }

    xTaskCreate(tareaEncenderLED, "Encender LED", 1000, NULL, 1, NULL);
    xTaskCreate(tareaApagarLED, "Apagar LED", 1000, NULL, 1, NULL);
}

void loop() {
    // No se usa, ya que las tareas se ejecutan en FreeRTOS
}
