#include "Menu.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>

#include <iostream>

#include "../world/Cursor.hpp"

using namespace gui;

Menu::Menu(glm::ivec3* cursorPointer): _cursorPos(cursorPointer) {};

void Menu::drawTools() {
	if (ImGui::Button("Create"))
		std::cout << "create" << std::endl;
	ImGui::SameLine();
	if (ImGui::Button("Delete"))
		std::cout << "delete" << std::endl;
	ImGui::SameLine();
	if (ImGui::Button("Extrude"))
		std::cout << "extrude" << std::endl;
	ImGui::SameLine();
	if (ImGui::Button("Dig"))
		std::cout << "dig" << std::endl;

	ImGui::Spacing();
}

void Menu::paint() {
	if (ImGui::Button("Paint"))
		std::cout << "paint" << std::endl;
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
	Menu::paint();
	Menu::editCursorPos();

	if (ImGui::Button("Generate map"))
		std::cout << "generate map" << std::endl;
	ImGui::Spacing();

	if (ImGui::Button("Import image"))
		std::cout << "Import image" << std::endl;
	
	ImGui::End();
}