#ifndef APPLICATION_HPP
#define APPLICATION_HPP

// Version Number
#define VERSION_MAJOR 0
#define VERSION_MINOR 28
#define VERSION_PATCH
#define VERSION_ALT -minimal
#define STR_HELPER(x) #x // convert to fit window title
#define STR(x) STR_HELPER(x)

// Screen Definitions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>
#include <json/json.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

using namespace glm;

// PreProcessor Declarations
#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#include <unistd.h>
#endif

// Some forward declarations for pointers
class Player;
class Enemy;
class Terrain;
class Physics;

class Application
{
public:
  const char *windowTitle =
      "C23 Engine: SDL2 v." STR(VERSION_MAJOR) "." STR(VERSION_MINOR)
          STR(VERSION_PATCH) STR(VERSION_ALT) " FPS:";
  Application();
  ~Application();

  bool init();
  void handleMouse(SDL_Event event);
  void handleEvents();
  void update();
  void render();
  void clean();
  bool running() { return gameRunning; }

  // Getters
  SDL_Window *getWindow() { return window; }
  const int getScreenWidth() { return SCREEN_WIDTH; }
  const int getScreenHeight() { return SCREEN_HEIGHT; }

private:
  SDL_Window *window;
  SDL_GLContext glContext;

  // OpenGL related
  GLuint shaderProgram;
  GLuint VAO, VBO;

  // Camera
  vec3 cameraPos;
  vec3 cameraFront;
  vec3 cameraUp;
  float yaw;
  float pitch;
  // Mouse input for camera rotation
  float lastX, lastY;
  float mouseSensitivity;
  bool firstMouse;

  // Game state
  bool gameRunning;
  bool debugMode;

  // Frame counting
  int frameCount;
  int startTime;
  int endTime;
  int timeDifference;
  float frameAverage;

  // Helper function for shader creation
  GLuint createShader(GLenum type, const char *source);
};

#endif