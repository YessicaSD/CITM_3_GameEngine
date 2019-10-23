#include "PanelConsole.h"

PanelConsole::PanelConsole(std::string name, bool active, std::vector<SDL_Scancode> shortcuts): Panel(name, active, shortcuts)
{

}

PanelConsole::~PanelConsole()
{
	output.clear();
}

void PanelConsole::Log(const char * sentence)
{
	output.appendf(sentence);
}

void PanelConsole::Draw()
{
	ImGui::Begin(name.c_str());
	ImGui::Text("Hello");
	ImGui::End();
	//ImGui::TextUnformatted(output.begin());
}
