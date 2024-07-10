#include "Application.h"
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <glm/gtc/type_ptr.hpp>
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
uint32_t linux_tick()
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ((ts.tv_sec * 1000) + (ts.tv_nsec / 1000000));
}
#endif

const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main()
    {
        gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";

const char *fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
)";

Application::Application()
    : gameRunning(true), frameCount(0), timeDifference(0), frameAverage(0),
      cameraPos(0.0f, 0.0f, 3.0f), cameraFront(0.0f, 0.0f, -1.0f), cameraUp(0.0f, 1.0f, 0.0f),
      yaw(-90.0f), pitch(0.0f), debugMode(true), window(nullptr), glContext(nullptr), lastX(SCREEN_WIDTH / 2.0f), lastY(SCREEN_HEIGHT / 2.0f),
      mouseSensitivity(0.1f), firstMouse(true)
{
  std::cout << "Application Created\n";
#ifdef _WIN32
  startTime = GetTickCount();
  endTime = GetTickCount();
#else
  startTime = linux_tick();
  endTime = linux_tick();
#endif
}

Application::~Application()
{
  std::cout << "Application Destroyed\n";
  clean();
}

bool Application::init()
{
  try
  {
    std::cout << "Initializing SDL..." << std::endl;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
      throw std::runtime_error("SDL initialization failed: " + std::string(SDL_GetError()));
    }

    std::cout << "Setting GL attributes..." << std::endl;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_CaptureMouse(SDL_TRUE);

    std::cout << "Creating window..." << std::endl;
    window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window)
    {
      throw std::runtime_error("Window creation failed: " + std::string(SDL_GetError()));
    }

    std::cout << "Creating GL context..." << std::endl;
    glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
      throw std::runtime_error("OpenGL context creation failed: " + std::string(SDL_GetError()));
    }

    std::cout << "Initializing GLEW..." << std::endl;
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
      throw std::runtime_error("GLEW initialization failed: " + std::string((char *)glewGetErrorString(glewError)));
    }

    std::cout << "Creating shaders..." << std::endl;
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    std::cout << "Creating shader program..." << std::endl;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
      GLchar infoLog[512];
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      throw std::runtime_error("Shader program linking failed: " + std::string(infoLog));
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::cout << "Setting up vertex data..." << std::endl;
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::cout << "Setting up ImGui..." << std::endl;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 330");

    std::cout << "Initialization complete." << std::endl;
    gameRunning = true;
    return true;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error in Application::init(): " << e.what() << std::endl;
    gameRunning = false;
    return false;
  }
}

void Application::render()
{
  try
  {
    std::cout << "Starting render..." << std::endl;

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::cout << "Using shader program..." << std::endl;
    glUseProgram(shaderProgram);

    std::cout << "Creating matrices..." << std::endl;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    std::cout << "Getting uniform locations..." << std::endl;
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

    if (modelLoc == -1 || viewLoc == -1 || projLoc == -1)
    {
      throw std::runtime_error("Failed to get uniform locations");
    }

    std::cout << "Setting uniform values..." << std::endl;
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    std::cout << "Binding VAO and drawing..." << std::endl;
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDrawArrays(GL_LINE_LOOP, 4, 4);
    for (int i = 0; i < 4; ++i)
    {
      glDrawArrays(GL_LINES, i, 2);
      glDrawArrays(GL_LINES, i + 4, 2);
    }

    std::cout << "Starting ImGui rendering..." << std::endl;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug");
    ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", cameraPos.x, cameraPos.y, cameraPos.z);
    ImGui::Text("Yaw: %.2f, Pitch: %.2f", yaw, pitch);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    std::cout << "Swapping window..." << std::endl;
    SDL_GL_SwapWindow(window);

    std::cout << "Render complete." << std::endl;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error in Application::render(): " << e.what() << std::endl;
    gameRunning = false;
  }
}

// Mouse handling
// SDL_Event event is passed in from handleEvents
void Application::handleMouse(SDL_Event event)
{
  if (firstMouse)
  {
    lastX = event.motion.x;
    lastY = event.motion.y;
    firstMouse = false;
  }
  float xoffset = event.motion.x - lastX;
  float yoffset = lastY - event.motion.y;
  lastX = event.motion.x;
  lastY = event.motion.y;

  xoffset *= mouseSensitivity;
  yoffset *= mouseSensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
  {
    pitch = 89.0f;
  }
  if (pitch < -89.0f)
  {
    pitch = -89.0f;
  }

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);
}

void Application::handleEvents()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT)
    {
      gameRunning = false;
    }
    else if (event.type == SDL_MOUSEMOTION)
    {
      handleMouse(event);
    }
    else if (event.type == SDL_KEYDOWN)
    {
      float cameraSpeed = 0.05f; // Adjust this value to change movement speed
      switch (event.key.keysym.sym)
      {
      case SDLK_w:
        cameraPos += cameraSpeed * cameraFront;
        break;
      case SDLK_s:
        cameraPos -= cameraSpeed * cameraFront;
        break;
      case SDLK_a:
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        break;
      case SDLK_d:
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        break;
      case SDLK_SPACE:
        cameraPos += cameraUp * cameraSpeed;
        break;
      case SDLK_LCTRL:
        cameraPos -= cameraUp * cameraSpeed;
        break;
      }
    }
  }
}

void Application::update()
{
  // Update game logic here
}

void Application::clean()
{
  if (ImGui::GetCurrentContext())
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
  }

  if (VAO)
  {
    glDeleteVertexArrays(1, &VAO);
  }
  if (VBO)
  {
    glDeleteBuffers(1, &VBO);
  }
  if (shaderProgram)
  {
    glDeleteProgram(shaderProgram);
  }

  if (glContext)
  {
    SDL_GL_DeleteContext(glContext);
  }
  if (window)
  {
    SDL_DestroyWindow(window);
  }
  SDL_Quit();
}

GLuint Application::createShader(GLenum type, const char *source)
{
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::string shaderType = (type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
    throw std::runtime_error(shaderType + " shader compilation failed: " + std::string(infoLog));
  }

  return shader;
}