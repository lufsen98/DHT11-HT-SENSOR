#include "pico/stdlib.h"
#include <stdio.h>
#include "dht11.h"

// Define the GPIO pin connected to DHT11 data pin
#define DHT11_PIN 18 

int main() {
    // Initialize standard library
    stdio_init_all();
    
    // Wait for serial connection to be established
    sleep_ms(2000);
    
    printf("DHT11 Sensor Example\n");
    
    // Initialize DHT11 sensor
    dht11_t sensor;
    dht11_init(&sensor, DHT11_PIN);
    
    // Main loop
    while (1) {
        printf("Reading DHT11 sensor...\n");
        
        if (dht11_read(&sensor)) {
            // Read successful, print the values
            printf("Temperature: %.1f °C\n", dht11_get_temperature(&sensor));
            printf("Humidity: %.1f %%\n", dht11_get_humidity(&sensor));
        } else {
            printf("Failed to read from DHT11 sensor!\n");
        }
        
        // DHT11 sampling rate is 1Hz (once per second)
        // Wait at least 2 seconds between readings
        sleep_ms(2000);
    }
    
    return 0;
}
