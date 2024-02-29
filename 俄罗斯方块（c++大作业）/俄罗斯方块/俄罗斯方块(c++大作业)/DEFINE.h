#pragma once

#define SLEEP_TIME 130

//判断某个键是否按下
#define IS_KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0) 