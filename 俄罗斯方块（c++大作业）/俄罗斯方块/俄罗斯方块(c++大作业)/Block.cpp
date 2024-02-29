#include"Block.h"

int Block::x_speed = 25;
int Block::y_speed = 4;
int Block::width = 25;

Block::Block() {}
Block::Block(int x, int y) :
	x(x), y(y) {}