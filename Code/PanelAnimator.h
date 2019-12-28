#ifndef PANEL_ANIMATOR_H_
#define PANEL_ANIMATOR_H_
#include "Panel.h"
#include "imgui/imgui.h"
#include "MathGeoLib/include/Math/float3.h"
#include "NodeEditor/Include/imgui_node_editor.h"


enum class PinKind
{
	Output,
	Input
};

enum class NodeType
{
	ENTER,
	STATES,
};

struct Node;

struct Pin
{
	ax::NodeEditor::PinId   ID;
	::Node*     Node;
	std::string Name;
	//	PinType     Type;
	PinKind     Kind;

	Pin(int id, const char* name/*, PinType type*/) :
		ID(id), Node(nullptr), Name(name)/*, Type(type)*/, Kind(PinKind::Input)
	{
	}
};

struct Node
{
public:
	ax::NodeEditor::NodeId ID;
	std::string name;
	std::vector<Pin> Inputs;
	std::vector<Pin> Outputs;
	ImColor Color;
	NodeType Type;
	ImVec2 Size;

	std::string State;
	std::string SavedState;
	ImVec2 start_pos = {0,0};
	bool first_loop = true;
	Node(int id, const char* name, ImColor color = ImColor(255, 255, 255), NodeType Type = NodeType::STATES) :
		ID(id), name(name), Color(color), Type(Type), Size(0, 0)
	{
	}
};

class PanelAnimator : public Panel
{
	std::vector<Node> nodes;
public:
	PanelAnimator(std::string name, bool active, std::vector<SDL_Scancode> shortcuts = {});
	void Draw() override;
	void CreateNode();
};
#endif // !PANEL_ANIMATOR_H_
