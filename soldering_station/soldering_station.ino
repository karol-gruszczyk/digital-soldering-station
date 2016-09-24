#include "settings.h"
#include "multi_func.h"


void setup()
{
    load_settings();
    multi_func_setup();
}

void loop()
{
	multi_func_loop();
}
