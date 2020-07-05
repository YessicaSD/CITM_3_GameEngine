#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


#pragma comment( lib, "src/SDL/libx86/SDL2.lib" )
#pragma comment( lib, "src/SDL/libx86/SDL2main.lib" )
#pragma comment (lib, "src/glew/lib/glew32.lib")
#pragma comment (lib, "src/glew/lib/glew32s.lib")
#pragma comment (lib, "src/Assimp/libx86/assimp.lib")
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */


#include "../SDL/include/SDL.h"


enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = NULL;

int main(int argc, char ** argv)
{
	LOG("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:
			App = new Application();
			LOG("-------------- Application Creation --------------");
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				LOG("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			LOG("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	delete App;
	App = nullptr;
	LOG("Exiting game '%s'...\n", TITLE);
	_CrtDumpMemoryLeaks();
	return main_return;
}