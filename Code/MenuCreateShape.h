#ifndef __MENU_CREATE_SHAPE_H__
#define __MENU_CREATE_SHAPE_H__

//#include "imgui/imgui.h"
//#include <functional>
//#include "Panel.h"
//#include <vector>
//#include "RenderTexture.h"
//#include "MathGeoLib/include/Math/float3.h"
//
//#define X_COORD 0
//#define Y_COORD 1
//#define Z_COORD 2
//
//typedef unsigned int uint;
//typedef struct par_shapes_mesh_s par_shapes_mesh;
//class MenuCreateShape;
//class ComponentTransform;
//class GameObject;
//class ResourceMesh;
//class ShapeValue
//{
//public:
//	ShapeValue(std::string name, ImGuiDataType data_type, void * value_ptr);
//
//public:
//	std::string name;
//	ImGuiDataType data_type;
//	void * value_ptr;
//};
//
////If you want to create a different layout for your shape, create a derived class from PanelCreateShape
//class PanelCreateShape : public Panel
//{
//public:
//	PanelCreateShape(std::string name, bool active = false, std::vector<SDL_Scancode> shortcut = {});
//	void MenuItem(const float button_height, const float button_space, const ImVec4& button_color, const ImVec2& button_size);
//private:
//	void Draw() override;
//	void CreateCopiesXYZ(float3 & position);
//	void CreateCopiesYZ(float3 & position);
//	void CreateCopiesZ(float3 & position);
//	void CreateCopyAtPosition(float3 position);
//	void CreateMultiple();//Function to create multiple is going to be the same for each one of them
//
//	//Function to call(may be the same that the MenuCreateShape::MenuItem() calls)
//public:
//	std::function<par_shapes_mesh*()> mesh_function;
//	std::vector<ShapeValue> shape_values;
//	std::string shape_name;
//
//private:
//	//Create multiple
//	//How many copies are going to be created in x, y and z?
//	int copies[3] = { 0u };
//	//How many separation is going to be between the copies?
//	float separation[3] = { 0.f };
//
//	//TODO: Think does each panel need to have its own copies value?
//	//TODO: Should this values be reset to 0 when you close the panel?
//};
//
////A class to handle the create shapes menu which lets you create different geometric shapes
class MenuCreateShape
{
//public:
//	MenuCreateShape();
//	~MenuCreateShape();
//
//	void MenuBarTab();
//	void CreateEmpty();
//	GameObject * CreateGameObjectWithParShape(std::string name, ComponentTransform * parent, ResourceMesh * asset_mesh);
//	//TODO: Load variables with a function from ModuleGui
//
//public:
//	RenderTexture preview_shapes_fbo;
//	//A dummy gameobject not connected to any parents
//	GameObject* preview_shape_gameobject = nullptr;
//	ResourceMesh* preview_shape_mesh = nullptr;
//
//private:
//
//	const float button_height = 7.5f;
//	const float button_space = 150.f;
//	ImVec4 button_color;
//	ImVec2 button_size;
//
//	//Parametric sphere
//	int parametric_sphere_slices = 10;
//	int parametric_sphere_stacks = 10;
//
//	//Subdivided sphere
//	int subdivided_sphere_nsubdivisions = 1;
//
//	//Hemisphere
//	int hemisphere_slices = 50;
//	int hemisphere_stacks = 20;
//
//	//Plane
//	int plane_slices = 10;
//	int plane_stacks = 10;
//
//	//Klein bottle
//	int klein_bottle_slices = 10;
//	int klein_bottle_stacks = 10;
//
//	//Cylinder
//	int cylinder_slices = 50;
//	int cylinder_stacks = 10;
//
//	//Cone variables
//	int cone_slices = 12;
//	int cone_stacks = 12;
//
//	//Torus variables
//	int torus_slices = 12;
//	int torus_stacks = 12;
//	float torus_radius = 0.5f;
//
//	//Trefoil knot
//	int trefoil_knot_slices = 50;
//	int trefoil_knot_stacks = 20;
//	float trefoil_knot_radius = 2.f;
//
//	//Disk
//	float * disk_center = nullptr;
//	float * disk_normal = nullptr;
//	int disk_slices = 12;
//	float disk_radius = 12.f;
//
//	//INFO: We create a different panel for each because we might want to customize them individually
//	std::vector<PanelCreateShape*> panels_create_shape;
};

#endif