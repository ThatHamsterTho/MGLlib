#ifndef IMGUIHELPERHEADERGUARD
#define IMGUIHELPERHEADERGUARD

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"	// TODO: check what is neede from example files

namespace MGLlib{
	class Window;

	class ImGuiHelper 
	{
		private:
		// GL 3.0 + GLSL 130
   		const char* glsl_version = "#version 330";

		public:
			ImGuiHelper(Window *w);
			~ImGuiHelper();

			void newFrame();
			void render();

			//! @brief Call before exiting window object
			void Exit(void);
	};
} // namespace

#endif