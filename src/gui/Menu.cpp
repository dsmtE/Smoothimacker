#include "Menu.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>

#include <iostream>

#include "../world/Cursor.hpp"


using namespace gui;

Menu::Menu(glm::ivec3* cursorPointer, world::Chunk* chunkPtr): _cursorPos(cursorPointer), _chunkPtr(chunkPtr) {};

void Menu::handleEvent(SDL_Event sdlEvent) {
	_tool.handleSDLEvents(sdlEvent);
}

void Menu::drawTools() {
	
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	if (ImGui::Button(" Create")) {
		std::cout << "assign CREATE strategy" << std::endl;
		// or using lambda function with [this]() {createAction(); };
		_tool.setAction(std::bind(&Menu::createAction, this));
	}
	ImGui::SameLine();
	if (ImGui::Button(" Delete")) {
		std::cout << "assign DELETE strategy" << std::endl;
		_tool.setAction(std::bind(&Menu::deleteAction, this));
	}
	ImGui::SameLine();
	if (ImGui::Button(" Extrude")) {
		std::cout << "assign EXTRUDE strategy" << std::endl;
		_tool.setAction(std::bind(&Menu::extrudeAction, this));
	}
	ImGui::SameLine();
	if (ImGui::Button(" Dig")) {
		std::cout << "assign DIG strategy" << std::endl;
		_tool.setAction(std::bind(&Menu::digAction, this));
	}

	ImGui::Spacing();

	if (ImGui::Button("Paint")) {
		std::cout << "PAINT" << std::endl;
		_tool.setAction(std::bind(&Menu::paintAction, this));
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

void Menu::createAction() {
	_chunkPtr->setType(*_cursorPos, 0);
}

void Menu::deleteAction() {
	_chunkPtr->delAt(*_cursorPos);
}

void Menu::digAction() {
	std::cout << "dig function undefined yet" << std::endl;
}

void Menu::extrudeAction() {
	std::cout << "dig function undefined yet" << std::endl;
}

void Menu::paintAction() {
	std::cout << "paint function undefined yet" << std::endl;
}