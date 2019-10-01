#include "PanelConsole.h"

PanelConsole::PanelConsole(std::string name, bool active): Panel(name, active)
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
	ImGui::TextUnformatted(output.begin());
	ImGui::End();
}
