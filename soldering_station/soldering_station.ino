#include "controller.h"
#include <LiquidCrystal.h>

#define MULTI_FUNC_ENCODER_PIN_1 2
#define MULTI_FUNC_ENCODER_PIN_2 3
#define MULTI_FUNC_ENCODER_BUTTON 8

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
enum mode_t {MODE_SELECT, IRON_TEMPERATURE, HOT_AIR_TEMPERATURE};
mode_t current_mode(IRON_TEMPERATURE), selected_mode(IRON_TEMPERATURE);

void multi_func_encoder();
void lcd_update();
void print_mode(mode_t mode);

void setup()
{
    controller_setup();
    lcd.begin(16, 2);
    print_mode(current_mode);
    pinMode(MULTI_FUNC_ENCODER_PIN_1, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(MULTI_FUNC_ENCODER_PIN_1), multi_func_encoder, FALLING);
    pinMode(MULTI_FUNC_ENCODER_PIN_2, INPUT_PULLUP);
    pinMode(MULTI_FUNC_ENCODER_BUTTON, INPUT_PULLUP);
}

void loop()
{
    controller_loop();
    if (digitalRead(MULTI_FUNC_ENCODER_BUTTON) == LOW)
    {
        if (current_mode == MODE_SELECT)
            current_mode = selected_mode;
        else
        {
            selected_mode = current_mode;
            current_mode = MODE_SELECT;
        }
        lcd.clear();
        print_mode(current_mode);
        delay(250);
    }

    lcd_update();
}

void lcd_update()
{
    if (current_mode == MODE_SELECT)
    {
        if ((millis() % 1500) < 1000)
            print_mode(selected_mode);
        else
            lcd.clear();
    }
    else
    {
        lcd.setCursor(0, 1);
        if (current_mode == IRON_TEMPERATURE)
            lcd.print("Temp: " + String(controller.current_iron_temperature) + "C/" + String(controller.target_iron_temperature) + "C");
        else if (current_mode == HOT_AIR_TEMPERATURE)
            lcd.print("Temp: " + String(controller.current_hot_air_temperature) + "C/" + String(controller.target_hot_air_temperature) + "C");
    }
}

void print_mode(mode_t mode)
{
    lcd.setCursor(0, 0);
    if (mode == IRON_TEMPERATURE)
        lcd.print("Mode: Iron");
    else if (mode == HOT_AIR_TEMPERATURE)
        lcd.print("Mode: Hot air");
}

volatile bool interrupt_in_progress = false;

void multi_func_encoder()
{
    if (interrupt_in_progress)
        return;
    interrupt_in_progress = true;
    bool rot_direction = digitalRead(MULTI_FUNC_ENCODER_PIN_2) == HIGH;
    delay(100);
    switch (current_mode)
    {
        case MODE_SELECT:
            if (selected_mode == IRON_TEMPERATURE)
                selected_mode = HOT_AIR_TEMPERATURE;
            else if (selected_mode == HOT_AIR_TEMPERATURE)
                selected_mode = IRON_TEMPERATURE;
            lcd.clear();
            break;
        case IRON_TEMPERATURE:
            if (rot_direction)
            {
                if (++controller.target_iron_temperature > MAX_IRON_TEMPERATURE)
                    controller.target_iron_temperature = MAX_IRON_TEMPERATURE;
            }
            else
            {
                if (--controller.target_iron_temperature < MIN_IRON_TEMPERATURE)
                    controller.target_iron_temperature = MIN_IRON_TEMPERATURE;
            }
            break;
        case HOT_AIR_TEMPERATURE:
            if (rot_direction)
            {
                if (++controller.target_hot_air_temperature > MAX_HOT_AIR_TEMPERATURE)
                    controller.target_hot_air_temperature = MAX_HOT_AIR_TEMPERATURE;
            }
            else
            {
                if (--controller.target_hot_air_temperature < MIN_HOT_AIR_TEMPERATURE)
                    controller.target_hot_air_temperature = MIN_HOT_AIR_TEMPERATURE;
            }
            break;
    }
    interrupt_in_progress = false;
}

