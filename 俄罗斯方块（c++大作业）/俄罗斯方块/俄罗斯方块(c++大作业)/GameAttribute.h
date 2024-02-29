#pragma once

class GameAttribute
{
public:
	int game_width;
	int game_height;
	int extra_width;
	//这个比例是为了在玩家改变地图大小后仍然能保持较好的显示
	double W_ratio;
	double H_ratio;
	//按键
	int PauseKey;
	int Upkey;
	int DownKey;
	int LeftKey;
	int RightKey;
	int QuickDownKey;
	int QuitKey;
	//背景色
	int background_color;

	GameAttribute();
};