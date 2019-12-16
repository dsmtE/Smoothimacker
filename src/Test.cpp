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

  world::Camera c(glm::vec3(0, 0, 5.f));
  App app(500, 500, "test cursor", c);

  openGL::Shader shader("assets/shaders/cursor.vert", "assets/shaders/cursor.frag", true);

  world::Cursor cursor; 

  while (app.isRunning()) {
	  app.beginFrame();
      shader.bind();
      shader.setMat4("view", app.getCam().getViewMatrix());
      shader.setMat4("projection", app.getCam().getProjectionMatrix(500, 500));
      shader.setMat4("model", app.getCursor().getModelMatrix());
	  app.getCursor().draw(shader);
      shader.unBind();
	  app.endFrame();
  }
  return 0;
}