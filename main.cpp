#include "Application.h"

#include "resource.h" //sounds

#include <string>

// Pointers NULL
Application *app = NULL; // Pointer to the app

void framerate_cap(Uint32 frame, const int fps) {
  if ((1000 / fps) > SDL_GetTicks64() - frame) {
    SDL_Delay(1000 / fps - (SDL_GetTicks64() - frame));
  }
}

int main(int argc, char *args[]) {
  app = new Application();
  app->init();

  Uint32 starting_tick;

  // Application loop
  while (app->running()) {
    Uint64 start_timer = SDL_GetPerformanceCounter(); // Start FPS Clock
    starting_tick = SDL_GetTicks64();                 // Get current clock ticks

    // Game handling events
    app->handleEvents();
    app->update();
    app->render();
    // Cap FPS and Wait timer
    framerate_cap(starting_tick, app->fps);

    Uint64 end_timer = SDL_GetPerformanceCounter(); // End FPS clock after delay
    // Calculate and print framerate
    float elapsed_time =
        (end_timer - start_timer) / (float)SDL_GetPerformanceFrequency();
    std::string currentFPS =
        app->windowTitle + std::to_string(1.0f / elapsed_time);
    SDL_SetWindowTitle(app->getWindow(), currentFPS.c_str());

    app->current_fps = 1.0f / elapsed_time; // pass to application current fps
  }
  app->clean();
  delete app;
  return 0;
}