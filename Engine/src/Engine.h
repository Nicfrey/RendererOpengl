#pragma once
#include <functional>
#include <gl/glew.h>

#include "glm/vec2.hpp"

class Shader;

class Engine
{
    public:
        Engine();
        Engine(GLuint width, GLuint height, const char* title);
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(Engine&&) = delete;
        ~Engine();
        void Start(const std::function<void()>& load);
		void SetShader(Shader* shader) { m_Shader = shader; }
		static glm::ivec2 GetWindowSize();  
    private:
		Shader* m_Shader;
		static glm::ivec2 m_WindowSize;
        const char* m_Title{"OpenGL Window"};
        void CreateWindowApp(int width, int height, const char* title);
        void InitializeGlew();
        void StartHeapControl();
        void DumpMemoryLeaks();
};
