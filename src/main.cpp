
#include <Arduino.h>
#include <string.h>
#include <cmath>
#include <TFT_eSPI.h>
#include <OneButton.h>
#include <WiFi.h>
#include "../include/build_configs.h"
#include "../include/rm67162.h"
#include "../include/my_font.h"
#include "../include/sevenSeg.h"
#include "../include/logos.h"
#include "../include/queue.h"
#include "../include/app.h"
#include "../include/ui.h"


#ifdef USE_BACKGROUND
#include "../include/background.h"
#endif

#ifdef PHOTO_SUPPORT
#include "../include/photos/photos.h"
#include "../include/NotoSansBold15.h"
#endif

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])

/******************************************************************************/
/* TFT definition                                                             */
/******************************************************************************/
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
TFT_eSprite start_spr = TFT_eSprite(&tft);

#ifdef PHOTO_SUPPORT
TFT_eSprite film = TFT_eSprite(&tft);
#endif

UI_MENU cur_ui;

/******************************************************************************/
/* APP class definition                                                       */
/******************************************************************************/

APP weather_app(weather_logo, UI_WEATHER);
APP clock_app(clock_logo, UI_CLOCK);
APP picture_app(picture_logo, UI_PICTURE);
APP game_app(game_logo, UI_GAME);
APP setup_app(setup_logo, UI_SETUP);

APP apps[MAX_APPS_NUM] = {
    picture_app,
    setup_app,
    clock_app,
    weather_app,
    game_app
};
APP *cur_app;

/******************************************************************************/
/* UI draw APIs                                                               */
/******************************************************************************/

void draw_fake_app_ui(const char *str)
{
    TFT_eSprite app_ui = TFT_eSprite(&tft);
    app_ui.createSprite(536, 240);
    app_ui.setSwapBytes(true);

    /* Set background. */
    app_ui.fillSprite(TFT_WHITE);
    app_ui.setTextColor(TFT_BLACK, TFT_WHITE);

    /* Draw title. */
    app_ui.setFreeFont(&DSEG14_Classic_Regular_28);
    app_ui.drawString(str, 15, 15);

    lcd_PushColors(0, 0, 536, 240, (uint16_t *)app_ui.getPointer());
}

void draw_main_menu()
{
    lcd_PushColors(0, 0, 536, 240, (uint16_t *)sprite.getPointer());

    /* Init Apps. */
    for (int i = 0; i < MAX_APPS_NUM; i++)
    {
        apps[i].draw_logo();
    }
}

void draw_picture_app_ui()
{
#ifdef PHOTO_SUPPORT
    int x, flag = 0, N = ARRAY_SIZE(photos);
    for (int i = 0; i < N && flag != 1; i++){
        for (int j = 0; j < 260 && flag != 1; j+=1){
            for (int k = N - 1; k >= 0; k --){
                /* Calculate AXIS and draw the photos. */
                x = 138 + 260 * (1 - k + i) + j - 122;
                if (x >= -260 && x < 536) {
                    film.pushImage(10, 40, 240, 160, photos[k]);
                    lcd_PushColors(x, 0, 260, 240, (uint16_t *)film.getPointer());
                }

                /* Exit Loop. */
                if (k == N - 1 && x == 0){
                    flag = 1;
                    break;
                }
            }
        }
    }
#else
    draw_fake_app_ui("PHOTO APP");
#endif
}

void draw_weather_app_ui()
{
    draw_fake_app_ui("Weather APP");
}

void draw_clock_app_ui()
{
    draw_fake_app_ui("CLOCK APP");
}

void draw_setup_app_ui()
{
    draw_fake_app_ui("SETUP APP");
}

void draw_game_app_ui()
{
    draw_fake_app_ui("GAME APP");
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
        case UI_GAME:
            draw_game_app_ui();
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
                apps[i].moving_left();
                if (apps[i].get_idx() == 2)
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
        draw_ui(cur_ui);
    }
}

/* Loop key. */
void down_click()
{
    switch (cur_ui)
    {
        case UI_MAIN:
            for (int i = 0; i < MAX_APPS_NUM; i++)
            {
                apps[i].moving_right();
                if (apps[i].get_idx() == 2)
                {
                    cur_app = &apps[i];
                }
            }
            break;
        case UI_PICTURE:
                draw_ui(cur_ui);
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
        draw_ui(cur_ui);
    }
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
/* Startup Page                                                           */
/******************************************************************************/

void draw_start_up_page()
{
    start_spr.createSprite(536, 240);
    start_spr.setSwapBytes(true);
    start_spr.fillSprite(TFT_BLACK);

    start_spr.setFreeFont(&DialogInput_plain_16);
    start_spr.setTextColor(TFT_BLACK, 0xFF50);
    start_spr.drawString("23-12-03 18:56", 5, 5);
    lcd_PushColors(0, 0, 536, 240, (uint16_t *)start_spr.getPointer());
    Queue queue(8, 20);

    delay(1000);
}

/******************************************************************************/
/* Adruino setup() and loop()                                                 */
/******************************************************************************/

void init_global_sprite(){
    /* Init sprite. */
    sprite.createSprite(536, 240);
    sprite.setSwapBytes(true);

#ifdef USE_BACKGROUND
    /* Set background. */
    sprite.pushImage(0,0,536,240,background);

    /* Draw title. */
    // sprite.setFreeFont(&DSEG14_Classic_Regular_28);
    sprite.setFreeFont(&DialogInput_plain_16);
    sprite.setTextColor(TFT_BLACK, 0xFF50);
    sprite.drawString("23-12-03 18:56", 5, 5);
#else
    sprite.fillSmoothRoundRect(0, 0, 536, 240, 30, TFT_WHITE, TFT_BLACK);
#endif

    /* Draw select rect. */
    sprite.fillSmoothRoundRect(202, 54, 132, 132, 10, TFT_BLACK, TFT_WHITE);
    sprite.fillSmoothRoundRect(204, 56, 128, 128, 8, TFT_WHITE, TFT_BLACK);

#ifdef PHOTO_SUPPORT
    /* Init film. */
    film.createSprite(260, 240);
    film.setSwapBytes(true);
    film.fillSprite(0x8A22);

    for (int i = 0; i < 8; i++)
    {
        film.fillSmoothRoundRect(12 + i * 32, 12, 12, 20, 2, TFT_WHITE, 0x8A22);
        film.fillSmoothRoundRect(12 + i * 32, 206, 12, 20, 2, TFT_WHITE, 0x8A22);
    }
    film.loadFont(NotoSansBold15);
    film.setTextColor(0xFC06, 0x8A22);
    film.drawString("kz 01 9542 0676-", 10, 227);

    uint16_t x = 140;
    for (uint16_t i = 0; i < 30; i ++){
        film.fillRect(x, 229, barcode_black[i], 10, 0xFC06);
        x += (barcode_white[i] + barcode_black[i]);
    }
#endif
}

void setup()
{
    /* Amoled lcd initialization. */
    rm67162_init();
    lcd_setRotation(1);
    lcd_brightness(150);

    /* Init serial. */
    Serial.begin(115200);

    init_button();

    init_global_sprite();

    // draw_start_up_page();

    /* Init UI and APPs. */
    cur_ui = UI_MAIN;
    cur_app = &apps[2];
    draw_ui(cur_ui);
}

void loop()
{
    loop_button();
}
