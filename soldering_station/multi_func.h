#include "settings.h"

#define MULTI_FUNC_ENCODER_PIN_1 3
#define MULTI_FUNC_ENCODER_PIN_2 4
#define MULTI_FUNC_ENCODER_BUTTON 5

enum mode_t {MODE_SELECT, TEMPERATURE, HOT_AIR_TEMPERATURE, HOT_AIR_SPEED};
mode_t selected_mode(MODE_SELECT), current_mode(TEMPERATURE);


void multi_func_encoder()
{
    bool direction = digitalRead(MULTI_FUNC_ENCODER_PIN_2) == HIGH;
    switch (current_mode)
    {
        case MODE_SELECT:
            if (direction)
            {
                if (selected_mode == TEMPERATURE)
                    selected_mode = HOT_AIR_TEMPERATURE;
                else if (selected_mode == HOT_AIR_TEMPERATURE)
                    selected_mode = HOT_AIR_SPEED;
            }
            else
            {
                if (selected_mode == HOT_AIR_SPEED)
                    selected_mode = HOT_AIR_TEMPERATURE;
                else if (selected_mode == HOT_AIR_TEMPERATURE)
                    selected_mode = TEMPERATURE;
            }
            break;
        case TEMPERATURE:
            if (direction)
            {
                if (++settings.target_temperature > MAX_TEMPERATURE)
                    settings.target_temperature = MAX_TEMPERATURE;
            }
            else
            {
                if (--settings.target_temperature < MIN_TEMPERATURE)
                    settings.target_temperature = MIN_TEMPERATURE;
            }
            break;
        case HOT_AIR_TEMPERATURE:
            if (direction)
            {
                if (++settings.hot_air_target_temperature > MAX_HOT_AIR_TEMPERATURE)
                    settings.hot_air_target_temperature = MAX_HOT_AIR_TEMPERATURE;
            }
            else
            {
                if (--settings.hot_air_target_temperature < MIN_HOT_AIR_TEMPERATURE)
                    settings.hot_air_target_temperature = MIN_HOT_AIR_TEMPERATURE;
            }
            break;
        case HOT_AIR_SPEED:
            if (direction)
            {
                if (++settings.hot_air_speed > MAX_HOT_AIR_SPEED)
                    settings.hot_air_speed = MAX_HOT_AIR_SPEED;
            }
            else
            {
                if (--settings.hot_air_speed < MIN_HOT_AIR_SPEED)
                    settings.hot_air_speed = MIN_HOT_AIR_SPEED;
            }
            break;
    }
}

void multi_func_setup()
{
    attachInterrupt(digitalPinToInterrupt(MULTI_FUNC_ENCODER_PIN_1), multi_func_encoder, RISING);
    pinMode(MULTI_FUNC_ENCODER_PIN_2, INPUT);
    pinMode(MULTI_FUNC_ENCODER_BUTTON, INPUT);
}

void multi_func_loop()
{
    if (digitalRead(MULTI_FUNC_ENCODER_BUTTON) == LOW)
    {
        if (current_mode == MODE_SELECT)
        {
            current_mode = selected_mode;
        }
        else
        {
            current_mode == MODE_SELECT;
            save_settings();
        }
    }
}
