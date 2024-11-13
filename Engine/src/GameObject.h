#pragma once
#include "Transform.h"
#include "Component.h"
#include <vector>

class GameObject
{
public:
    GameObject();
    GameObject(const char* pName);
    ~GameObject();
    template<typename Comp>
    bool AddComponent(Comp* pComponent);
    template<typename Comp>
    bool RemoveComponent();
    template<typename Comp>
    bool RemoveComponent(Comp* component);
    template<typename Comp>
    Comp* GetComponent() const;
    template<typename Comp>
    bool IsComponentInGameObject(Comp* pComponent);
    template<typename Comp>
    bool HasComponent() const;
    Transform* GetTransform() const;

    std::vector<GameObject*> GetChildren() const;
    void Init();
    void Update();
    void Render() const;
    void RenderGUI();

private:
    const char* m_pName;
    Transform* m_pTransform;
    std::vector<Component*> m_pComponents;
    std::vector<GameObject*> m_pChildren;
};

template <typename Comp>
inline bool GameObject::AddComponent(Comp *pComponent)
{
    if(dynamic_cast<Component*>(pComponent) == nullptr)
    {
    	return false;
	}
    pComponent->SetGameObject(this);
    m_pComponents.emplace_back(pComponent);
    return true;
}

template <typename Comp>
inline bool GameObject::RemoveComponent()
{
    auto it{std::ranges::find_if(m_pComponents,[](Component* pComponent) {
        return dynamic_cast<Comp*>(pComponent) != nullptr;
    })};
    if(it != m_pComponents.end())
    {
        (*it)->RemoveGameObject();
        m_pComponents.erase(it);
        return true;
    }
    return false;
}

template <typename Comp>
bool GameObject::RemoveComponent(Comp* component)
{
    if(IsComponentInGameObject(component))
    {
        auto it{ std::ranges::find_if(m_pComponents,[component](Component* other)
        {
            return dynamic_cast<Comp*>(other) == component;
        }) };
        m_pComponents.erase(it);
        return true;
    }
    return false;
}

template<typename Comp>
Comp * GameObject::GetComponent() const
{
    auto it{std::ranges::find_if(m_pComponents,[](Component* pComponent) {
        return dynamic_cast<Comp*>(pComponent) != nullptr;
    })};
    if(it != m_pComponents.end())
    {
        return dynamic_cast<Comp>(*it);
    }
    return nullptr;
}

template <typename Comp>
bool GameObject::IsComponentInGameObject(Comp* pComponent)
{
    auto it{ std::ranges::find_if(m_pComponents,[pComponent](Component* pOther)
    {
        return dynamic_cast<Comp*>(pOther) == pComponent;
    }) };
	return it != m_pComponents.end();
}

template<typename Comp>
bool GameObject::HasComponent() const
{
    auto it{std::ranges::find_if(m_pComponents,[](Component* pComponent) {
        return dynamic_cast<Comp*>(pComponent) != nullptr;
    })};
    return it != m_pComponents.end();
}
