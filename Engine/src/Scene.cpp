#include "Scene.h"

#include "imgui.h"

Scene::Scene(const char* pName): m_pName{pName}
{
}

Scene::~Scene()
{
	for(const GameObject* gameObject: m_pGameObjects)
	{
		if(gameObject)
		{
			delete gameObject;
			gameObject = nullptr;
		}
	}
	m_pGameObjects.clear();
}

const char* Scene::GetName() const
{
	return m_pName;
}

void Scene::AddGameObject(GameObject* gameObject)
{
	m_pGameObjects.emplace_back(gameObject);
}

void Scene::Init()
{
	for(GameObject* gameObject: m_pGameObjects)
	{
		gameObject->Init();
	}
}

void Scene::Update()
{
	for(GameObject* gameObject: m_pGameObjects)
	{
		gameObject->Update();
	}
}

void Scene::Render() const
{
	for(const GameObject* gameObject: m_pGameObjects)
	{
		gameObject->Render();
	}
}

void Scene::RenderGUI()
{
	ImGui::Begin(m_pName);
	int index{ 0 };
	for(GameObject* gameObject: m_pGameObjects)
	{
		ImGui::PushID(index++);
		gameObject->RenderGUI();
		ImGui::PopID();
	}
	ImGui::End();
}
