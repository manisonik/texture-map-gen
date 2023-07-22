// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Specify single-inheritance only  
#pragma pointers_to_members( full_generality, multiple_inheritance ) 

#define _USE_MATH_DEFINES

// Windows Header Files
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <gdiplus.h>

// Cairo
#include <cairo-gl.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include <stdarg.h>
#include <sstream>
#include <chrono>
#include <ctime>
#include <variant>
#include <algorithm>

// GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/matrix_transform_2d.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/intersect.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/normal.hpp"
#include "glm/gtx/string_cast.hpp"

// Opengl
#include <gl/glew.h>
#include <gl/GL.h>

// libPNG
#include <png.h>

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

// DevIL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

// TODO: reference additional headers your program requires here
