#pragma once
#include <vector>

#include "Scene.h"
#include "Singleton.h"

class SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager() = default;
	SceneManager(const SceneManager& other) = delete;
	SceneManager(SceneManager&& other) noexcept = delete;
	SceneManager& operator=(const SceneManager& other) = delete;
	SceneManager& operator=(SceneManager&& other) noexcept = delete;
	~SceneManager() override;

	void AddScene(Scene* pScene);
	void SetActiveScene(const char* nameScene);
	void AddGameObject(GameObject* gameObject);
	void Init();
	void Update();
	void Render() const;
	void RenderGUI();
	void Destroy();
private:
	Scene* m_pActiveScene{ nullptr };
	std::vector<Scene*> m_pScenes;
};