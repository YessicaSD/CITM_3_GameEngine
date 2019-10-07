#ifndef PANEL_PROPERTIES_H_
#define PANEL_PROPERTIES_H_

#include "Panel.h"
#include <string>
class PanelProperties : public Panel
{
public:
	PanelProperties(std::string name, bool state);
	void Draw() override;
};



#endif // !PANEL_PROPERTIES_H_

