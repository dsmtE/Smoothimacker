#include "Menu.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>

#include <iostream>
#include <algorithm> // for clamp
#include <functional>   // for bind
#include "../imath/util.hpp"
#include "../imath/RadialBasisFunction.hpp"

#include "../world/Cursor.hpp"


using namespace gui;

Menu::Menu(world::Chunk* chunkPtr, AppSettings* settings) : _chunkPtr(chunkPtr), _settings(settings) {};

void Menu::handleEvent(SDL_Event sdlEvent) {
	_tool.handleSDLEvents(sdlEvent);
}

void Menu::drawTools() {

	// tool bar
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
	// infos
	ImGui::Text("informations");
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

}

void Menu::editCursorPos() {
	ImGui::Text("Cursor position");
	if ( ImGui::InputInt("X", &(_settings->_cursorPos->x))  ) {
		_settings->_cursorPos->x = std::clamp( _settings->_cursorPos->x, 0, int(_settings->_chunkPtr->size()) );
	}
	if ( ImGui::InputInt("Y", &(_settings->_cursorPos->y))  ) {
		_settings->_cursorPos->y = std::clamp( _settings->_cursorPos->y, 0, int(_settings->_chunkPtr->size()) );
	}
	if ( ImGui::InputInt("Z", &(_settings->_cursorPos->z))  ) {
		_settings->_cursorPos->z = std::clamp( _settings->_cursorPos->z, 0, int(_settings->_chunkPtr->size()) );
	}
	ImGui::Spacing();
}

void Menu::drawMenuBar() {
	if (ImGui::BeginMainMenuBar(), ImGuiWindowFlags_NoBringToFrontOnFocus) {
		if (ImGui::BeginMenu("Menu")) {
			if (ImGui::MenuItem("New")) {
				// CLEAR VOXELS
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Save File")) {
				// save file, open file Handler
				//fileHandler.OpenWindow(FileHandler::Window::FILE_SAVE);
			}
			if (ImGui::MenuItem("Load File")) {
				//fileHandler.OpenWindow(FileHandler::Window::FILE_OPEN);
			}
			if (ImGui::MenuItem("Exit")) {
				_settings->exit();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Options")) {

			ImGui::Checkbox("enable rayCasting", &(_settings->_rayCastingEnable));

			ImGui::Spacing();

			ImGui::Text("camera speed");
			ImGui::SliderFloat("camSpeed", _settings->_cameraSpeed, AppSettings::camMinSpeed, AppSettings::camMaxSpeed);

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Menu::drawMenu() {
	
	drawMenuBar();

	ImGui::Begin("Editor settings", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove); // | ImGuiWindowFlags_AlwaysAutoResize
	ImGui::SetWindowPos(ImVec2(_settings->windowSize().x - ImGui::GetWindowWidth(), 18.0f));
	ImGui::SetWindowSize(ImVec2(300, _settings->windowSize().y));
	ImGui::Text("Tools:");
	
	Menu::drawTools();
	Menu::editCursorPos();

	if (ImGui::Button("Generate map")) {
		// std::cout << "generate map" << std::endl;
		imath::rbf::generateTerrain(*(_settings->_chunkPtr), _settings->_controlPts->getPts(), std::function<float(float)>(std::bind(imath::rbf::terrainLvlQuadratic, std::placeholders::_1, 0.03, 0.03)));
	}
	ImGui::Spacing();

	if (ImGui::Button("random control pts")) {
		_settings->_controlPts->resetControlPts();
		for (size_t i = 0; i < _settings->_nbRandomControlPts; i++){
			_settings->_controlPts->addControlPts(imath::genVec3(_settings->_chunkPtr->size()));
		}
	}
	ImGui::Text("number of control pts:");
	ImGui::InputInt("", &(_settings->_nbRandomControlPts));
	assert(_settings->_nbRandomControlPts >= 0);
	ImGui::Spacing();

	if (ImGui::Button("cursor add control pt")) {
		_settings->_controlPts->addControlPts(*(_settings->_cursorPos));
		_settings->_nbRandomControlPts++;
	}

	ImGui::Spacing();

	if (ImGui::Button("Import image"))
		std::cout << "Import image" << std::endl;
	
	ImGui::End();
}

// action callBack functions
void Menu::createAction() {
	_chunkPtr->setColor(*(_settings->_cursorPos), glm::vec3(1, 1, 0.9));
}

void Menu::deleteAction() {
	_chunkPtr->delAt(*(_settings->_cursorPos));
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