// dht11.h - Header file for the DHT11 library
#ifndef DHT11_H
#define DHT11_H

#include "pico/stdlib.h"
#include <stdbool.h>

// DHT11 sensor data structure
typedef struct {
    uint gpio_pin;       // GPIO pin connected to DHT11 sensor
    float temperature;   // Last read temperature in Celsius
    float humidity;      // Last read humidity percentage
    bool last_read_ok;   // Status of the last read operation
} dht11_t;

/**
 * Initialize a DHT11 sensor
 * @param dht Pointer to DHT11 structure
 * @param gpio_pin GPIO pin connected to the DHT11 sensor
 */
void dht11_init(dht11_t *dht, uint gpio_pin);

/**
 * Read temperature and humidity data from DHT11
 * @param dht Pointer to DHT11 structure
 * @return true if read successful, false otherwise
 */
bool dht11_read(dht11_t *dht);

/**
 * Get the last read temperature
 * @param dht Pointer to DHT11 structure
 * @return Temperature in Celsius
 */
float dht11_get_temperature(const dht11_t *dht);

/**
 * Get the last read humidity
 * @param dht Pointer to DHT11 structure
 * @return Humidity percentage
 */
float dht11_get_humidity(const dht11_t *dht);

#endif // DHT11_H
