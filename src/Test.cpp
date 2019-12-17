#include <iostream>
#include <string>

// SDL
#include <SDL.h>

// IMGUI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>

// GLAD
#include <glad/glad.h>

// GLM & glm transform
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 
#include "App.hpp"
#include "world/Cursor.hpp"
#include "world/Camera.hpp"

int main(int argc, char *argv[]) {
    
  std::cout << "---------- Test File ----------" << std::endl;

  App app(1000, 700, "test cursor");

  app.loop();

  return 0;
}