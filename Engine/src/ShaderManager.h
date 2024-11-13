#pragma once
#include <vector>

#include "Singleton.h"

class Shader;

class ShaderManager : public Singleton<ShaderManager>
{
public:
	ShaderManager() = default;
	ShaderManager(const ShaderManager& other) = delete;
	ShaderManager(ShaderManager&& other) noexcept = delete;
	ShaderManager& operator=(const ShaderManager& other) = delete;
	ShaderManager& operator=(ShaderManager&& other) noexcept = delete;
	~ShaderManager() override;
	void Destroy();
	void AddShader(Shader* pShader);
private:
	std::vector<Shader*> m_pShaders;
};
