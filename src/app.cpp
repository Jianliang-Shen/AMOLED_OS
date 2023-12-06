#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../include/ui.h"
#include "../include/app.h"
#include "../include/rm67162.h"

APP::APP(const uint16_t *logo_data, UI_MENU ui)
{
    this->_idx = 0;
    this->_x = 0;
    this->_y = 60;

    this->_logo_data = logo_data;
    this->_ui = ui;

    this->_logo_spr.createSprite(120, 120);
    this->_logo_spr.setSwapBytes(true);
    this->_logo_spr.pushImage(0, 0, 120, 120, _logo_data);

    /* The initial index of APP comes from the ui_number. */
    set_idx(ui);
}

APP::~APP() {
    delete[] _logo_data;
}

void APP::draw_logo()
{
    lcd_PushColors(_x, _y, 120, 120, (uint16_t *)_logo_spr.getPointer());
};

void APP::set_idx(int16_t idx)
{
    _idx = idx;

    /* APPs work as a de-queue. */
    if (idx == MAX_APPS_NUM)
    {
        _idx = 0;
    }
    if (idx < 0)
    {
        _idx = MAX_APPS_NUM - 1;
    }

    /* Calculate the relative _x. */
    _x = _idx * 130 - 52;
};

void APP::moving_right()
{
    set_idx(_idx + 1);
    draw_logo();
};

void APP::moving_left()
{
    set_idx(_idx - 1);
    draw_logo();
}

int16_t APP::get_idx()
{
    return _idx;
}

UI_MENU APP::get_ui_idx()
{
    return _ui;
};
