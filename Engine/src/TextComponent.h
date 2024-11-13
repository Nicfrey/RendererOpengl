#pragma once
#include <string>

#include "Component.h"
#include "glm/vec3.hpp"

class Font;

class TextComponent : public Component
{
public:
	TextComponent(const char* pathFont, const std::string& text = "",int size = 32, const glm::vec3& color = glm::vec3{1.f});
	~TextComponent() override = default;
	TextComponent(const TextComponent& other) = delete;
	TextComponent(TextComponent&& other) noexcept = delete;
	TextComponent() = delete;
	TextComponent& operator=(const TextComponent& other) = delete;
	TextComponent& operator=(TextComponent&& other) noexcept = delete;
	void Render() const override;
	void RenderGUI() override;
	std::string m_Text;
	glm::vec3 m_Color;
private:
	Font* m_pFont;
};
