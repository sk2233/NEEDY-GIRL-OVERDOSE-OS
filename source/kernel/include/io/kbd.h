#ifndef OS_KBD_H
#define OS_KBD_H

#include "comm/type.h"

#define KDB_Q 16
#define KDB_W 17
#define KDB_E 18
#define KDB_R 19
#define KDB_T 20
#define KDB_Y 21
#define KDB_U 22
#define KDB_I 23
#define KDB_O 24
#define KDB_P 25

#define KDB_A 30
#define KDB_S 31
#define KDB_D 32
#define KDB_F 33
#define KDB_G 34
#define KDB_H 35
#define KDB_J 36
#define KDB_K 37
#define KDB_L 38

#define KDB_Z 44
#define KDB_X 45
#define KDB_C 46
#define KDB_V 47
#define KDB_B 48
#define KDB_N 49
#define KDB_M 50

void kbd_init();
void kbd_update(uint8_t data);

bool_t kbd_press(uint8_t key);
bool_t kbd_just_press(uint8_t key);
bool_t kbd_just_release(uint8_t key);
void kbd_refresh();

#endif
