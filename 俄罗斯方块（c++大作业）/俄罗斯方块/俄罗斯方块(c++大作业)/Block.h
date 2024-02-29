#pragma once

class Block
{
public:
	int x;
	int y;
	static int x_speed;
	static int y_speed;
	static int width;

	Block();
	Block(int x, int y);
};