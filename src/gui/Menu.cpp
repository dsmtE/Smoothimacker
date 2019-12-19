#include "Menu.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>

#include <iostream>

#include "../world/Cursor.hpp"


using namespace gui;

Menu::Menu(glm::ivec3* cursorPointer): _cursorPos(cursorPointer) {};

void Menu::handleEvent(SDL_Event sdlEvent) {
	_tool.handleSDLEvents(sdlEvent);
}

void Menu::drawTools() {
	
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	if (ImGui::Button("Create")) {
		std::cout << "CREATE" << std::endl;
		_tool.setStrategy(Create);
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete")) {
		std::cout << "DELETE" << std::endl;
		_tool.setStrategy(Delete);
	}
	ImGui::SameLine();
	if (ImGui::Button("Extrude")) {
		std::cout << "EXTRUDE" << std::endl;
		_tool.setStrategy(Extrude);
	}
	ImGui::SameLine();
	if (ImGui::Button("Dig")) {
		std::cout << "DIG" << std::endl;
		_tool.setStrategy(Dig);
	}

	ImGui::Spacing();

	if (ImGui::Button("Paint")) {
		std::cout << "PAINT" << std::endl;
		_tool.setStrategy(Paint);
	}
	const char* items[] = { "BLUE", "RED", "GREEN", "BROWN", "YELLOW", "PURPLE", "GREY" };
	static int item_current = 0;
	ImGui::SameLine();
	ImGui::Combo("Color", &item_current, items, IM_ARRAYSIZE(items));

	ImGui::Spacing();
}

void Menu::editCursorPos() {
	ImGui::Text("Cursor position");
	ImGui::InputInt("X", &(_cursorPos->x));
	ImGui::InputInt("Y", &(_cursorPos->y));
	ImGui::InputInt("Z", &(_cursorPos->z));

	ImGui::Spacing();
}

void Menu::drawMenu() {
	/*ImGui::ShowDemoWindow();*/
	ImGui::SetNextWindowPos(ImVec2(950, 5), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(550, 980), ImGuiCond_FirstUseEver);
	ImGui::Begin("Tools");
	
	Menu::drawTools();
	Menu::editCursorPos();

	if (ImGui::Button("Generate map"))
		std::cout << "generate map" << std::endl;
	ImGui::Spacing();

	if (ImGui::Button("Import image"))
		std::cout << "Import image" << std::endl;
	
	ImGui::End();
}