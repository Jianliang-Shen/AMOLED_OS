
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

/******************************************************************************/
/* TFT definition                                                             */
/******************************************************************************/
TFT_eSPI tft = TFT_eSPI();

/******************************************************************************/
/* UI definition                                                              */
/******************************************************************************/
typedef enum
{
    UI_PICTURE = 0,
    UI_SETUP = 1,
    UI_CLOCK = 2,
    UI_WEATHER = 3,
    UI_WIFI = 4,

    UI_MAIN = 5,
} UI_MENU;

UI_MENU cur_ui;

/******************************************************************************/
/* APP class definition                                                       */
/******************************************************************************/
#define MAX_APPS_NUM 5

class APP
{
public:
    APP(const uint16_t *logo_data, UI_MENU ui)
    {
        _logo_data = logo_data;
        _ui = ui;

        /* The initial index of APP comes from the ui_number. */
        set_idx(ui);
    };

    /* Draw the app's logo image. Should initialize or update idx. */
    void draw_logo()
    {
        /* Only the first three apps are shown. */
        if (_idx >= 0 && _idx <= 2)
        {
            TFT_eSprite logo_spr = TFT_eSprite(&tft);
            logo_spr.createSprite(120, 120);
            logo_spr.setSwapBytes(true);

            logo_spr.pushImage(0, 0, 120, 120, _logo_data);
            lcd_PushColors(_x, _y, 120, 120, (uint16_t *)logo_spr.getPointer());
        }
    };

    void set_idx(int16_t idx)
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
        _x = _idx * 164 + 44;
    };

    int16_t get_idx()
    {
        return _idx;
    }

    UI_MENU get_ui_idx()
    {
        return _ui;
    };

private:
    int16_t _idx = 0;   /* The index of app's location. */
    uint16_t _x = 0;    /* AXIS x */
    uint16_t _y = 90;   /* AXIS y */
    const uint16_t *_logo_data; /* Pointer to the const logo data */
    UI_MENU _ui;  /* Internal UI index, the initial is idx. */
};

APP weather_app(weather_logo, UI_WEATHER);
APP clock_app(clock_logo, UI_CLOCK);
APP picture_app(picture_logo, UI_PICTURE);
APP wifi_app(wifi_logo, UI_WIFI);
APP setup_app(setup_logo, UI_SETUP);

APP apps[MAX_APPS_NUM] = {weather_app, clock_app, picture_app, wifi_app, setup_app};
APP *cur_app;


/******************************************************************************/
/* UI draw APIs                                                               */
/******************************************************************************/

void draw_main_menu()
{
    TFT_eSprite sprite = TFT_eSprite(&tft);
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

    /* Init Apps. */
    for (int i = 0; i < MAX_APPS_NUM; i++)
    {
        apps[i].draw_logo();
    }
}

void draw_film_rect(int16_t x, int16_t y)
{
    TFT_eSprite film = TFT_eSprite(&tft);
    film.createSprite(260, 240);
    film.setSwapBytes(true);
    film.fillSprite(TFT_BROWN);

    for (int i = 0; i < 8; i++)
    {
        film.fillSmoothRoundRect(10 + i * 32, 10, 12, 24, 2, TFT_WHITE, TFT_BROWN);
        film.fillSmoothRoundRect(10 + i * 32, 206, 12, 24, 2, TFT_WHITE, TFT_BROWN);
    }
    film.fillRect(10, 40, 240, 160, TFT_WHITE);

    lcd_PushColors(x, y, 260, 240, (uint16_t *)film.getPointer());
}

void draw_picture_app_ui()
{
    draw_film_rect(138, 0);
    draw_film_rect(138 + 260, 0);
    draw_film_rect(138 - 260, 0);
}

void draw_weather_app_ui()
{
    TFT_eSprite weather_ui = TFT_eSprite(&tft);
    weather_ui.createSprite(536, 240);
    weather_ui.setSwapBytes(true);

    /* Set background. */
    weather_ui.fillSprite(TFT_WHITE);
    weather_ui.setTextColor(TFT_BLACK, TFT_WHITE);

    /* Draw title. */
    weather_ui.setFreeFont(&DSEG14_Classic_Regular_28);
    weather_ui.drawString("Weather APP", 15, 15);

    lcd_PushColors(0, 0, 536, 240, (uint16_t *)weather_ui.getPointer());
}

void draw_clock_app_ui()
{
    TFT_eSprite clock_ui = TFT_eSprite(&tft);
    clock_ui.createSprite(536, 240);
    clock_ui.setSwapBytes(true);

    /* Set background. */
    clock_ui.fillSprite(TFT_WHITE);
    clock_ui.setTextColor(TFT_BLACK, TFT_WHITE);

    /* Draw title. */
    clock_ui.setFreeFont(&DSEG14_Classic_Regular_28);
    clock_ui.drawString("CLOCK APP", 15, 15);

    lcd_PushColors(0, 0, 536, 240, (uint16_t *)clock_ui.getPointer());
}

void draw_setup_app_ui()
{
    TFT_eSprite setup_ui = TFT_eSprite(&tft);
    setup_ui.createSprite(536, 240);
    setup_ui.setSwapBytes(true);

    /* Set background. */
    setup_ui.fillSprite(TFT_WHITE);
    setup_ui.setTextColor(TFT_BLACK, TFT_WHITE);

    /* Draw title. */
    setup_ui.setFreeFont(&DSEG14_Classic_Regular_28);
    setup_ui.drawString("SETUP APP", 15, 15);

    lcd_PushColors(0, 0, 536, 240, (uint16_t *)setup_ui.getPointer());
}

void draw_wifi_app_ui()
{
    TFT_eSprite wifi_ui = TFT_eSprite(&tft);
    wifi_ui.createSprite(536, 240);
    wifi_ui.setSwapBytes(true);

    /* Set background. */
    wifi_ui.fillSprite(TFT_WHITE);
    wifi_ui.setTextColor(TFT_BLACK, TFT_WHITE);

    /* Draw title. */
    wifi_ui.setFreeFont(&DSEG14_Classic_Regular_28);
    wifi_ui.drawString("WIFI APP", 15, 15);

    lcd_PushColors(0, 0, 536, 240, (uint16_t *)wifi_ui.getPointer());
}

void draw_ui(UI_MENU ui)
{
    switch (ui)
    {
        case UI_MAIN:
            draw_main_menu();
            break;
        case UI_PICTURE:
            draw_picture_app_ui();
            break;
        case UI_WEATHER:
            draw_weather_app_ui();
            break;
        case UI_CLOCK:
            draw_clock_app_ui();
            break;
        case UI_SETUP:
            draw_setup_app_ui();
            break;
        case UI_WIFI:
            draw_wifi_app_ui();
            break;
        default:
            break;
    }
}

/******************************************************************************/
/* Button definitions                                                         */
/******************************************************************************/

#define up 21 /* Up button*/
#define down 0 /* Down button*/

OneButton button_up(up, true);
OneButton button_down(down, true);

void up_click()
{
    switch (cur_ui)
    {
        case UI_MAIN:
            for (int i = 0; i < MAX_APPS_NUM; i++)
            {
                apps[i].set_idx(apps[i].get_idx() - 1);
                apps[i].draw_logo();
                if (apps[i].get_idx() == 1)
                {
                    cur_app = &apps[i];
                }
            }
            break;
        case UI_PICTURE:
            break;
        default:
            break;
    }
}

void up_doubleclick()
{
    if (cur_ui != UI_MAIN)
    {
        cur_ui = UI_MAIN;
    }
    draw_ui(cur_ui);
}

/* Loop key. */
void down_click()
{
    switch (cur_ui)
    {
        case UI_MAIN:
            for (int i = 0; i < MAX_APPS_NUM; i++)
            {
                apps[i].set_idx(apps[i].get_idx() + 1);
                apps[i].draw_logo();
                if (apps[i].get_idx() == 1)
                {
                    cur_app = &apps[i];
                }
            }
            break;
        case UI_PICTURE:
            for (int j = 0; j <= 260; j+=4){
                if (138 - 260 + j > 0){
                    draw_film_rect(138 - 520 + j, 0);
                }
                draw_film_rect(138 - 260 + j, 0);
                draw_film_rect(138 + j, 0);
                if (138 + 260 + j < 536){
                    draw_film_rect(138 + 260 + j, 0);
                }
            }
            break;
        default:
            break;
    }
}

/* Enter Key. */
void down_doubleclick()
{
    if (cur_ui == UI_MAIN)
    {
        cur_ui = cur_app->get_ui_idx();
    }
    draw_ui(cur_ui);
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
    button_up.tick();
    button_down.tick();
}

/******************************************************************************/
/* Adruino setup() and loop()                                                 */
/******************************************************************************/

void setup()
{
    init_button();

    /* Amoled lcd initialization. */
    rm67162_init();
    lcd_setRotation(1);
    lcd_brightness(150);

    cur_ui = UI_MAIN;
    draw_ui(cur_ui);

    /* Init serial. */
    Serial.begin(115200);
}

void loop()
{
    loop_button();
}
