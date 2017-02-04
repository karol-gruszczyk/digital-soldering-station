#ifndef SETTINGS_H_
#define SETTINGS_H_

#define MAX_IRON_TEMPERATURE 480
#define MIN_IRON_TEMPERATURE 100
#define MAX_HOT_AIR_TEMPERATURE 480
#define MIN_HOT_AIR_TEMPERATURE 100
#define MAX_HOT_AIR_SPEED 100
#define MIN_HOT_AIR_SPEED 10

struct settings_t
{
    uint16_t iron_target_temperature;
    uint16_t hot_air_target_temperature;
    uint16_t hot_air_speed;
} settings;


void load_settings()
{
    eeprom_read_block(&settings, NULL, sizeof(settings)); 
}

void save_settings()
{
    eeprom_write_block(&settings, NULL, sizeof(settings));
}

#endif /* SETTINGS_H_ */
