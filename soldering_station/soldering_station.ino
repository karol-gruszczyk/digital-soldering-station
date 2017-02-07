#include "settings.h"
#include "controller.h"
#include <LiquidCrystal.h>

#define MULTI_FUNC_ENCODER_PIN_1 2
#define MULTI_FUNC_ENCODER_PIN_2 3
#define MULTI_FUNC_ENCODER_BUTTON 8

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
enum mode_t {MODE_SELECT, IRON_TEMPERATURE, HOT_AIR_TEMPERATURE, HOT_AIR_SPEED};
mode_t current_mode(IRON_TEMPERATURE), selected_mode(HOT_AIR_TEMPERATURE);

void multi_func_encoder();
void lcd_update();
void print_mode(mode_t mode);

void setup()
{
    load_settings();
    lcd.begin(16, 2);
    print_mode(current_mode);
    //attachInterrupt(digitalPinToInterrupt(MULTI_FUNC_ENCODER_PIN_1), multi_func_encoder, RISING);
    //pinMode(MULTI_FUNC_ENCODER_PIN_2, INPUT);
    //pinMode(MULTI_FUNC_ENCODER_BUTTON, INPUT);
}

void loop()
{
    if (digitalRead(MULTI_FUNC_ENCODER_BUTTON) == LOW && false)
    {
        if (current_mode == MODE_SELECT)
        {
            current_mode = selected_mode;
            
        }
        else
        {
            selected_mode = current_mode;
            current_mode = MODE_SELECT;
            save_settings();
        }
        lcd.clear();
        print_mode(current_mode);
    }

    lcd_update();
}

void lcd_update()
{
    if (current_mode == MODE_SELECT)
    {
        if ((millis() % 1500) < 1000)
        {
            print_mode(selected_mode);
        }
        else
        {
            lcd.clear();
        }
    }
    else
    {
        lcd.setCursor(0, 1);
        if (current_mode == IRON_TEMPERATURE)
            lcd.print("Temp: " + String(current_iron_temperature) + "C/" + String(settings.iron_target_temperature) + "C");
        else if (current_mode == HOT_AIR_TEMPERATURE)
            lcd.print("Temp: " + String(settings.hot_air_speed) + "%/" + String(MAX_HOT_AIR_SPEED) + "%");
        else if (current_mode == HOT_AIR_SPEED)
            lcd.print("Mode: Fan speed");
    }
}

void print_mode(mode_t mode)
{
    lcd.setCursor(0, 0);
    if (mode == IRON_TEMPERATURE)
        lcd.print("Mode: Iron");
    else if (mode == HOT_AIR_TEMPERATURE)
        lcd.print("Mode: Hot air");
    else if (mode == HOT_AIR_SPEED)
        lcd.print("Mode: Fan speed");
}

void multi_func_encoder()
{
    bool direction = digitalRead(MULTI_FUNC_ENCODER_PIN_2) == HIGH;
    switch (current_mode)
    {
        case MODE_SELECT:
            if (direction)
            {
                if (selected_mode == IRON_TEMPERATURE)
                    selected_mode = HOT_AIR_TEMPERATURE;
                else if (selected_mode == HOT_AIR_TEMPERATURE)
                    selected_mode = HOT_AIR_SPEED;
            }
            else
            {
                if (selected_mode == HOT_AIR_SPEED)
                    selected_mode = HOT_AIR_TEMPERATURE;
                else if (selected_mode == HOT_AIR_TEMPERATURE)
                    selected_mode = IRON_TEMPERATURE;
            }
            break;
        case IRON_TEMPERATURE:
            if (direction)
            {
                if (++settings.iron_target_temperature > MAX_IRON_TEMPERATURE)
                    settings.iron_target_temperature = MAX_IRON_TEMPERATURE;
            }
            else
            {
                if (--settings.iron_target_temperature < MIN_IRON_TEMPERATURE)
                    settings.iron_target_temperature = MIN_IRON_TEMPERATURE;
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

