
#ifndef _PANEL_H_
#define _PANEL_H_

#include <string>
#include <vector>
#include "SDL\include\SDL_scancode.h"
class Panel
{
	protected:
		bool active = false;
		std::string name="";
		std::vector<SDL_Scancode> ShortCut;

	public:
		Panel(std::string name, bool active=false):name(name), active(active){}
		Panel(std::string name, std::vector<SDL_Scancode> ShortCUt, bool active=false):name(name), active(active){}


		void SwitchActive()
		{
			active = !active;
		}
		bool IsActive() const
		{
			return active;
		}
		void SetName(std::string name)
		{
			this->name = name;
		}

		virtual void Draw() {}
		
};
#endif // !_PANEL_H_
