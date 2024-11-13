#pragma once
class Transform;
class GameObject;

class Component
{
public:
    virtual ~Component() = default;

    Component();
    explicit Component(GameObject* pGameObject);

    virtual void Start() {}
    virtual void Update() {}
    virtual void Init() {}
    virtual void Render() const {}
    virtual void RenderGUI() {}
    
    void SetGameObject(GameObject* pGameObject);
    void RemoveGameObject();
	GameObject* GetGameObject() const;
	Transform* GetTransform() const;
private:
    GameObject* m_pGameObject;
};
