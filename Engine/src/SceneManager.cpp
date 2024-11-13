#include "SceneManager.h"

#include <iostream>

SceneManager::~SceneManager()
{
	Destroy();
}

void SceneManager::AddScene(Scene* pScene)
{
	// Check if a scene with that name already exist
	const auto it{ std::ranges::find_if(m_pScenes,[pScene](const Scene* other)
	{
		return pScene->GetName() == other->GetName();
	}) };
	if(it != m_pScenes.end())
	{
		std::cerr << "Can't create scene when already exists\n";
		return;
	}
	m_pScenes.emplace_back(pScene);
	if(m_pScenes.size() == 1)
	{
		m_pActiveScene = pScene;
	}
}

void SceneManager::SetActiveScene(const char* nameScene)
{
	const auto it{ std::ranges::find_if(m_pScenes,[nameScene](const Scene* pScene)
	{
		return pScene->GetName() == nameScene;
	}) };
	if(it != m_pScenes.end())
	{
		m_pActiveScene = *it;
	}
}

void SceneManager::AddGameObject(GameObject* gameObject)
{
	if(!m_pActiveScene)
	{
		std::cerr << "Set an active scene\n";
		return;
	}
	m_pActiveScene->AddGameObject(gameObject);
}

void SceneManager::Init()
{
	if(!m_pActiveScene)
	{
		std::cerr << "Set an active scene\n";
		return;
	}
	m_pActiveScene->Init();
}

void SceneManager::Update()
{
	if(!m_pActiveScene)
	{
		std::cerr << "Set an active scene\n";
		return;
	}
	m_pActiveScene->Update();
}

void SceneManager::Render() const
{
	if(!m_pActiveScene)
	{
		std::cerr << "Set an active scene\n";
		return;
	}
	m_pActiveScene->Render();
}

void SceneManager::RenderGUI()
{
	if(!m_pActiveScene)
	{
		std::cerr << "Set an active scene\n";
		return;
	}
	m_pActiveScene->RenderGUI();
}

void SceneManager::Destroy()
{
	for (const Scene* pScene : m_pScenes)
	{
		delete pScene;
		pScene = nullptr;
	}
	m_pActiveScene = nullptr;
	m_pScenes.clear();
}
