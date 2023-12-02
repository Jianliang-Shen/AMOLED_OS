#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "OneButton.h"

#define up 21
#define down 0

void up_click();
void up_doubleclick();
void up_longPressStart();
void up_longPress();
void up_longPressStop();
void down_click();
void down_doubleclick();
void down_longPressStart();
void down_longPress();
void down_longPressStop();

void init_button();
void loop_button();

#endif
