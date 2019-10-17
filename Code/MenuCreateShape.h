#ifndef __MENU_CREATE_SHAPE_H__
#define __MENU_CREATE_SHAPE_H__

//A class to handle the create shapes menu which lets you create different geometric shapes

struct ImVec4;
struct ImVec2;

class MenuCreateShape
{
public:
	void Display();

private:
	void MenuItemCreateShape(const float space, const float button_height, const ImVec4 &button_color, const ImVec2 &button_size);

	//TODO: Load variables
};

#endif