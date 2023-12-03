
// #define CLOCK_X(angle, r) (clock_spr_R + ((r) * sin(angle * M_PI / 180)))
// #define CLOCK_Y(angle, r) (clock_spr_R - ((r) * cos(angle * M_PI / 180)))

// #define RGB_TRAN(x_24) (((x_24 & 0xFF0000) >> 19) << 11) + (((x_24 & 0x00FF00) >> 10) << 5) + ((x_24 & 0x0000FF) >> 3)

// void draw_clock_spr()
// {
//     int16_t secAngel = 50;
//     int16_t minAngel = 280;
//     float hourAngel = 150;
//     int colok_x = 20;
//     int colok_y = 60;
//     int clock_spr_R = 80;
//     int clock_spr_r = 75;
//     int clock_spr_w = 2 * clock_spr_R;

//     clock_spr.createSprite(clock_spr_w, clock_spr_w);
//     clock_spr.setSwapBytes(true);
//     clock_spr.fillSprite(TFT_SKYBLUE);
//     clock_spr.fillSmoothRoundRect(0, 0, clock_spr_w, clock_spr_w, 30, TFT_BLACK, TFT_SKYBLUE);
//     clock_spr.fillCircle(clock_spr_R, clock_spr_R, clock_spr_r, TFT_BLACK);
//     clock_spr.fillCircle(clock_spr_R, clock_spr_R, clock_spr_r - 5, TFT_WHITE);

//     /* Draw dash board. */
//     for (int i = 0; i <= 360; i += 30)
//     {
//         float x = (clock_spr_r - 7) * cos(M_PI - i * M_PI / 180);
//         float y = (clock_spr_r - 7) * sin(i * M_PI / 180);
//         clock_spr.drawWideLine(clock_spr_R + x, clock_spr_R - y, clock_spr_R, clock_spr_R, 2, TFT_BLACK);
//     }

//     clock_spr.fillCircle(clock_spr_R, clock_spr_R, clock_spr_r - 12, TFT_WHITE);

//     /* Draw center ring. */
//     clock_spr.fillCircle(clock_spr_R, clock_spr_R, 5, TFT_BLACK);
//     clock_spr.fillCircle(clock_spr_R, clock_spr_R, 3, TFT_WHITE);

//     /* Draw minute. */
//     clock_spr.drawWideLine(CLOCK_X(minAngel, 5), CLOCK_Y(minAngel, 5), CLOCK_X(minAngel, 20), CLOCK_Y(minAngel, 20), 2, TFT_BLACK);
//     clock_spr.drawWideLine(CLOCK_X(minAngel, 20), CLOCK_Y(minAngel, 20), CLOCK_X(minAngel, clock_spr_r - 15), CLOCK_Y(minAngel, clock_spr_r - 15), 6, TFT_BLACK);

//     /* Draw hour. */
//     clock_spr.drawWideLine(CLOCK_X(hourAngel, 5), CLOCK_Y(hourAngel, 5), CLOCK_X(hourAngel, 20), CLOCK_Y(hourAngel, 20), 2, TFT_BLACK);
//     clock_spr.drawWideLine(CLOCK_X(hourAngel, 20), CLOCK_Y(hourAngel, 20), CLOCK_X(hourAngel, clock_spr_r - 30), CLOCK_Y(hourAngel, clock_spr_r - 30), 6, TFT_BLACK);

//     /* Draw second. */
//     clock_spr.drawWideLine(CLOCK_X(secAngel, 5), CLOCK_Y(secAngel, 5), CLOCK_X(secAngel, clock_spr_r - 15), CLOCK_Y(secAngel, clock_spr_r - 15), 2, TFT_RED);

//     lcd_PushColors(colok_x, colok_y, clock_spr_w, clock_spr_w, (uint16_t *)clock_spr.getPointer());
// }


// class clockApp : public APP {
// public:
//     // 子类可以访问父类的公有成员
//     clockApp() : APP(){};

//     // 新的成员函数
//     void draw() override {
//     };
// };
