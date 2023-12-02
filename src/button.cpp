#include "../include/button.h"

OneButton button_up(up, true);
OneButton button_down(down, true);

void up_click()
{
    printf("Button 1 click.\n");
}

void up_doubleclick()
{
    printf("Button 1 doubleclick.\n");
}

void up_longPressStart()
{
    printf("Button 1 longPress start\n");
}

void up_longPress()
{
    printf("Button 1 longPress...\n");
}

void up_longPressStop()
{
    printf("Button 1 longPress stop\n");
}

void down_click()
{
    printf("Button 2 click.\n");
}

void down_doubleclick()
{
    printf("Button 2 doubleclick.\n");
}

void down_longPressStart()
{
    printf("Button 2 longPress start\n");
}

void down_longPress()
{
    printf("Button 2 longPress...\n");
}

void down_longPressStop()
{
    printf("Button 2 longPress stop\n");
}

void init_button()
{
    pinMode(up, INPUT_PULLUP);
    pinMode(down, INPUT_PULLUP);

    button_up.attachClick(up_click);
    button_up.attachDoubleClick(up_doubleclick);
    button_up.attachLongPressStart(up_longPressStart);
    button_up.attachLongPressStop(up_longPress);
    button_up.attachDuringLongPress(up_longPressStop);

    button_down.attachClick(down_click);
    button_down.attachDoubleClick(down_doubleclick);
    button_down.attachLongPressStart(down_longPressStart);
    button_down.attachLongPressStop(down_longPress);
    button_down.attachDuringLongPress(down_longPressStop);
}

void loop_button()
{
    button_up.tick();
    button_down.tick();
}