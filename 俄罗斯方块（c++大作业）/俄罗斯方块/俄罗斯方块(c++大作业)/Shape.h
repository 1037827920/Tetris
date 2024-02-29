#pragma once

class Shape
{
public:
	//每个形状都只有四种方块
	Block B[4];
	//形状的颜色
	int color;

public:
	//旋转函数
	virtual void Spin(int& game_state, int PauseKey, int UpKey) = 0;
	//边界限制函数,只有两边的判断
	virtual void TwoSideBorder(int width) = 0;
};

//■■
//■■
class ShapeO :public Shape
{
public:
	//构造函数
	ShapeO(int x, int y)
	{
		B[0].x = x, B[0].y = y;
		B[1].x = B[0].x + Block::width;
		B[1].y = B[0].y;
		B[2].x = B[0].x;
		B[2].y = B[0].y + Block::width;
		B[3].x = B[0].x + Block::width;
		B[3].y = B[0].y + Block::width;
		color = YELLOW;
	}
	//具体实现O型旋转
	virtual void Spin(int& game_state, int PauseKey, int UpKey) override;
	//具体实现O型边界限制函数
	virtual void TwoSideBorder(int width) override;
};

//■
//■
//■       ■■■■
//■
class ShapeI :public Shape
{
private:
	//I型有两种形态
	int state = 0;

public:
	//构造函数
	ShapeI(int x, int y)
	{
		B[0].x = x, B[0].y = y;
		B[1].x = B[0].x, B[2].x = B[0].x, B[3].x = B[0].x;
		B[1].y = B[0].y + Block::width;
		B[2].y = B[0].y + 2 * Block::width;
		B[3].y = B[0].y + 3 * Block::width;
		color = RGB(0, 0, 204);
	}
	//具体实现I型旋转
	virtual void Spin(int& game_state, int PauseKey, int UpKey)override;
	//具体实现I型边界限制函数
	virtual void TwoSideBorder(int width) override;
};

//  ■■      ■
//■■.       ■■.
//              ■ 
class ShapeS :public Shape
{
private:
	//S型有两种形态
	int state = 0;

public:
	//构造函数
	ShapeS(int x, int y)
	{
		B[0].x = x, B[0].y = y;
		B[1].x = B[0].x + Block::width, B[1].y = B[0].y;
		B[2].x = B[0].x, B[2].y = B[0].y + Block::width;
		B[3].x = B[0].x - Block::width, B[3].y = B[0].y + Block::width;
		color = RGB(255, 0, 0);
	}
	//具体实现S型旋转
	virtual void Spin(int& game_state, int PauseKey, int UpKey)override;
	//具体实现S型边界限制函数
	virtual void TwoSideBorder(int width) override;
};

//■■         ■
//  ■■     ■■
//           ■
class ShapeZ :public Shape
{
private:
	//S型有两种形态
	int state = 0;

public:
	//构造函数
	ShapeZ(int x, int y)
	{
		B[0].x = x, B[0].y = y;
		B[1].x = B[0].x + Block::width, B[1].y = B[0].y;
		B[2].x = B[1].x, B[2].y = B[1].y + Block::width;
		B[3].x = B[2].x + Block::width, B[3].y = B[2].y;
		color = RGB(127, 0, 255);
	}
	//具体实现Z型旋转
	virtual void Spin(int& game_state, int PauseKey, int UpKey)override;
	//具体实现Z型边界限制函数
	virtual void TwoSideBorder(int width) override;
};

//  ■           ■■
//  ■  ■       ■     ■■■
//■■  ■■■   ■         ■
class ShapeJ :public Shape
{
private:
	//J型有四种形态
	int state = 0;

public:
	//构造函数
	ShapeJ(int x, int y)
	{
		B[0].x = x, B[0].y = y;
		B[1].x = B[0].x, B[1].y = B[0].y + Block::width;
		B[2].x = B[0].x, B[2].y = B[0].y + 2 * Block::width;
		B[3].x = B[0].x - Block::width, B[3].y = B[0].y + 2 * Block::width;
		color = RGB(0, 153, 0);
	}
	//具体实现J型旋转
	virtual void Spin(int& game_state, int PauseKey, int UpKey)override;
	//具体实现J型边界限制函数
	virtual void TwoSideBorder(int width) override;
};

//■     ■■■   ■■  
//■     ■         ■       ■
//■■              ■   ■■■
class ShapeL :public Shape
{
private:
	//L型有四种形态5
	int state = 0;

public:
	//构造函数
	ShapeL(int x, int y)
	{
		B[0].x = x, B[0].y = y;
		B[1].x = B[0].x, B[1].y = B[0].y + Block::width;
		B[2].x = B[0].x, B[2].y = B[0].y + 2 * Block::width;
		B[3].x = B[0].x + Block::width, B[3].y = B[0].y + 2 * Block::width;
		color = RGB(255, 153, 51);
	}
	//具体实现L型旋转
	virtual void Spin(int& game_state, int PauseKey, int UpKey)override;
	//具体实现L型边界限制函数
	virtual void TwoSideBorder(int width) override;

};

//■■■     ■             ■  
//  ■     ■■     ■      ■■ 
//           ■   ■■■    ■
class ShapeT :public Shape
{
private:
	//T型有四种形态5
	int state = 0;

public:
	//构造函数
	ShapeT(int x, int y)
	{
		B[0].x = x, B[0].y = y;
		B[1].x = B[0].x + Block::width, B[1].y = B[0].y;
		B[2].x = B[1].x + Block::width, B[2].y = B[1].y;
		B[3].x = B[1].x, B[3].y = B[1].y + Block::width;
		color = RGB(0, 255, 255);
	}
	//具体实现T型旋转
	virtual void Spin(int& game_state, int PauseKey, int UpKey)override;
	//具体实现T型边界限制函数
	virtual void TwoSideBorder(int width) override;

};