
#include <Arduino.h>
#include <string.h>
#include <cmath>
#include <TFT_eSPI.h>
#include <OneButton.h>
#include <WiFi.h>
#include "../include/rm67162.h"
#include "../include/font.h"
#include "../include/sevenSeg.h"
#include "../include/logos.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
TFT_eSprite clock_spr = TFT_eSprite(&tft);

#define MAX_APPS_NUM 5

class APP
{
public:
    APP(const uint16_t *logo_data){
        _logo_data = logo_data;
    };

    void draw_logo()
    {
        if (_idx >= 0 && _idx <= 2)
        {
            TFT_eSprite logo_spr = TFT_eSprite(&tft);
            logo_spr.createSprite(120, 120);
            logo_spr.setSwapBytes(true);

            logo_spr.pushImage(0, 0, 120, 120, _logo_data);
            lcd_PushColors(_x, _y, 120, 120, (uint16_t *)logo_spr.getPointer());
        }
    };

    void set_idx(uint16_t idx)
    {
        _idx = idx;

        if (_idx > MAX_APPS_NUM)
        {
            printf("idx should be smaller than 3\n");
        };
        if (idx == MAX_APPS_NUM)
        {
            _idx = 0;
        }

        _x = _idx * 164 + 44;

        draw_logo();
    };

    uint16_t get_idx()
    {
        return _idx;
    }

private:
    uint16_t _idx = 0, _x = 0, _y = 90;
    const uint16_t *_logo_data;
};

APP weather_app(weather_logo);
APP clock_app(clock_logo);
APP picture_app(picture_logo);
APP wifi_app(wifi_logo);
APP setup_app(setup_logo);

APP apps[MAX_APPS_NUM] = {weather_app, clock_app, picture_app, wifi_app, setup_app};

#define up 21
#define down 0

OneButton button_up(up, true);
OneButton button_down(down, true);

void draw_background(){
    sprite.createSprite(536, 240);
    sprite.setSwapBytes(true);

    /* Set background. */
    sprite.fillSprite(TFT_BLACK);
    sprite.fillSmoothRoundRect(0, 0, 536, 240, 30, TFT_WHITE, TFT_BLACK);
    sprite.setTextColor(TFT_BLACK, TFT_WHITE);

    /* Draw title. */
    // sprite.setFreeFont(&DSEG14_Classic_Regular_28);
    sprite.setFreeFont(myfonts[8]);
    sprite.drawString("23-12-03 18:56", 15, 15);
    sprite.drawString("192.168.10.1", 330, 15);

    /* Draw select rect. */
    // 44 120 44 120 44 120 44
    // 178, 70: 318, 210
    sprite.fillSmoothRoundRect(196, 78, 144, 144, 30, TFT_BLACK, TFT_WHITE);
    sprite.fillSmoothRoundRect(200, 82, 136, 136, 26, TFT_WHITE, TFT_BLACK);

    lcd_PushColors(0, 0, 536, 240, (uint16_t *)sprite.getPointer());
}

void up_click()
{
}

void up_doubleclick()
{
}

void down_click()
{
    for(int i = 0; i < MAX_APPS_NUM; i++){
        apps[i].set_idx(apps[i].get_idx() + 1);
    }
}

void down_doubleclick()
{
}

void init_button()
{
    pinMode(up, INPUT_PULLUP);
    pinMode(down, INPUT_PULLUP);

    button_up.attachClick(up_click);
    button_up.attachDoubleClick(up_doubleclick);

    button_down.attachClick(down_click);
    button_down.attachDoubleClick(down_doubleclick);
}

void loop_button()
{
    // button_up.tick();
    button_down.tick();
}

void setup()
{
    init_button();

    /* Amoled lcd initialization. */
    rm67162_init();
    lcd_setRotation(1);
    lcd_brightness(150);

    draw_background();

    /* Init Apps. */
    for (int i = 0; i < MAX_APPS_NUM; i++)
    {
        apps[i].set_idx(i);
    }

    /* Init serial. */
    Serial.begin(115200);
}

void loop()
{
    loop_button();
}
