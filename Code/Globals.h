#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)
#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI
#define ASSETS_FOLDER "Assets/"

typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned char uchar;
typedef unsigned long long UID;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

#ifdef NULL
#undef NULL
#endif
#define NULL  0

// Deletes a buffer
#define RELEASE( x ) \
    {                        \
    if( x != NULL )        \
	    {                      \
      delete x;            \
	  x = NULL;              \
	    }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
    {                              \
    if( x != NULL )              \
	    {                            \
      delete[] x;                \
	  x = NULL;                    \
	    }                            \
                              \
    }

//Configuration
#define ASSETS_FOLDER "Assets/"
#define SETTINGS_FOLDER "Settings/"
#define RESOURCES_FOLDER "Resources/"
#define RESOURCES_AUDIO_FOLDER "Resources/Audio/"
#define RESOURCES_TEXTURES_FOLDER "Resources/Textures/"
#define RESOURCES_MESH_FOLDER "Resources/Meshes/"
#define RESOURCES_ANIMATION_FOLDER "Resources/Animations/"
#define RESOURCES_BONE_FOLDER "Resources/Bones/"
#define RESOURCES_SCENE_FOLDER "Resources/Scenes/"
#define RESOURCES_MODEL_FOLDER "Resources/Models/"
#define RESOURCES_MATERIAL_FOLDER "Resources/Materials/"
#define RESOURCES_STATE_MACHINE_FOLDER "Resources/StateMachines/"

#define META_EXTENSION "hinata_meta"
#define MESH_EXTENSION "hinata_mesh"
#define ANIMATION_EXTENSION "hinata_animation"
#define BONE_EXTENSION "hinata_bone"
#define MODEL_EXTENSION "hinata_model"
#define TEXTURE_EXTENSION "dds"
#define MATERIAL_EXTENSION "hinata_material"
#define SCENE_EXTENSION "hinata_scene"

#define SCREEN_WIDTH 1920 * 0.75f //TODO: Remove when releasing the game
#define SCREEN_HEIGHT 1080 * 0.75f //TODO: Remove when releasing the game
#define SCREEN_SIZE 1
#define TITLE "Hinata Engine"
#define VERSION "0.1"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif
