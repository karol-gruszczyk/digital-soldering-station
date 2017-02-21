#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#define MAX_IRON_TEMPERATURE 400
#define MIN_IRON_TEMPERATURE 100
#define MAX_HOT_AIR_TEMPERATURE 400
#define MIN_HOT_AIR_TEMPERATURE 100

#define IRON_HEATER_PIN 0
#define IRON_THERMISTOR_PIN A0
#define IRON_REFERENCE_RESISTANCE 98.3
#define IRON_THERMISTOR_MIN_RESISTANCE 50.0  // at room temperature - 20C
#define IRON_THERMISTOR_MAX_RESISTANCE 200.0  // when iron is hot - 400C

struct controller_t 
{
    uint16_t current_iron_temperature = 0;
    uint16_t target_iron_temperature = 300;
    float iron_thermistor_offset;
    float iron_thermistor_factor;
    
    uint16_t current_hot_air_temperature = 0;
    uint16_t target_hot_air_temperature = 300;
    
} controller;

void controller_setup()
{
    float min_reading = IRON_THERMISTOR_MIN_RESISTANCE / (IRON_REFERENCE_RESISTANCE + IRON_THERMISTOR_MIN_RESISTANCE) * 1024;
    float max_reading = IRON_THERMISTOR_MAX_RESISTANCE / (IRON_REFERENCE_RESISTANCE + IRON_THERMISTOR_MAX_RESISTANCE) * 1024;
    float iron_range = max_reading - min_reading;
    controller.iron_thermistor_factor = (400 - 20) / iron_range;  // temperature range from 20C to 400C
    controller.iron_thermistor_offset = min_reading * controller.iron_thermistor_factor - 20;  // subtract room temperature
    Serial.begin(9600);
}

void controller_loop()
{
    int raw_iron = analogRead(IRON_THERMISTOR_PIN);
    controller.current_iron_temperature = raw_iron * controller.iron_thermistor_factor - controller.iron_thermistor_offset;
    if (controller.current_iron_temperature < controller.target_iron_temperature)
        digitalWrite(IRON_HEATER_PIN, HIGH);
    else
        digitalWrite(IRON_HEATER_PIN, LOW);
}

#endif /* CONTROLLER_H_ */

