// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// opengl header files
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#pragma comment(lib, "glew32.lib")

// reference additional headers your program requires here
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;