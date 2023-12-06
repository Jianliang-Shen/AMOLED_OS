#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../include/ui.h"

#define MAX_APPS_NUM 5

class APP
{
public:
    APP(const uint16_t *logo_data, UI_MENU ui);
    ~APP();

    /* Draw the app's logo image. Should initialize or update idx. */
    void draw_logo();
    void set_idx(int16_t idx);
    void moving_right();
    void moving_left();
    int16_t get_idx();
    UI_MENU get_ui_idx();

private:
    int16_t _idx;               /* The index of app's location. */
    int16_t _x;                 /* AXIS x */
    int16_t _y;                 /* AXIS y */
    const uint16_t *_logo_data; /* Pointer to the const logo data */
    UI_MENU _ui;                /* Internal UI index, the initial is idx. */
    TFT_eSPI _tft_app = TFT_eSPI();
    TFT_eSprite _logo_spr = TFT_eSprite(&_tft_app);
};
