#ifndef __MORGANA_BASE_DEVICES_DISPLAY_OPENGL_OGLHEADERS_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_OPENGL_OGLHEADERS_H__

#include "platform.h"

#ifdef OS_WINDOWS_WIN32
#include <Windows.h>
#include "GLee.h"
#include "gl/gl.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GLee.lib")

#define glGenPrograms	glGenProgramsARB

#endif

#endif