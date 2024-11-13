#include "ShaderManager.h"

#include "Shader.h"

ShaderManager::~ShaderManager()
{
	Destroy();
}

void ShaderManager::Destroy()
{
	for(auto& shader : m_pShaders)
	{
		delete shader;
		shader = nullptr;
	}
	m_pShaders.clear();
}

void ShaderManager::AddShader(Shader* pShader)
{
	const auto it{ std::ranges::find(m_pShaders, pShader) };
	if(it == m_pShaders.end())
	{
		m_pShaders.push_back(pShader);
	}
}
