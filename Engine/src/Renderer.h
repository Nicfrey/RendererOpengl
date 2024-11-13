#pragma once
#include "Singleton.h"
#include "GLFW/glfw3.h"

class Renderer : public Singleton<Renderer>
{
public:
	explicit Renderer() = default;
	void Init(GLFWwindow* pWindow);
	void Render() const;
	void Destroy();
	GLFWwindow* m_pWindow;
private:
};
