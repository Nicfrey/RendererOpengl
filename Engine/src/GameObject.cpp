#include "GameObject.h"

#include "imgui.h"

GameObject::GameObject(): m_pName{"GameObject"}, m_pTransform(new Transform{})
{
	AddComponent(m_pTransform);
}

GameObject::GameObject(const char* pName): GameObject{}
{
	m_pName = pName;
}

GameObject::~GameObject()
{
    for(const Component* c : m_pComponents)
    {
        delete c;
        c = nullptr;
    }
}

Transform* GameObject::GetTransform() const
{
    return m_pTransform;
}

std::vector<GameObject *> GameObject::GetChildren() const
{
    return m_pChildren;
}

void GameObject::Init()
{
	for(Component* c : m_pComponents)
	{
		c->Init();
	}
}

void GameObject::Update()
{
	for(Component* c : m_pComponents)
	{
		c->Update();
	}
}

void GameObject::Render() const
{
	for(const Component* c : m_pComponents)
	{
		c->Render();
	}
}

void GameObject::RenderGUI()
{
	ImGui::TextColored(ImVec4{ 1,0,0,1 }, m_pName);
	for(Component* c : m_pComponents)
	{
		c->RenderGUI();
		ImGui::Separator();
	}
}
