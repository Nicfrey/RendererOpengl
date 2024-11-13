#include "TextComponent.h"

#include "Font.h"
#include "FontManager.h"
#include "GameObject.h"
#include "imgui.h"

TextComponent::TextComponent(const char* pathFont,const std::string& text , int size, const glm::vec3& color): m_Color(color), m_Text(text)
{
	if(Font* font{ FontManager::GetInstance().GetFont(pathFont) })
	{
		m_pFont = font;
	}
	else
	{
		m_pFont = new Font{ pathFont, size };
		FontManager::GetInstance().AddFont(m_pFont);
	}
}

void TextComponent::Render() const
{
	m_pFont->Render(m_Text, GetTransform()->GetLocalPosition().x, GetTransform()->GetLocalPosition().y, GetTransform()->GetScale().x, m_Color);
}

void TextComponent::RenderGUI()
{
	int size { m_pFont->GetSize() };
	ImGui::Text("Text Component");
	ImGui::DragInt("Size", &size, 1, 8, 128);
	if(size != m_pFont->GetSize())
	{
		m_pFont->SetSize(size);
	}
}

