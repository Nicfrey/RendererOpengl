#include "Component.h"

#include "GameObject.h"

Component::Component(): Component{nullptr}
{
}

Component::Component(GameObject *pGameObject): m_pGameObject{pGameObject}
{
}


void Component::SetGameObject(GameObject *pGameObject)
{
    if(!pGameObject)
        return;
    if(m_pGameObject)
    {
        m_pGameObject->RemoveComponent(this);
    }
    m_pGameObject = pGameObject;
}

void Component::RemoveGameObject()
{
    m_pGameObject = nullptr;
}

GameObject *Component::GetGameObject() const
{
    return m_pGameObject;
}

Transform* Component::GetTransform() const
{
	return m_pGameObject->GetTransform();
}
