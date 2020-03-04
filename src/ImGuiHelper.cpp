#include "ImGuiHelper.h"
#include "Window.h"

namespace MGLlib {
	ImGuiHelper::ImGuiHelper(Window *w){
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(w->getGLFWwindow(), true);
		ImGui_ImplOpenGL3_Init(glsl_version);
		ImGui::StyleColorsDark();
	}
	ImGuiHelper::~ImGuiHelper(){}

	void ImGuiHelper::Exit(void){
		// close ImGui
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiHelper::newFrame(void){
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiHelper::render(void){
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}