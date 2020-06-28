#ifndef HEATER_HEATER_
#define HEATER_HEATER_
#define HYSTERESIS 2
#define ERROR_CHECK_TIME 60000
#define ERROR_CHECK_DIFF (HYSTERESIS * 10)
class Heater {
    public:
        void set_targetTemp(float temp);
        void set_current(float temp);
        void set_enabled(bool enabled);
        bool get_signal();
        bool get_ready();
        bool is_error();
        void update();
    private:
        int pin;
        float prev_value = 0;
        bool enabled = false;
        bool error = false;
        bool heating = false;
        float current = 0;
        float targetTemp = 0;
        unsigned long prev_time = 0;
        bool ready;

        
};
#endif