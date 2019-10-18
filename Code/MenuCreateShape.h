#ifndef __MENU_CREATE_SHAPE_H__
#define __MENU_CREATE_SHAPE_H__

#include "imgui/imgui.h"
#include <functional>
#include "Panel.h"

typedef struct par_shapes_mesh_s par_shapes_mesh;
class MenuCreateShape;

class PanelCreateShape : public Panel
{
public:
	PanelCreateShape(std::string name, MenuCreateShape * menu_create_shape, bool active = false, std::vector<SDL_Scancode> shortcut = {});
protected:
	MenuCreateShape * menu_create_shape = nullptr;
	void CreateMultiple();//Function to create multiple is going to be the same for each one of them
};

class PanelCreateCube : public PanelCreateShape
{
public:
	PanelCreateCube(std::string name, MenuCreateShape * menu_create_shape, bool active = false, std::vector<SDL_Scancode> shortcut = {});
	void Draw() override;
};

class PanelCreateParametricSphere : public PanelCreateShape
{
public:
	PanelCreateParametricSphere(std::string name, MenuCreateShape * menu_create_shape, bool active = false, std::vector<SDL_Scancode> shortcut = {});
	void Draw() override;
};

class PanelCreateSubdividedSphere : public PanelCreateShape
{
public:
	PanelCreateSubdividedSphere(std::string name, MenuCreateShape * menu_create_shape, bool active = false, std::vector<SDL_Scancode> shortcut = {});
	void Draw() override;
};

class PanelCreateHemisphere : public PanelCreateShape
{
public:
	PanelCreateHemisphere(std::string name, MenuCreateShape * menu_create_shape, bool active = false, std::vector<SDL_Scancode> shortcut = {});
	void Draw() override;
};

class PanelCreatePlane : public PanelCreateShape
{
public:
	PanelCreatePlane(std::string name, MenuCreateShape * menu_create_shape, bool active = false, std::vector<SDL_Scancode> shortcut = {});
	void Draw() override;
};

class PanelCreateKleinBottle : public PanelCreateShape
{
public:
	PanelCreateKleinBottle(std::string name, MenuCreateShape * menu_create_shape, bool active = false, std::vector<SDL_Scancode> shortcut = {});
	void Draw() override;
};

class PanelCreateCylinder : public PanelCreateShape
{
public:
	PanelCreateCylinder(std::string name, MenuCreateShape * menu_create_shape, bool active = false, std::vector<SDL_Scancode> shortcut = {});
	void Draw() override;
};

class PanelCreateCone : public PanelCreateShape
{
public:
	PanelCreateCone(std::string name, MenuCreateShape * menu_create_shape, bool active = false, std::vector<SDL_Scancode> shortcut = {});
	void Draw() override;
};

class PanelCreateTorus : public PanelCreateShape
{
public:
	PanelCreateTorus(std::string name, MenuCreateShape * menu_create_shape, bool active = false, std::vector<SDL_Scancode> shortcut = {});
	void Draw() override;
};

class PanelCreateTrefoilKnot : public PanelCreateShape
{
public:
	PanelCreateTrefoilKnot(std::string name, MenuCreateShape * menu_create_shape, bool active = false, std::vector<SDL_Scancode> shortcut = {});
	void Draw() override;
};

class PanelCreateDodecahedron : public PanelCreateShape
{
public:
	PanelCreateDodecahedron(std::string name, MenuCreateShape * menu_create_shape, bool active = false, std::vector<SDL_Scancode> shortcut = {});
	void Draw() override;
};

class PanelCreateIcosahedron : public PanelCreateShape
{
public:
	PanelCreateIcosahedron(std::string name, MenuCreateShape * menu_create_shape, bool active = false, std::vector<SDL_Scancode> shortcut = {});
	void Draw() override;
};

class PanelCreateDisk : public PanelCreateShape
{
public:
	PanelCreateDisk(std::string name, MenuCreateShape * menu_create_shape, bool active = false, std::vector<SDL_Scancode> shortcut = {});
	void Draw() override;
};

//A class to handle the create shapes menu which lets you create different geometric shapes
class MenuCreateShape
{
public:
	MenuCreateShape();
	~MenuCreateShape();

	void MenuBarTab();

private:
	void PanelCreateMultiple();
	void MenuItem(std::string name, std::function<par_shapes_mesh*()> mesh_function, Panel & panel);

	//TODO: Load variables with a function from ModuleGui

public:

private:
	const float button_height = 7.5f;
	const float button_space = 150.f;
	ImVec4 button_color;
	ImVec2 button_size;

	//Parametric sphere
	int parametric_sphere_slices = 10;
	int parametric_sphere_stacks = 10;

	//Subdivided sphere
	int subdivided_sphere_nsubdivisions = 1;

	//Hemisphere
	int hemisphere_slices = 50;
	int hemisphere_stacks = 20;

	//Plane
	int plane_slices = 10;
	int plane_stacks = 10;

	//Klein bottle
	int klein_bottle_slices = 10;
	int klein_bottle_stacks = 10;

	//Cylinder
	int cylinder_slices = 50;
	int cylinder_stacks = 10;

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

	//INFO: We create a different panel for each because we might want to customize them individually
	PanelCreateCube panel_cube;
	PanelCreateParametricSphere panel_parametric_sphere;
	PanelCreateSubdividedSphere panel_subdivided_sphere; 
	PanelCreateHemisphere panel_hemisphere;
	PanelCreatePlane panel_plane;
	PanelCreateKleinBottle panel_klein_bottle;
	PanelCreateCylinder panel_cylinder;
	PanelCreateCone panel_cone;
	PanelCreateTorus panel_torus;
	PanelCreateTrefoilKnot panel_trefoil_knot;
	PanelCreateDodecahedron panel_dodecahedron;
	PanelCreateIcosahedron panel_icosahedron;
	PanelCreateDisk panel_disk;

	friend class PanelCreateCone;
};

#endif