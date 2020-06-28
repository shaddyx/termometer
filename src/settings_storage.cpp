#include <EEPROM.h>
#include <Arduino.h>
#include "config.h"
#include "settings_storage.h"

bool compareSettings(Settings a, Settings b){
    return a.targetTemp == b.targetTemp &&
        a.targetTime == b.targetTime;
}

Settings fixSettings(Settings settings){
    if (isnan(settings.targetTemp)){
        settings.targetTemp = 0;
    }
    if (settings.targetTemp > 1000){
        settings.targetTemp = 0;
    }
    if (settings.targetTime > HOUR){
        settings.targetTime = 0;
    }
    return settings;
}

void settings_save(Settings settings){
    Settings oldSettings = settings_load();
    if (!compareSettings(settings, oldSettings)){
        EEPROM.put(0, settings);
        Serial.println("Saving settings");
        delay(1000);
    }
}
Settings settings_load(){
    Settings settings;
    EEPROM.get(0, settings);
    return fixSettings(settings);
}

