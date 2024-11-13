#pragma once
#include <vector>

#include "GameObject.h"

class Scene final
{
public:
	Scene(const char* pName);
	~Scene();

	const char* GetName() const;
	void AddGameObject(GameObject* gameObject);
	void Init();
	void Update();
	void Render() const;
	void RenderGUI();

private:
	const char* m_pName;
	std::vector<GameObject*> m_pGameObjects{};
};
