#ifndef HEATER_BUTTONS_
#define HEATER_BUTTONS_

#define BTN_LEFT 0
#define BTN_RIGHT 1
#define BTN_UP 2
#define BTN_DOWN 3
#define BTN_SELECT 4
#define BTN_NONE 4000
bool btns_pressed(int button);
void btns_update();
void btns_init();

#endif
