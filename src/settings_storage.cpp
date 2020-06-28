#include <EEPROM.h>
#include <Arduino.h>
#include "settings_storage.h"

bool compareSettings(Settings a, Settings b){
    return a.targetTemp == b.targetTemp &&
        a.targetTime == b.targetTime;
}

void settings_save(Settings settings){
    Settings oldSettings = settings_load();
    if (!compareSettings(settings, oldSettings)){
        EEPROM.put(0, settings);
        Serial.println("Saving settings");
    }
}
Settings settings_load(){
    Settings settings;
    EEPROM.get(0, settings);
    return settings;
}

