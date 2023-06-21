#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "lodepng.h"
#include <glm/glm.hpp>
#include <iostream>


using namespace std;
using namespace glm;

namespace GLContext {	
	extern const char* window_name;
	extern bool alpha;  
	extern bool fullscreen;
	extern int SCR_WIDTH;
	extern int SCR_HEIGHT;

	extern int viewportResW;
	extern int viewportResH;

	extern vec4 background;
	extern void(*onInput)(int key);
	extern void(*onDraw)();
	extern void(*initialize)();
	extern void(*onDrawUI)();
	

	int init(int width, int height);
	
	void TakeScreenshot(string filepath);
	
	void drawPoint(vec2 position, float size = 5, vec4 color = vec4(1, 1, 1, 1));
	void drawLine(vec2 position1, vec2 position2, float width = 1, vec4 color = vec4(1, 1, 1, 1));
	
}

