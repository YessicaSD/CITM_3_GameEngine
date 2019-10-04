#ifndef _PANEL_RENDER_MODE_H_
#define _PANEL_RENDER_MODE_H_
#include "Panel.h"
class PanelRenderMode :public Panel
{
public:
	PanelRenderMode(std::string name, bool active);
	void Draw() override;
	std::string labelState;
private:

};


#endif // !_PANEL_RENDER_MODE_H_

