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
#include "IconsFontAwesome5.h"
//#include "IconsFontAwesome5Pro.h"

using namespace gui;

Menu::Menu(world::Chunk* chunkPtr, AppSettings* settings) : _chunkPtr(chunkPtr), _settings(settings),  _rbfDysplayRange(10.0f) {
	//icons
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();

	// merge in icons from Font Awesome
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config; 
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	io.Fonts->AddFontFromFileTTF("assets/fonts/fa-solid-900.ttf", 16.0f, &icons_config, icons_ranges);

};

void Menu::handleEvent(SDL_Event sdlEvent) {
	_tool.handleSDLEvents(sdlEvent);
}

void Menu::colorPicker() {
	ImGui::Text("Color picker:");
	ImGuiColorEditFlags misc_flags = (ImGuiColorEditFlags_HDR) | (ImGuiColorEditFlags_NoOptions);
	ImGuiColorEditFlags flags = misc_flags;
	flags |= ImGuiColorEditFlags_DisplayRGB;
	ImGui::ColorPicker3("MyColor", (float*)&(_settings->_colorPick), flags);
	ImGui::Spacing();
	ImGui::Separator();
}

void Menu::drawTools() {
	ImGui::Text("Cubes tools");
	ImGui::Spacing(); ImGui::Spacing();
	
	ImGui::Columns(2, NULL, false);
	static int selected = -1;

	if (ImGui::Selectable(ICON_FA_CUBE " Create", selected == 0)) {
		selected = 0;
		//assign CREATE strategy
		// or using lambda function with [this]() {createAction(); };
		_tool.setAction(std::bind(&Menu::createAction, this));
	}
	
	if (ImGui::Selectable(ICON_FA_ERASER " Delete", selected == 1)) {
		selected = 1;
		//assign DELETE strategy
		_tool.setAction(std::bind(&Menu::deleteAction, this));
	}
	
	ImGui::NextColumn();

	if (ImGui::Selectable(ICON_FA_CUBES " Extrude", selected == 2)) {
		selected = 2;
		//assign EXTRUDE strategy
		_tool.setAction(std::bind(&Menu::extrudeAction, this));
	}

	if (ImGui::Selectable(ICON_FA_KIWI_BIRD " Dig", selected == 3)) {
		selected = 3;
		//assign DIG strategy
		_tool.setAction(std::bind(&Menu::digAction, this));
	}

	ImGui::NextColumn();
	
	if (ImGui::Selectable(ICON_FA_PAINT_BRUSH " Paint", selected == 4)) {
		selected = 4;
		//assign PAINT strategy
		_tool.setAction(std::bind(&Menu::paintAction, this));
	}
	ImGui::Columns(1);
	ImGui::Separator();
	
}

void Menu::lightsSettings() {
	ImGui::Text("Lights");
	ImGui::Spacing();
	ImGui::Columns(2, NULL, false);
	if (ImGui::Button(ICON_FA_LIGHTBULB)) {
		_settings->_pointLights->addPointLight(*(_settings->_cursorPos),  1.0f, 0.09f, 0.032f, _settings->_colorPick * 0.1f, _settings->_colorPick);
	}
	ImGui::SameLine();
	ImGui::Text("Add   ");
	ImGui::NextColumn();
	if (ImGui::Button(ICON_FA_TRASH)) {
		_settings->_pointLights->delPointLight(*(_settings->_cursorPos));
	}
	ImGui::SameLine();
	ImGui::Text("Delete ");
	ImGui::Columns(1);
	ImGui::Separator();
	ImGui::Spacing();
}

void Menu::randomGeneration() {
	ImGui::Text("Generation with control points");
	ImGui::Spacing();
	ImGui::Columns(2, NULL, false);
	if (ImGui::Button(ICON_FA_PLUS)) {
		_settings->_controlPts->addControlPts(*(_settings->_cursorPos));
		_settings->_nbRandomControlPts++;
	}
	ImGui::SameLine();
	ImGui::Text("Add   ");
	
	if (ImGui::Button(ICON_FA_TRASH)) {
		if(_settings->_controlPts->delControlPts(*(_settings->_cursorPos))) {
			_settings->_nbRandomControlPts--;
		}
	}
	ImGui::SameLine();
	ImGui::Text("Delete");

	ImGui::NextColumn();

	ImGui::SetNextItemWidth(100);
	ImGui::InputInt("", &(_settings->_nbRandomControlPts));
	assert(_settings->_nbRandomControlPts >= 0);
	

	if (ImGui::Button("Random points")) {
		_settings->_controlPts->resetControlPts();
		for (size_t i = 0; i < _settings->_nbRandomControlPts; i++) {
			_settings->_controlPts->addControlPts(imath::genVec3(_settings->_chunkPtr->size()));
		}
	}
	
	ImGui::Columns(1);
	ImGui::Spacing();
	if (ImGui::Button("Generate map")) {
		imath::rbf::generateTerrain(*(_settings->_chunkPtr), _settings->_controlPts->getPts(), std::function<float(float)>(std::bind(imath::rbf::terrainLvlQuadratic, std::placeholders::_1, 0.03, 0.03)));
	}
	ImGui::Separator();
	ImGui::Spacing();
}

void Menu::editCursorPos() {
	ImGui::Text("Cursor position");
	ImGui::Spacing();
	ImGui::SetNextItemWidth(70);
	if ( ImGui::InputInt("X", &(_settings->_cursorPos->x))  ) {
		_settings->_cursorPos->x = std::clamp( _settings->_cursorPos->x, 0, int(_settings->_chunkPtr->size()) );
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(70);
	if ( ImGui::InputInt("Y", &(_settings->_cursorPos->y))  ) {
		_settings->_cursorPos->y = std::clamp( _settings->_cursorPos->y, 0, int(_settings->_chunkPtr->size()) );
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(70);
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

		if (ImGui::BeginMenu("Help")) {
			ImGui::TextColored(ImVec4(0.25f, 0.55f, 0.85f, 1.0f), "Camera");
			ImGui::Text("Lock/unlock camera: R");
			ImGui::Text("Zoom: wheel");
			ImGui::Text("Camera rotation: wheel click");
			ImGui::Text("Move camera to right: D");
			ImGui::Text("Move camera to left: Q");
			ImGui::Text("Move camera to back: S");
			ImGui::Text("Move camera to front: Z");
			ImGui::Text("Move camera up: space bar");
			ImGui::Text("Move camera down: shift");
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(0.25f, 0.55f, 0.85f, 1.0f), "Cursor");
			ImGui::Text("Move cursor to right: numpad " ICON_FA_ARROW_RIGHT);
			ImGui::Text("Move cursor to left: numpad " ICON_FA_ARROW_LEFT);
			ImGui::Text("Move cursor to top: numpad " ICON_FA_ARROW_UP);
			ImGui::Text("Move cursor to bottom: numpad " ICON_FA_ARROW_DOWN);
			ImGui::Text("Move cursor to front: numpad " ICON_FA_MINUS);
			ImGui::Text("Move cursor to back: numpad " ICON_FA_PLUS);
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(0.25f, 0.55f, 0.85f, 1.0f), "Tools");
			ImGui::Text("Select your tool, position the cursor and press 'A' to use your tool.");
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(0.25f, 0.55f, 0.85f, 1.0f), "Lights");
			ImGui::Text("Position the cursor and click the button to add or delete a point light.");
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(0.25f, 0.55f, 0.85f, 1.0f), "Generation with control points");
			ImGui::TextWrapped("Position the cursor and click the button to add or delete a control point. Or, put the number of control point you want and click 'random points' to generate random control points. ");
			ImGui::Text("Click 'Generate map' to generate a map.");
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
	ImGui::SetWindowSize(ImVec2(270, _settings->windowSize().y));
	
	ImGui::ShowDemoWindow();

	Menu::colorPicker();
	Menu::drawTools();
	Menu::lightsSettings();
	Menu::randomGeneration();
	Menu::editCursorPos();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Separator();

	// infos
	ImGui::Text("informations");
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
}

// action callBack functions
void Menu::createAction() {
	_chunkPtr->setColor(*(_settings->_cursorPos), _settings->_colorPick);

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
	_chunkPtr->setColor(*(_settings->_cursorPos), _settings->_colorPick);
}