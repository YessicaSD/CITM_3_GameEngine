#include "PanelAnimator.h"
#include "../imgui/imgui.h"
#include "../NodeEditor/Include/imgui_node_editor.h"
namespace ed = ax::NodeEditor;
#include "ModuleGui.h"
#include "Application.h"

// Struct to hold basic information about connection between
// pins. Note that connection (aka. link) has its own ID.
// This is useful later with dealing with selections, deletion
// or other operations.
struct LinkInfo
{
	ed::LinkId Id;
	ed::PinId  InputId;
	ed::PinId  OutputId;
};
static std::vector<LinkInfo>   g_Links;
static int                  g_NextLinkId = 100;
static int pin_id = 1;
static int node_id = 1;
PanelAnimator::PanelAnimator(std::string name, bool active, std::vector<SDL_Scancode> shortcuts): Panel(name, active, shortcuts)
{

	Node enter_node(node_id++, "Start", ImColor(255, 255, 255), NodeType::ENTER);
	enter_node.Outputs.push_back(Pin(pin_id++, "Out put"));
	nodes.push_back(enter_node);

	Node enter_node2(node_id++, "Idle", ImColor(255, 255, 255), NodeType::STATES);
	enter_node2.Outputs.push_back(Pin(pin_id++, "Out put"));
	enter_node2.start_pos = { 210, 60 };
	nodes.push_back(enter_node2);

}

void PanelAnimator::Draw()
{

	////ImGui::Begin("Animator");
	//static bool g_FirstFrame = true;
	//ed::SetCurrentEditor(App->gui->g_Context);

	//	ed::Begin("My Editor");
	//	{
	//		
	//		for (auto node = nodes.begin(); node != nodes.end(); ++node)
	//		{
	//			if ((*node).first_loop)
	//			{
	//				ed::SetNodePosition((*node).ID, (*node).start_pos);
	//				(*node).first_loop = false;
	//			}

	//			ed::BeginNode((*node).ID);
	//			ImGui::Text((*node).name.c_str());
	//			for (auto pin = (*node).Outputs.begin(); pin != (*node).Outputs.end(); ++pin)
	//			{
	//				ed::BeginPin((*pin).ID, ed::PinKind::Output);
	//				ImGui::Text("-> OutPut");
	//				ed::EndPin();
	//			}
	//			ed::EndNode();
	//		}
	//	}

	//	// Submit Links
	//	for (std::vector<LinkInfo>::iterator linkInfo = g_Links.begin(); linkInfo != g_Links.end(); ++linkInfo)
	//	{
	//		ed::Link((*linkInfo).Id, (*linkInfo).InputId, (*linkInfo).OutputId);
	//	}

	//	// Handle creation action, returns true if editor want to create new object (node or link)
	//	if (ed::BeginCreate())
	//	{
	//		ed::PinId inputPinId, outputPinId;
	//		if (ed::QueryNewLink(&inputPinId, &outputPinId))
	//		{
	//			// QueryNewLink returns true if editor want to create new link between pins.
	//			//
	//			// Link can be created only for two valid pins, it is up to you to
	//			// validate if connection make sense. Editor is happy to make any.
	//			//
	//			// Link always goes from input to output. User may choose to drag
	//			// link from output pin or input pin. This determine which pin ids
	//			// are valid and which are not:
	//			//   * input valid, output invalid - user started to drag new ling from input pin
	//			//   * input invalid, output valid - user started to drag new ling from output pin
	//			//   * input valid, output valid   - user dragged link over other pin, can be validated

	//			if (inputPinId && outputPinId) // both are valid, let's accept link
	//			{
	//				// ed::AcceptNewItem() return true when user release mouse button.
	//				if (ed::AcceptNewItem())
	//				{
	//					// Since we accepted new link, lets add one to our list of links.
	//					g_Links.push_back({ ed::LinkId(g_NextLinkId++), inputPinId, outputPinId });

	//					// Draw new link.
	//					ed::Link(g_Links.back().Id, g_Links.back().InputId, g_Links.back().OutputId);
	//				}

	//				// You may choose to reject connection between these nodes
	//				// by calling ed::RejectNewItem(). This will allow editor to give
	//				// visual feedback by changing link thickness and color.
	//			}
	//		}
	//	}
	//	ed::EndCreate(); // Wraps up object creation action handling.

	//	ed::End();

	//	ed::SetCurrentEditor(nullptr);

	//	//ImGui::End();
	//	g_FirstFrame = false;

}
