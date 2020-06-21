#ifndef __PANEL_IMPORT_H__
#define __PANEL_IMPORT_H__

#include "Panel.h"

struct ModelImportOptionsAux
{
	bool calcTangentSpace = false;
	bool joinIdenticalVertices = false;
	bool makeLeftHanded = false;
};

//Panel that lets you change file import options
class PanelImport : public Panel
{
public:
	PanelImport(std::string name, bool active = false, std::vector<SDL_Scancode> shortcuts = {});
	void Draw()override;
	void DrawModel();
	void DrawTexture();

	void Tooltip(const char * desc) const;

private:
	ModelImportOptionsAux model_aux;
	//TODO: Each time we select a new model, this options get reset / get imported from the new model
	//TODO: Only when we have this window opened
};

#endif