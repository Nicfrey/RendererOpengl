#include "Light.h"

#include "imgui.h"

Light* Light::m_pDirectionLight{nullptr};

Light::Light(float intensity, const glm::vec3& color, LightType type)
	: m_Color(color), m_Intensity(intensity), m_Type(type)
{
	if(!m_pDirectionLight && m_Type == LightType::Directional)
		m_pDirectionLight = this;
}

Light::~Light()
{
	if(m_pDirectionLight == this)
	{
		m_pDirectionLight = nullptr;
	}
}

void Light::RenderGUI()
{
	ImGui::Text("Light");
	ImGui::ColorEdit3("Color", &m_Color.r);
	ImGui::SliderFloat("Intensity", &m_Intensity, 0.f, 100.f);
	ImGui::Text("Type");
	ImGui::RadioButton("Directional", reinterpret_cast<int*>(&m_Type), static_cast<int>(LightType::Directional));
	ImGui::RadioButton("Point", reinterpret_cast<int*>(&m_Type), static_cast<int>(LightType::Point));
}

Light* Light::GetDirectionalLight()
{
	return m_pDirectionLight;
}
