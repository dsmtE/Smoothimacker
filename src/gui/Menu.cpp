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

Menu::Menu(world::Chunk* chunkPtr, AppSettings* settings) : _chunkPtr(chunkPtr), _settings(settings),  _rbfDysplayRange(10.0f) {
};

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
}

void Menu::drawMenuBar() {
	if (ImGui::BeginMainMenuBar(), ImGuiWindowFlags_NoBringToFrontOnFocus) {
		if (ImGui::BeginMenu("Menu")) {
			if (ImGui::MenuItem("New")) {
				// Clear voxel, new scene
				_settings->_chunkPtr->reset();
			}
			ImGui::Separator();
			// if (ImGui::MenuItem("Save File")) {
			// 	// save file, open file Handler
			// 	//fileHandler.OpenWindow(FileHandler::Window::FILE_SAVE);
			// }
			// if (ImGui::MenuItem("Load File")) {
			// 	//fileHandler.OpenWindow(FileHandler::Window::FILE_OPEN);
			// }
			if (ImGui::MenuItem("Exit")) {
				_settings->exit();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scene Options")) {

			ImGui::Checkbox("enable rayCasting", &(_settings->_rayCastingEnable));
 
			ImGui::Separator();
		
			ImGui::Text("Night"); ImGui::SameLine();
			ImGui::SliderFloat("", &(_settings->_dayMode), 0.0f, 1.0f);
			ImGui::SameLine(); ImGui::Text("Day");
			ImGui::Checkbox("anim Sun", &(_settings->_animSun));

			ImGui::Separator();

			ImGui::Text("camera speed");
			ImGui::SliderFloat("camSpeed", _settings->_cameraSpeed, AppSettings::camMinSpeed, AppSettings::camMaxSpeed);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Radial Basis Functions")) {
			ImGui::SliderFloat("decrease rate", &_settings->_rbfAlpha, 0.0f, 1.0f);
			ImGui::SliderFloat("base level value", &_settings->_rbfLevel, 0.0f, 0.2f);
			ImGui::Separator();

			float values[200];
			for (size_t i = 0; i < 200; i++) {
				values[i] = _settings->_rbf(_rbfDysplayRange * float(i)/float(200));
			}
			
			if (ImGui::Combo("func", &_selectedRbfId, "quadratic\0gaussian\0") ) {
				switch (_selectedRbfId) {
				case 0:
					_settings->_rbf = std::function<float(float)>(std::bind(imath::rbf::terrainLvlQuadratic, std::placeholders::_1, _settings->_rbfAlpha, _settings->_rbfLevel));
					break;
				case 1:
					_settings->_rbf = std::function<float(float)>(std::bind(imath::rbf::terrainLvlGaussian, std::placeholders::_1, _settings->_rbfAlpha, _settings->_rbfLevel));
					break;
				default:
					break;
				}
			}
        	ImGui::SliderFloat("display max value", &_rbfDysplayRange, 1.0f, 20.0f);
			ImGui::PlotLines("radial funct", values, IM_ARRAYSIZE(values), 0, nullptr, 0.0f, 1.0f, ImVec2(0,80));

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

	ImGui::Spacing();

	if (ImGui::Button("Generate map")) {
		imath::rbf::generateTerrain(*(_settings->_chunkPtr), _settings->_controlPts->getPts(), _settings->_rbf);
	}

	if (ImGui::Button("random control pts")) {
		_settings->_controlPts->resetControlPts();
		for (size_t i = 0; i < _settings->_nbRandomControlPts; i++){
			_settings->_controlPts->addControlPts(imath::genVec3(_settings->_chunkPtr->size()));
		}
	}
	ImGui::InputInt("number of control pts:", &(_settings->_nbRandomControlPts));
	assert(_settings->_nbRandomControlPts >= 0);
	if (ImGui::Button("Add control pt")) {
		_settings->_controlPts->addControlPts(*(_settings->_cursorPos));
		_settings->_nbRandomControlPts++;
	}
	ImGui::SameLine();
	if (ImGui::Button("Del control pt")) {
		if(_settings->_controlPts->delControlPts(*(_settings->_cursorPos))) {
			_settings->_nbRandomControlPts--;
		}
	}

	ImGui::Spacing();

	if (ImGui::Button("add pt light")) {
		_settings->_pointLights->addPointLight(*(_settings->_cursorPos),  1.0f, 0.09f, 0.032f, glm::vec3(0.8f, 0.2f, 0.2f) * 0.1f, glm::vec3(0.8f, 0.2f, 0.2f));
	}
	ImGui::SameLine();
	if (ImGui::Button("Del pt light")) {
		_settings->_pointLights->delPointLight(*(_settings->_cursorPos));
	}

	ImGui::Spacing();
	
	// infos
	ImGui::Text("informations");
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::Spacing();

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
	glm::ivec3 tempPos = *(_settings->_cursorPos);
	if(_chunkPtr->getColorPtr(tempPos) != nullptr) {
		tempPos += glm::ivec3(0,1,0); // ignore the first iteration already tested
		while (_chunkPtr->getColorPtr(tempPos) != nullptr && tempPos.y < _chunkPtr->size()-1) { // loop until we find the top of our column
			tempPos.y +=1;
		}
		_chunkPtr->delAt(tempPos - glm::ivec3(0,1,0)); // delete cube on top
	} else {
		std::cout << "there is no cube here." << std::endl;
	}
}

void Menu::extrudeAction() {
	glm::ivec3 tempPos = *(_settings->_cursorPos);
	if(_chunkPtr->getColorPtr(tempPos) != nullptr) {
		tempPos += glm::ivec3(0,1,0); // ignore the first iteration already tested
		while (_chunkPtr->getColorPtr(tempPos) != nullptr && tempPos.y < _chunkPtr->size()-1) { // loop until we find the top of our column
			tempPos += glm::ivec3(0,1,0);
		}
		_chunkPtr->setColor(tempPos, _chunkPtr->getColor(tempPos - glm::ivec3(0,1,0))); // create on top
	} else {
		std::cout << "there is no cube here." << std::endl;
	}
}

void Menu::paintAction() {
	std::cout << "paint function undefined yet" << std::endl;
}