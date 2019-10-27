#ifndef __PARSER_HELPER_H__
#define __PARSER_HELPER_H__

#include "parson/parson.h"
//Methods

typedef unsigned int uint;

bool SaveFloatArray(JSON_Object * obj, const char * name, const float * arr, const uint size);
bool LoadFloatArray(JSON_Object * obj, const char * name, float * values, const uint size);

#endif