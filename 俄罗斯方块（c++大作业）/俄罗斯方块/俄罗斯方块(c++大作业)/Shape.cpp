#include"MyConsoleEnegine.h"

//■■
//■■
//具体实现O型旋转
void ShapeO::Spin(int& game_state, int PauseKey, int UpKey)
{
	if (IS_KEY_DOWN(PauseKey))
	{
		game_state = 2;
		Sleep(SLEEP_TIME);
	}
}
//具体实现O型边界限制函数
void ShapeO::TwoSideBorder(int width)
{
	if (B[0].x <= 0)
	{
		B[0].x = 0, B[1].x = B[0].x + Block::width, B[2].x = B[0].x, B[3].x = B[1].x;
	}
	if (B[1].x + Block::width >= width)
	{
		B[1].x = width - Block::width;
		B[0].x = B[1].x - Block::width;
		B[2].x = B[0].x;
		B[3].x = B[1].x;
	}
}

//■
//■
//■       ■■■■
//■
//具体实现I型旋转
void ShapeI::Spin(int& game_state, int PauseKey, int UpKey)
{
	if (IS_KEY_DOWN(UpKey))
	{
		ShapeI::state++;
		//I型有两种形态
		ShapeI::state = ShapeI::state % 2;
		if (ShapeI::state == 0)
		{
			for (int i = 1; i <= 3; i++)
			{
				B[i].x = B[0].x;
				B[i].y = B[0].y + i * Block::width;
			}
		}
		if (ShapeI::state == 1)
		{
			for (int i = 1; i <= 3; i++)
			{
				B[i].y = B[0].y;
				B[i].x = B[0].x + i * Block::width;
			}
		}
		//延时一下，防止重复执行这段代码
		Sleep(SLEEP_TIME);
	}
	if (IS_KEY_DOWN(PauseKey))
	{
		game_state = 2;//2代表的是暂停
		//延时一下，防止重复执行这段代码
		Sleep(SLEEP_TIME);
	}
}
//具体实现I型边界限制函数
void ShapeI::TwoSideBorder(int width)
{
	if (state == 0)
	{
		if (B[0].x <= 0)
		{
			for (int i = 0; i < 4; i++)
				B[i].x = 0;
		}
		if (B[0].x + Block::width >= width)
		{
			for (int i = 0; i < 4; i++)
				B[i].x = width - Block::width;
		}
	}
	if (state == 1)
	{
		if (B[0].x <= 0)
		{
			B[0].x = 0;
			for (int i = 1; i <= 3; i++)
				B[i].x = B[0].x + i * Block::width;
		}
		if (B[0].x + 4 * Block::width >= width)
		{
			B[0].x = width - 4 * Block::width;
			for (int i = 1; i <= 3; i++)
				B[i].x = B[0].x + i * Block::width;
		}
	}
}

//  ■■      ■
//■■.       ■■.
//              ■ 
//具体实现S型旋转
void ShapeS::Spin(int& game_state, int PauseKey, int UpKey)
{
	//检测按键是否按下
	if (IS_KEY_DOWN(UpKey))
	{
		ShapeS::state++;
		//S型有两种形态
		ShapeS::state = ShapeS::state % 2;
		if (ShapeS::state == 0)
		{
			B[0].x += Block::width;
			B[1].x = B[0].x + Block::width, B[1].y = B[0].y;
			B[2].x = B[0].x, B[2].y = B[0].y + Block::width;
			B[3].x = B[0].x - Block::width, B[3].y = B[0].y + Block::width;
		}
		if (ShapeS::state == 1)
		{
			B[1].x = B[0].x, B[1].y = B[0].y - Block::width;
			B[2].x = B[0].x + Block::width, B[2].y = B[0].y;
			B[3].x = B[0].x + Block::width, B[3].y = B[0].y + Block::width;
		}
		Sleep(SLEEP_TIME + 10);
	}
	if (IS_KEY_DOWN(PauseKey))
	{
		game_state = 2;//2代表暂停
		Sleep(SLEEP_TIME);
	}
}
//具体实现S型边界限制函数
void ShapeS::TwoSideBorder(int width)
{
	if (state == 0)
	{
		if (B[2].x <= 0)
		{
			B[2].x = 0;
			B[3].x = B[2].x + Block::width;
			B[0].x = B[3].x;
			B[1].x = B[0].x + Block::width;
		}
		if (B[1].x + Block::width >= width)
		{
			B[1].x = width - Block::width;
			B[0].x = B[1].x - Block::width;
			B[3].x = B[0].x;
			B[2].x = B[3].x - Block::width;
		}
	}
	else if (state == 1)
	{
		if (B[0].x <= 0)
		{
			B[0].x = 0;
			B[1].x = B[0].x;
			B[2].x = B[0].x + Block::width;
			B[3].x = B[2].x;
		}
		if (B[2].x + Block::width >= width)
		{
			B[2].x = width - Block::width;
			B[3].x = B[2].x;
			B[0].x = B[2].x - Block::width;
			B[1].x = B[0].x;
		}
	}
}

//■■         ■
//  ■■     ■■
//           ■
//具体实现Z型旋转
void ShapeZ::Spin(int& game_state, int PauseKey, int UpKey)
{
	if (IS_KEY_DOWN(UpKey))
	{
		ShapeZ::state++;
		//Z型有两种形态
		ShapeZ::state = ShapeZ::state % 2;
		if (ShapeZ::state == 0)
		{
			B[1].x = B[0].x + Block::width, B[1].y = B[0].y;
			B[2].x = B[1].x, B[2].y = B[1].y + Block::width;
			B[3].x = B[2].x + Block::width, B[3].y = B[2].y;
		}
		if (ShapeZ::state == 1)
		{
			B[1].x = B[0].x, B[1].y = B[0].y - Block::width;
			B[2].x = B[1].x + Block::width, B[2].y = B[1].y;
			B[3].x = B[2].x, B[3].y = B[2].y - Block::width;
		}
		Sleep(SLEEP_TIME + 10);
	}
	if (IS_KEY_DOWN(PauseKey))
	{
		game_state = 2;
		Sleep(SLEEP_TIME);
	}
}
//具体实现Z型边界限制函数
void ShapeZ::TwoSideBorder(int width)
{
	if (state == 0)
	{
		if (B[0].x <= 0)
		{
			B[0].x = 0;
			B[1].x = B[0].x + Block::width;
			B[2].x = B[1].x;
			B[3].x = B[2].x + Block::width;
		}
		if (B[3].x + Block::width >= width)
		{
			B[3].x = width - Block::width;
			B[2].x = B[3].x - Block::width;
			B[1].x = B[2].x;
			B[0].x = B[1].x - Block::width;
		}
	}
	else if (state == 1)
	{
		if (B[0].x <= 0)
		{
			B[0].x = 0;
			B[1].x = B[0].x;
			B[2].x = B[1].x + Block::width;
			B[3].x = B[2].x;
		}
		if (B[3].x + Block::width >= width)
		{
			B[3].x = width - Block::width;
			B[2].x = B[3].x;
			B[1].x = B[2].x - Block::width;
			B[0].x = B[1].x;
		}
	}
}

//  ■           ■■
//  ■  ■       ■     ■■■
//■■  ■■■   ■         ■
//具体实现J型旋转
void ShapeJ::Spin(int& game_state, int PauseKey, int UpKey)
{
	if (IS_KEY_DOWN(UpKey))
	{
		ShapeJ::state++;
		//J型有四种形态
		ShapeJ::state = ShapeJ::state % 4;
		if (ShapeJ::state == 0)
		{
			B[0].x += Block::width;
			B[1].x = B[0].x, B[1].y = B[0].y + Block::width;
			B[2].x = B[0].x, B[2].y = B[0].y + 2 * Block::width;
			B[3].x = B[0].x - Block::width, B[3].y = B[0].y + 2 * Block::width;
		}
		if (ShapeJ::state == 1)
		{
			B[1].x = B[0].x, B[1].y = B[0].y + Block::width;
			B[2].x = B[1].x + Block::width, B[2].y = B[1].y;
			B[3].x = B[2].x + Block::width, B[3].y = B[2].y;
		}
		if (ShapeJ::state == 2)
		{
			B[1].x = B[0].x + Block::width, B[1].y = B[0].y;
			B[2].x = B[0].x, B[2].y = B[0].y + Block::width;
			B[3].x = B[0].x, B[3].y = B[2].y + Block::width;
		}
		if (ShapeJ::state == 3)
		{
			B[1].x = B[0].x + Block::width, B[1].y = B[0].y;
			B[2].x = B[1].x + Block::width, B[2].y = B[1].y;
			B[3].x = B[2].x, B[3].y = B[2].y + Block::width;
		}
		Sleep(SLEEP_TIME);
	}
	if (IS_KEY_DOWN(PauseKey))
	{
		game_state = 2;
		Sleep(SLEEP_TIME);
	}
}
//具体实现J型边界限制函数
void ShapeJ::TwoSideBorder(int width)
{
	if (state == 0)
	{
		if (B[3].x <= 0)
		{
			B[3].x = 0;
			B[2].x = B[3].x + Block::width;
			B[1].x = B[2].x;
			B[0].x = B[1].x;
		}
		if (B[0].x + Block::width >= width)
		{
			B[0].x = width - Block::width;
			B[1].x = B[0].x;
			B[2].x = B[1].x;
			B[3].x = B[2].x - Block::width;
		}
	}
	if (state == 1)
	{
		if (B[0].x <= 0)
		{
			B[0].x = 0;
			B[1].x = B[0].x;
			B[2].x = B[1].x + Block::width;
			B[3].x = B[2].x + Block::width;
		}
		if (B[3].x + Block::width >= width)
		{
			B[3].x = width - Block::width;
			B[2].x = B[3].x - Block::width;
			B[1].x = B[2].x - Block::width;
			B[0].x = B[1].x;
		}
	}
	if (state == 2)
	{
		if (B[0].x <= 0)
		{
			B[0].x = 0;
			B[1].x = B[0].x + Block::width;
			B[2].x = B[0].x;
			B[3].x = B[2].x;
		}
		if (B[1].x + Block::width >= width)
		{
			B[1].x = width - Block::width;
			B[0].x = B[1].x - Block::width;
			B[2].x = B[0].x;
			B[3].x = B[2].x;
		}
	}
	if (state == 3)
	{
		if (B[0].x <= 0)
		{
			B[0].x = 0;
			B[1].x = B[0].x + Block::width;
			B[2].x = B[1].x + Block::width;
			B[3].x = B[2].x;
		}
		if (B[2].x + Block::width > width)
		{
			B[2].x = width - Block::width;
			B[3].x = B[2].x;
			B[1].x = B[2].x - Block::width;
			B[0].x = B[1].x - Block::width;
		}
	}
}

//■     ■■■   ■■  
//■     ■         ■       ■
//■■              ■   ■■■
//具体实现L型旋转
void ShapeL::Spin(int& game_state, int PauseKey, int UpKey)
{
	if (IS_KEY_DOWN(UpKey))
	{
		ShapeL::state++;
		//L型有四种状态
		ShapeL::state = ShapeL::state % 4;
		if (ShapeL::state == 0)
		{
			B[1].x = B[0].x, B[1].y = B[0].y + Block::width;
			B[2].x = B[0].x, B[2].y = B[0].y + 2 * Block::width;
			B[3].x = B[0].x + Block::width, B[3].y = B[0].y + 2 * Block::width;
		}
		if (ShapeL::state == 1)
		{
			B[1].x = B[0].x + Block::width, B[1].y = B[0].y;
			B[2].x = B[1].x + Block::width, B[2].y = B[1].y;
			B[3].x = B[0].x, B[3].y = B[0].y + Block::width;

		}
		if (ShapeL::state == 2)
		{
			B[0].x += Block::width;
			B[1].x = B[0].x - Block::width, B[1].y = B[0].y;
			B[2].x = B[0].x, B[2].y = B[0].y + Block::width;
			B[3].x = B[2].x, B[3].y = B[2].y + Block::width;
		}
		if (ShapeL::state == 3)
		{
			B[0].x += 2 * Block::width;
			B[1].x = B[0].x, B[1].y = B[0].y + Block::width;
			B[2].x = B[1].x - Block::width, B[2].y = B[1].y;
			B[3].x = B[2].x - Block::width, B[3].y = B[2].y;
		}
		Sleep(SLEEP_TIME);
	}
	if (IS_KEY_DOWN(PauseKey))
	{
		game_state = 2;
		Sleep(SLEEP_TIME);
	}
}
//具体实现L型边界限制函数
void ShapeL::TwoSideBorder(int width)
{
	if (state == 0)
	{
		if (B[0].x <= 0)
		{
			B[0].x = 0;
			B[1].x = B[0].x;
			B[2].x = B[0].x;
			B[3].x = B[0].x + Block::width;
		}
		if (B[3].x + Block::width >= width)
		{
			B[3].x = width - Block::width;
			B[2].x = B[3].x - Block::width;
			B[1].x = B[2].x;
			B[0].x = B[1].x;
		}
	}
	if (state == 1)
	{
		if (B[0].x <= 0)
		{
			B[0].x = 0;
			B[1].x = B[0].x + Block::width;
			B[2].x = B[1].x + Block::width;
			B[3].x = B[0].x;
		}
		if (B[2].x + Block::width >= width)
		{
			B[2].x = width - Block::width;
			B[1].x = B[2].x - Block::width;
			B[0].x = B[1].x - Block::width;
			B[3].x = B[0].x;
		}
	}
	if (state == 2)
	{
		if (B[0].x <= 0)
		{
			B[0].x = 0;
			B[1].x = B[0].x + Block::width;
			B[2].x = B[1].x;
			B[3].x = B[1].x;
		}
		if (B[1].x + Block::width >= width)
		{
			B[1].x = width - Block::width;
			B[0].x = B[1].x - Block::width;
			B[2].x = B[1].x;
			B[3].x = B[1].x;
		}
	}
	if (state == 3)
	{
		if (B[3].x <= 0)
		{
			B[3].x = 0;
			B[2].x = B[3].x + Block::width;
			B[1].x = B[2].x + Block::width;
			B[0].x = B[1].x;
		}
		if (B[0].x + Block::width > width)
		{
			B[0].x = width - Block::width;
			B[1].x = B[0].x;
			B[2].x = B[1].x - Block::width;
			B[3].x = B[2].x - Block::width;
		}
	}
}

//■■■     ■             ■  
//  ■     ■■     ■      ■■ 
//           ■   ■■■    ■
//具体实现T型旋转
void ShapeT::Spin(int& game_state, int PauseKey, int UpKey)
{
	if (IS_KEY_DOWN(UpKey))
	{
		ShapeT::state++;
		//T型有四种状态
		ShapeT::state = ShapeT::state % 4;
		if (ShapeT::state == 0)
		{
			B[1].x = B[0].x + Block::width, B[1].y = B[0].y;
			B[2].x = B[1].x + Block::width, B[2].y = B[1].y;
			B[3].x = B[1].x, B[3].y = B[1].y + Block::width;
		}
		if (ShapeT::state == 1)
		{
			B[0].x = B[1].x;
			B[1].x = B[0].x - Block::width, B[1].y = B[0].y + Block::width;
			B[2].x = B[0].x, B[2].y = B[0].y + Block::width;
			B[3].x = B[2].x, B[3].y = B[2].y + Block::width;
		}
		if (ShapeT::state == 2)
		{
			B[1].x = B[0].x - Block::width, B[1].y = B[0].y + Block::width;
			B[2].x = B[1].x + Block::width, B[2].y = B[1].y;
			B[3].x = B[2].x + Block::width, B[3].y = B[2].y;
		}
		if (ShapeT::state == 3)
		{
			B[1].x = B[0].x, B[1].y = B[0].y + Block::width;
			B[2].x = B[1].x + Block::width, B[2].y = B[1].y;
			B[3].x = B[1].x, B[3].y = B[1].y + Block::width;
		}
		Sleep(SLEEP_TIME + 10);
	}
	if (IS_KEY_DOWN(PauseKey))
	{
		game_state = 2;
		Sleep(SLEEP_TIME);
	}
}
//具体实现T型边界限制函数
void ShapeT::TwoSideBorder(int width)
{
	if (state == 0)
	{
		if (B[0].x <= 0)
		{
			B[0].x = 0;
			B[1].x = B[0].x + Block::width;
			B[2].x = B[1].x + Block::width;
			B[3].x = B[1].x;
		}
		if (B[2].x + Block::width >= width)
		{
			B[2].x = width - Block::width;
			B[1].x = B[2].x - Block::width;
			B[0].x = B[1].x - Block::width;
			B[3].x = B[1].x;
		}
	}
	if (state == 1)
	{
		if (B[1].x <= 0)
		{
			B[1].x = 0;
			B[0].x = B[1].x + Block::width;
			B[2].x = B[0].x;
			B[3].x = B[0].x;
		}
		if (B[0].x + Block::width >= width)
		{
			B[0].x = width - Block::width;
			B[1].x = B[0].x - Block::width;
			B[2].x = B[0].x;
			B[3].x = B[0].x;
		}
	}
	if (state == 2)
	{
		if (B[1].x <= 0)
		{
			B[1].x = 0;
			B[0].x = B[1].x + Block::width;
			B[2].x = B[0].x;
			B[3].x = B[0].x + Block::width;
		}
		if (B[3].x + Block::width >= width)
		{
			B[3].x = width - Block::width;
			B[0].x = B[3].x - Block::width;
			B[2].x = B[0].x;
			B[1].x = B[2].x - Block::width;
		}
	}
	if (state == 3)
	{
		if (B[0].x <= 0)
		{
			B[0].x = 0;
			B[1].x = B[0].x;
			B[2].x = B[1].x + Block::width;
			B[3].x = B[0].x;
		}
		if (B[2].x + Block::width > width)
		{
			B[2].x = width - Block::width;
			B[0].x = B[2].x - Block::width;
			B[1].x = B[0].x;
			B[3].x = B[0].x;
		}
	}
}