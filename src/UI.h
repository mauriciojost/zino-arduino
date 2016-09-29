
#include <Arduino.h>

#define STATE_DISPLAY 0x0000
#define STATE_CONFIG  0x0001
#define STATE_RUN   0x0004

#define MIN_WATER_PERIOD_HOURS 1
#define MAX_WATER_PERIOD_HOURS 24*15
#define DEFAULT_WATER_PERIOD_HOURS 24*2

const uint32_t TIMER_PERIOD_UI_MS = 10000L;
const uint32_t TIMER_PERIOD_SLEEP_MS = 100000L;


class UI {

public:

  UI() {
    this->state = STATE_DISPLAY;
    this->water_period_hours = DEFAULT_WATER_PERIOD_HOURS;
    this->timer_period_ms = TIMER_PERIOD_UI_MS;
  }

  // Interruptions in case of button pressed or timer
  void go(uint8_t b0, uint8_t b1, uint8_t t0) {

    switch (this->state) {

        case STATE_RUN:
            if      (t0 == HIGH) { refresh_wdt(); do_water(); }
            else if (b0 == HIGH || b1 == HIGH)      { to_state_display(); }
            break;
        case STATE_DISPLAY:
            if      (b0 == HIGH) { to_state_run(); }
            else if (b1 == HIGH) { to_state_config(); }
            break;
        case STATE_CONFIG:
            if      (b0 == HIGH) { to_state_display(); }
            else if (b1 == HIGH) { increase_water_period(); }
            break;
        default:
            break;
    }

  }
  uint32_t get_timer_period_ms() {
      return this->timer_period_ms;
  }

private:
    uint8_t state;
    uint32_t water_period_hours;
    uint32_t timer_period_ms;

    void to_state_display() {
        set_timer_ms(TIMER_PERIOD_UI_MS);
        set_state(STATE_DISPLAY);
    }

    void to_state_run() {
        set_timer_ms(TIMER_PERIOD_SLEEP_MS);
        set_state(STATE_RUN);
    }

    void to_state_config() {
        // do whatever it takes to configure the module
        set_state(STATE_CONFIG);
    }


    void do_water() {
        // do the watering
    }

    void increase_water_period() {
        this->water_period_hours = constrain(this->water_period_hours + 1, MIN_WATER_PERIOD_HOURS, MAX_WATER_PERIOD_HOURS);
    }

    void refresh_wdt() {
        // set the WDT
    }

    void set_state(uint8_t new_state){
        this->state = new_state;
    }

    void set_timer_ms(uint32_t timer_period){
        this->timer_period_ms = timer_period;
    }

};
