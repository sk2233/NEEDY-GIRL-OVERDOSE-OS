#ifndef OS_MOUSE_H
#define OS_MOUSE_H

#include "comm/type.h"

#define MOUSE_LEFT (1<<0)
#define MOUSE_MID  (1<<1)
#define MOUSE_RIGHT (1<<2)

extern volatile uint16_t mouse_x;
extern volatile uint16_t mouse_y;
extern volatile uint16_t last_mouse_x;
extern volatile uint16_t last_mouse_y;

void mouse_init();
void mouse_update(uint8_t data);

bool_t mouse_press(uint8_t key);
bool_t mouse_just_press(uint8_t key);
bool_t mouse_just_release(uint8_t key);
void mouse_refresh();
bool_t mouse_dirty();
void mouse_save();

#endif
