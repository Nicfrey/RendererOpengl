#pragma once
#include <vector>

#include "Component.h"
#include "glm/vec3.hpp"

enum class LightType
{
	Directional,
	Point
};

class Light final : public Component
{
public:
	Light(float intensity = 5.f, const glm::vec3& color = {1.f,1.f,1.f}, LightType type = LightType::Directional);
	~Light() override;
	Light(const Light& other) = delete;
	Light(Light&& other) noexcept = delete;
	Light& operator=(const Light& other) = delete;
	Light& operator=(Light&& other) noexcept = delete;
	void RenderGUI() override;
	glm::vec3 m_Color;
	float m_Intensity;
	LightType m_Type;
	static Light* GetDirectionalLight();
private:
	static Light* m_pDirectionLight;
};