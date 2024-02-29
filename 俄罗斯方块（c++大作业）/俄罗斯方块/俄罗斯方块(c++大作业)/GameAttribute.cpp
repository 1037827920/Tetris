#include"MyConsoleEnegine.h"

GameAttribute::GameAttribute()
{
	this->game_width = 400;
	this->game_height = 600;
	this->extra_width = 150;
	this->W_ratio = (double)(game_width + extra_width) / 550.0;
	this->H_ratio = (double)game_height / 600.0;
	//这里的数字都是对应键的ASCII码
	this->PauseKey = 80;
	this->Upkey = 87;
	this->DownKey = 83;
	this->LeftKey = 65;
	this->RightKey = 68;
	this->QuickDownKey = 74;
	this->QuitKey = 27;
	//背景色
	this->background_color = RGB(192, 192, 192);
}

