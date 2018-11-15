#pragma once
class Drawer
{
public:
	unsigned int VAO;
	unsigned int VBO;
	Drawer();
	~Drawer();
	virtual void Draw() = 0;
};

