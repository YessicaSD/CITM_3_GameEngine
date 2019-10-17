#ifndef __MENU_CREATE_SHAPE_H__
#define __MENU_CREATE_SHAPE_H__

#include "imgui/imgui.h"
#include <functional>

typedef struct par_shapes_mesh_s par_shapes_mesh;

//A class to handle the create shapes menu which lets you create different geometric shapes
class MenuCreateShape
{
public:
	MenuCreateShape();
	~MenuCreateShape();

	void Display();

private:
	void MenuItem(std::string name, std::function<par_shapes_mesh*()> mesh_function);

	//TODO: Load variables
public:
	
private:
	const float button_height = 7.5f;
	const float button_space = 150.f;
	ImVec4 button_color;
	ImVec2 button_size;

	//Cone variables
	int cone_slices = 12;
	int cone_stacks = 12;

	//Torus variables
	int torus_slices = 12;
	int torus_stacks = 12;
	float torus_radius = 0.5f;

	//Trefoil knot
	int trefoil_knot_slices = 50;
	int trefoil_knot_stacks = 20;
	float trefoil_knot_radius = 2.f;

	//Disk
	float * disk_center = nullptr;
	float * disk_normal = nullptr;
	int disk_slices = 12;
	float disk_radius = 12.f;

};

#endif