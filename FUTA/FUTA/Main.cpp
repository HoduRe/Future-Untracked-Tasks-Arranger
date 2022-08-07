#include <iostream>
#include "FUTA.h"

#include "SDL/include/SDL.h"
#include "glew-2.1.0/include/GL/glew.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#pragma comment( lib, "glew-2.1.0/lib/Release/Win32/glew32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib")

SDL_Window* window = nullptr;
SDL_Surface* screenSurface = nullptr;
SDL_GLContext context = nullptr;

bool InitScreen() {

	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) { ret = false; }

	else {

		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		int width = DM.w;
		int height = DM.h;

		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
		window = SDL_CreateWindow("FUTA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width * 0.8, height * 0.8, flags);
		if (window == NULL) { ret = false; }
		else { screenSurface = SDL_GetWindowSurface(window); }

	}

	context = SDL_GL_CreateContext(window);
	if (context == NULL) { ret = false; }

	GLenum glewError = glewInit();
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) { return false; }

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init("#version 130");

	SDL_DisplayMode displayMode;
	int numDisplayModes = SDL_GetNumDisplayModes(0);
	SDL_GetDisplayMode(SDL_GetWindowDisplayIndex(window), SDL_GetWindowDisplayMode(window, &displayMode), &displayMode);
	// THIS IS MISSING LOTS OF THINGS; THIS SET-UP ONLY IS USED TO HAVE IMGUI AVAILABLE, NOT SO MUCH DEEP OPENGL USAGE

	return true;
}

void RenderPostUpdate() {

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(window);

}

void ImGuiStartFrame() {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();
	ImGuiDockNodeFlags flag = ImGuiDockNodeFlags_NoDockingInCentralNode;
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), flag);

}

int main(int argc, char** argv) {

	bool mainLoop = true;
	int screenWidth, screenHeight;
	FUTA futaApp;
	futaApp.LoadData();

	if (!InitScreen()) { return 1; }
	while (mainLoop == true) {

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) { case SDL_QUIT: mainLoop = false; break; }
		}

		ImGuiStartFrame();

		SDL_GetWindowSize(window, &screenWidth, &screenHeight);
		futaApp.Update(screenWidth, screenHeight);

		ImGui::EndFrame();
		RenderPostUpdate();

	}

	futaApp.SaveData();

	SDL_GL_DeleteContext(context);
	if (window != NULL) { SDL_DestroyWindow(window); }
	SDL_Quit();

	return 0;
}

