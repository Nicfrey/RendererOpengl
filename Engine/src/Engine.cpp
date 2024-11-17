#include "Engine.h"

#define MS_PER_FRAME 8

#include <windows.h>
#include <chrono>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <crtdbg.h>
#include <heapapi.h>


#include "Font.h"
#include "FontManager.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "TimeEngine.h"
#include "freetype/freetype.h"


glm::ivec2 Engine::m_WindowSize{800,600};

Engine::Engine()
{
    CreateWindowApp(m_WindowSize.x,m_WindowSize.y,m_Title);
    InitializeGlew();
}

Engine::Engine(GLuint width, GLuint height, const char *title): m_Title(title)
{
    m_WindowSize = glm::ivec2(width,height);
    CreateWindowApp(m_WindowSize.x, m_WindowSize.y,title);
    InitializeGlew();
}

Engine::~Engine()
{
	DumpMemoryLeaks();
}

void Engine::Start(const std::function<void()>& load)
{
    StartHeapControl();
    load();
    std::cout << "Engine started!" << "\n";
    auto lastTime{ std::chrono::high_resolution_clock::now() };
    SceneManager& sceneManager{ SceneManager::GetInstance() };
    Renderer& renderer{ Renderer::GetInstance() };
    FontManager& fontManager{ FontManager::GetInstance() };
    ShaderManager& shaderManager{ ShaderManager::GetInstance() };

    sceneManager.Init();
    while (!glfwWindowShouldClose(renderer.m_pWindow))
    {
        glfwPollEvents();
        const auto currentTime{ std::chrono::high_resolution_clock::now() };
        TimeEngine::GetInstance().SetDeltaTime(std::chrono::duration<float>(currentTime - lastTime).count());
        sceneManager.Update();
        renderer.Render();
        const auto sleepTime{ currentTime + std::chrono::milliseconds(MS_PER_FRAME) - std::chrono::high_resolution_clock::now() };
        std::this_thread::sleep_for(sleepTime);
    }
    fontManager.Destroy();
	shaderManager.Destroy();
    sceneManager.Destroy();
    renderer.Destroy();
}

glm::ivec2 Engine::GetWindowSize()
{
    return m_WindowSize;
}

void Engine::CreateWindowApp(int width, int height, const char* title)
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create window!");
    }
    glfwMakeContextCurrent(window);
    Renderer::GetInstance().Init(window);
}

void Engine::InitializeGlew()
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        throw std::runtime_error("Failed to initialize GLEW!");
    }
}

void Engine::StartHeapControl()
{
#if defined(DEBUG) | defined(_DEBUG)
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // Breakpoint at specific object location
	//_CrtSetBreakAlloc(518);
#endif
}

void Engine::DumpMemoryLeaks()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif
}
