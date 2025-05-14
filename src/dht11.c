// dht11.c - Implementation file for the DHT11 library
#include "dht11.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>

// DHT11 timing constants (in microseconds)
#define DHT11_START_SIGNAL_TIME 18000
#define DHT11_RESPONSE_TIME 40
#define DHT11_BIT_LOW_TIME 50
#define DHT11_BIT_HIGH_0_TIME 26
#define DHT11_BIT_HIGH_1_TIME 70
#define DHT11_TIMEOUT 100000  // 100ms timeout

// Define timeout for reading bits
#define DHT11_READ_TIMEOUT 1000

void dht11_init(dht11_t *dht, uint gpio_pin) {
    dht->gpio_pin = gpio_pin;
    dht->temperature = 0.0f;
    dht->humidity = 0.0f;
    dht->last_read_ok = false;
    
    // Initialize GPIO
    gpio_init(gpio_pin);
}

static inline uint64_t get_time_us() {
    return to_us_since_boot(get_absolute_time());
}

static bool dht11_wait_for_edge(uint gpio_pin, bool level, uint64_t timeout_us) {
    uint64_t start = get_time_us();
    while (gpio_get(gpio_pin) != level) {
        if (get_time_us() - start > timeout_us) {
            return false;  // Timeout
        }
        tight_loop_contents();  // Optimize for tight timing
    }
    return true;
}

bool dht11_read(dht11_t *dht) {
    uint8_t data[5] = {0, 0, 0, 0, 0};
    uint64_t start_time;
    
    // Reset last_read_ok flag
    dht->last_read_ok = false;
    
    // 1. Send start signal
    gpio_set_dir(dht->gpio_pin, GPIO_OUT);
    gpio_put(dht->gpio_pin, 0);       // Pull low
    sleep_us(DHT11_START_SIGNAL_TIME); // Hold low for at least 18ms
    gpio_put(dht->gpio_pin, 1);       // Pull high
    sleep_us(40);                     // Hold high for 40us
    
    // 2. Switch to input mode and wait for DHT11 response
    gpio_set_dir(dht->gpio_pin, GPIO_IN);
    
    // Wait for the sensor to pull the line low
    if (!dht11_wait_for_edge(dht->gpio_pin, 0, DHT11_READ_TIMEOUT)) {
        return false;
    }
    
    // Wait for the sensor to pull the line high
    if (!dht11_wait_for_edge(dht->gpio_pin, 1, DHT11_READ_TIMEOUT)) {
        return false;
    }
    
    // Wait for the sensor to pull the line low again to start data transmission
    if (!dht11_wait_for_edge(dht->gpio_pin, 0, DHT11_READ_TIMEOUT)) {
        return false;
    }
    
    // 3. Read 40 bits of data (5 bytes)
    for (int i = 0; i < 40; i++) {
        // Wait for rising edge (start of bit)
        if (!dht11_wait_for_edge(dht->gpio_pin, 1, DHT11_READ_TIMEOUT)) {
            return false;
        }
        
        start_time = get_time_us();
        
        // Wait for falling edge (end of bit)
        if (!dht11_wait_for_edge(dht->gpio_pin, 0, DHT11_READ_TIMEOUT)) {
            return false;
        }
        
        // Measure high pulse width to determine bit value
        uint64_t pulse_width = get_time_us() - start_time;
        
        // If high pulse is longer than ~50us, it's a '1' bit
        // DHT11 sends a '1' with ~70us high pulse, a '0' with ~26-28us high pulse
        if (pulse_width > 40) {  // Middle point between 0 and 1 timing
            data[i / 8] |= (1 << (7 - (i % 8)));
        }
    }
    
    // 4. Verify checksum (last byte should be sum of first 4 bytes)
    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if (checksum != data[4]) {
        return false;
    }
    
    // 5. Parse data: first two bytes are humidity, next two are temperature
    dht->humidity = (float)data[0] + (data[1] / 10.0f);
    dht->temperature = (float)data[2] + (data[3] / 10.0f);
    
    // Mark read as successful
    dht->last_read_ok = true;
    
    return true;
}

float dht11_get_temperature(const dht11_t *dht) {
    return dht->temperature;
}

float dht11_get_humidity(const dht11_t *dht) {
    return dht->humidity;
}(
