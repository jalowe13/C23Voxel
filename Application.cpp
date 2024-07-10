#include "Application.h"

// Upgrade to C++ 11

#ifdef _WIN32
#else
uint32_t linux_tick() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ((ts.tv_sec * 1000) + (ts.tv_nsec / 1000000));
}
#endif

Application::Application() {
  std::cout << "-----Application Created\n";
  gameRunning = true;
  frameCount = 0;
// Windows and Linux Definitions
#ifdef _WIN32
  startTime = GetTickCount();
  endTime = GetTickCount();
  setRefreshRate();
#else
  startTime = linux_tick();
  endTime = linux_tick();
#endif
  timeDifference = 0;
  frameAverage = 0;
}

Application::~Application() { std::cout << "-----Application Destroyed\n"; }

bool Application::init() {
  try {
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
      std::cout << "-----SDL Init Done" << std::endl;
      window = SDL_CreateWindow(
          windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

      if (!window) {
        throw "Window creation failed!";
      }

      std::cout << "-----Window Created" << std::endl;

      renderer =
          SDL_CreateRenderer(window, -1,
                             SDL_RENDERER_PRESENTVSYNC |
                                 SDL_RENDERER_ACCELERATED); // Create renderer
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Set clear color to white

      if (!renderer) {
        throw "Renderer creation failed.";
      }

      std::cout << "-----Renderer Created" << std::endl;

      std::cout << "-----Start ImGUI\n";
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      io = ImGui::GetIO();
      io.ConfigFlags |=
          ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
      io.ConfigFlags |=
          ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
      ImGui::StyleColorsDark();
      // Setup Platform/Renderer backends
      ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
      ImGui_ImplSDLRenderer2_Init(renderer);
      std::cout << "-----ImGUI Created" << std::endl;
      // Loading texture memory
      SDL_Texture *temp_tex = NULL;

      gameRunning = true;
    } else {
      gameRunning = false;
    }

    return true;
  } catch (const char *error) {
    std::cout << error << std::endl;
    return false;
  }
}

void Application::setRefreshRate() {
  DEVMODE dm;
  dm.dmSize = sizeof(dm);
  EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
  this->fps = dm.dmDisplayFrequency;
  std::cout << "Refresh Rate: " << this->fps << std::endl;
}

void Application::handleEvents() {
  SDL_Event event;
  SDL_PollEvent(&event);
  ImGui_ImplSDL2_ProcessEvent(&event);
  switch (event.type) {
  case SDL_MOUSEMOTION:
    SDL_GetMouseState(&xMouse, &yMouse);
    break;
  case SDL_MOUSEBUTTONDOWN:
    mouseDown = true;
    break;
  case SDL_MOUSEBUTTONUP:
    mouseDown = false;
    break;
  case SDL_QUIT:
    gameRunning = false;
    break;
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {}
    break;
  case SDL_KEYUP:
    switch (event.key.keysym.sym) {
    case SDLK_w: {
      break;
    }
    case SDLK_a: {
      break;
    }
    case SDLK_s: {
      break;
    }
    case SDLK_d: {
      break;
    }
    // Debug Mode Toggle
    case SDLK_BACKQUOTE: {
      debugMode = (debugMode) ? false : true;
      std::cout << "Debug Mode: " << debugMode << std::endl;
      break;
    }
    case SDLK_KP_0: {
      break;
    }
    case SDLK_KP_1: {
      break;
    }
    case SDLK_KP_2: {
    }
    }
    break;
  default:
    break;
  }
}

void Application::update() // Update Logic
{
  // Logic --> Physics --> Render
}

void Application::render() {
  SDL_SetRenderDrawColor(
      renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255),
      (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
  if (debugMode) {
    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    // Frame actions
    static float f = 0.0f;
    static int counter = 0;
    ImGui::Text("Hello, world %d", 123);
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

    // Rendering Frame
    ImGui::Render();
    SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x,
                       io.DisplayFramebufferScale.y);
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
  } else {
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
  }
}

void Application::clean() {
  SDL_DestroyWindow(window); // destroy the window
  SDL_Quit();                // quit and delete all SDL
}
