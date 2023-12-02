
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <cmath>
#include <WiFi.h>
#include <string.h>
#include <string>
#include "../include/rm67162.h"
#include "../include/font.h"
#include "../include/sevenSeg.h"
#include "../include/button.h"

#define RGB_TRAN(x_24) (((x_24 & 0xFF0000) >> 19) << 11) + (((x_24 & 0x00FF00) >> 10) << 5) + ((x_24 & 0x0000FF) >> 3)

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
TFT_eSprite clock_spr = TFT_eSprite(&tft);

#define CLOCK_X(angle, r) (clock_spr_R + ((r) * sin(angle * M_PI / 180)))
#define CLOCK_Y(angle, r) (clock_spr_R - ((r) * cos(angle * M_PI / 180)))

class APP
{
    public:
        APP(){};
        virtual void draw(){};
        void change_idx(){
            _idx += 1;
            if (_idx == 3){
                _idx = 0;
            };

            _x = _idx * 172 + 10;
        };

    private:
        uint8_t _idx = 0;
        uint16_t _x = 0;
        uint16_t _y = 60;
};

void draw_clock_spr()
{
    int16_t secAngel = 50;
    int16_t minAngel = 280;
    float hourAngel = 150;
    int colok_x = 20;
    int colok_y = 60;
    int clock_spr_R = 80;
    int clock_spr_r = 75;
    int clock_spr_w = 2 * clock_spr_R;

    clock_spr.createSprite(clock_spr_w, clock_spr_w);
    clock_spr.setSwapBytes(true);
    clock_spr.fillSprite(TFT_SKYBLUE);
    clock_spr.fillSmoothRoundRect(0, 0, clock_spr_w, clock_spr_w, 30, TFT_BLACK, TFT_SKYBLUE);
    clock_spr.fillCircle(clock_spr_R, clock_spr_R, clock_spr_r, TFT_BLACK);
    clock_spr.fillCircle(clock_spr_R, clock_spr_R, clock_spr_r - 5, TFT_WHITE);

    /* Draw dash board. */
    for (int i = 0; i <= 360; i += 30)
    {
        float x = (clock_spr_r - 7) * cos(M_PI - i * M_PI / 180);
        float y = (clock_spr_r - 7) * sin(i * M_PI / 180);
        clock_spr.drawWideLine(clock_spr_R + x, clock_spr_R - y, clock_spr_R, clock_spr_R, 2, TFT_BLACK);
    }

    clock_spr.fillCircle(clock_spr_R, clock_spr_R, clock_spr_r - 12, TFT_WHITE);

    /* Draw center ring. */
    clock_spr.fillCircle(clock_spr_R, clock_spr_R, 5, TFT_BLACK);
    clock_spr.fillCircle(clock_spr_R, clock_spr_R, 3, TFT_WHITE);

    /* Draw minute. */
    clock_spr.drawWideLine(CLOCK_X(minAngel, 5), CLOCK_Y(minAngel, 5), CLOCK_X(minAngel, 20), CLOCK_Y(minAngel, 20), 2, TFT_BLACK);
    clock_spr.drawWideLine(CLOCK_X(minAngel, 20), CLOCK_Y(minAngel, 20), CLOCK_X(minAngel, clock_spr_r - 15), CLOCK_Y(minAngel, clock_spr_r - 15), 6, TFT_BLACK);

    /* Draw hour. */
    clock_spr.drawWideLine(CLOCK_X(hourAngel, 5), CLOCK_Y(hourAngel, 5), CLOCK_X(hourAngel, 20), CLOCK_Y(hourAngel, 20), 2, TFT_BLACK);
    clock_spr.drawWideLine(CLOCK_X(hourAngel, 20), CLOCK_Y(hourAngel, 20), CLOCK_X(hourAngel, clock_spr_r - 30), CLOCK_Y(hourAngel, clock_spr_r - 30), 6, TFT_BLACK);

    /* Draw second. */
    clock_spr.drawWideLine(CLOCK_X(secAngel, 5), CLOCK_Y(secAngel, 5), CLOCK_X(secAngel, clock_spr_r - 15), CLOCK_Y(secAngel, clock_spr_r - 15), 2, TFT_RED);

    lcd_PushColors(colok_x, colok_y, clock_spr_w, clock_spr_w, (uint16_t *)clock_spr.getPointer());
}

class clockApp : public APP {
public:
    // 子类可以访问父类的公有成员
    clockApp() : APP(){};

    // 新的成员函数
    void draw() override {
        draw_clock_spr();
    };
};

void setup()
{
    init_button();

    /* Amoled lcd initialization. */
    rm67162_init();
    lcd_setRotation(1);
    lcd_brightness(150);

    sprite.createSprite(536, 240);
    sprite.setSwapBytes(true);

    /* Set background. */
    sprite.fillSprite(TFT_BLACK);
    sprite.fillSmoothRoundRect(0, 0, 536, 240, 30, TFT_SKYBLUE, TFT_BLACK);
    sprite.setTextColor(TFT_BLACK, TFT_SKYBLUE);

    /* Draw title. */
    sprite.setFreeFont(&DSEG14_Classic_Regular_28);
    sprite.drawString("AMOLED -- OS", 150, 20);
    lcd_PushColors(0, 0, 536, 240, (uint16_t *)sprite.getPointer());

    clockApp clock_app;
    clock_app.draw();

    /* Init serial. */
    Serial.begin(115200);
}

void loop()
{
    loop_button();
}
