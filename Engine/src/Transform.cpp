#include "Transform.h"

#include <iostream>
#include <ostream>

#include "imgui.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

Transform::Transform(): Component{nullptr}, m_pParent{nullptr}
{
}

Transform::Transform(GameObject *pGameObject): Component{pGameObject}
{
}

void Transform::SetPosition(glm::vec3 newPosition)
{
	SetLocalPosition(newPosition);
    SetPositionDirty();
}

void Transform::SetLocalPosition(glm::vec3 newPosition)
{
    m_LocalPosition = newPosition;
}

void Transform::SetRotation(glm::vec3 newRotation)
{
    SetLocalRotation(newRotation);
    SetRotationDirty();
}

void Transform::SetLocalRotation(glm::vec3 newRotation)
{
    m_LocalRotation = newRotation;
}

void Transform::SetScale(glm::vec3 newScale)
{
    SetLocalScale(newScale);
    SetScaleDirty();
}

void Transform::SetLocalScale(glm::vec3 newScale)
{
    m_LocalScale = newScale;
}

void Transform::RemoveChild(Transform *child)
{
    if(!child) {
        std::cerr << "Child pointer is null\n";
        return;
    }

    const auto it = std::ranges::find_if(m_pChildren,[child](Transform* other) {
        return child == other;
    });

    if(it != m_pChildren.end()) {
        m_pChildren.erase(it);
        child->SetParent(nullptr);
    }
}

void Transform::AddChild(Transform *child) {
    if(IsEqualToParent(child) || child == nullptr) {
        std::cerr << "Child pointer is null\n";
        return;
    }

    child->GetParent()->RemoveChild(child);

    m_pChildren.emplace_back(child);
}

void Transform::SetParent(Transform *pNewParent, bool keepWorldPosition)
{
    if(m_pParent == pNewParent || IsChild(pNewParent) || pNewParent == this)
    {
        return;
    }

    if(pNewParent == nullptr)
    {
        SetLocalPosition(GetPosition());
        SetLocalRotation(GetRotation());
    }
    else
    {
        if(keepWorldPosition)
        {
            SetLocalPosition(GetPosition() - pNewParent->GetPosition());
            SetLocalRotation(GetRotation() - pNewParent->GetRotation());
        }
        SetPositionDirty();
        SetRotationDirty();
    }
    if(m_pParent != nullptr) {
        m_pParent->RemoveChild(this);
    }

    m_pParent = pNewParent;

    if(m_pParent != nullptr) {
        m_pParent->AddChild(this);
    }
}

glm::vec3 Transform::GetPosition()
{
    if(m_pParent == nullptr)
    {
    	return GetLocalPosition();
	}
    if(m_IsPositionDirty)
    {
        UpdatePosition();
    }
    return m_Position;
}

glm::vec3 Transform::GetLocalPosition() const
{
    return m_LocalPosition;
}

glm::vec3 Transform::GetRotation()
{
    if (m_pParent == nullptr)
    {
        return GetLocalRotation();
    }
	if(m_IsRotationDirty)
    {
        UpdateRotation();
    }
    return m_Rotation;
}

glm::vec3 Transform::GetLocalRotation() const
{
    return m_LocalRotation;
}

glm::vec3 Transform::GetScale()
{
    if (m_pParent == nullptr)
    {
        return GetLocalScale();
    }
	if(m_IsScaleDirty)
    {
        UpdateScale();
    }
    return m_Scale;
}

glm::vec3 Transform::GetLocalScale() const
{
    return m_LocalScale;
}

Transform * Transform::GetParent() const {
    return m_pParent;
}

void Transform::RenderGUI()
{
    ImGui::Text("Transform");
    float localPosition[3]{ GetLocalPosition().x, GetLocalPosition().y, GetLocalPosition().z };
    float localRotation[3]{ GetLocalRotation().x, GetLocalRotation().y, GetLocalRotation().z };
    float localScale[3]{ GetLocalScale().x, GetLocalScale().y, GetLocalScale().z };
    ImGui::DragFloat3("Position", localPosition,0.1f);
    ImGui::DragFloat3("Rotation", localRotation,0.1f);
    ImGui::DragFloat3("Scale", localScale);
    SetLocalPosition(glm::vec3{ localPosition[0],localPosition[1],localPosition[2] });
    SetLocalRotation(glm::vec3{ localRotation[0],localRotation[1],localRotation[2] });
    SetLocalScale(glm::vec3{ localScale[0],localScale[1],localScale[2] });
}

void Transform::Update()
{
    CalculateForwardAndRight();
    CalculateModelMatrix();
}

glm::mat4 Transform::GetModelMatrix() const
{
	return m_ModelMatrix;
}

glm::vec3 Transform::GetForward() const
{
	return m_Forward;
}

bool Transform::IsChild(Transform *pParent)
{
    const auto it = std::ranges::find_if(m_pChildren, [pParent](Transform *pChild) {
        return pChild == pParent;
    });
    return it != m_pChildren.end();
}

bool Transform::IsEqualToParent(Transform *child) {
    Transform* parent{child->m_pParent};
    while(parent != nullptr) {
        if(parent == child) {
            return true;
        }
        parent = parent->GetParent();
    }
    return false;
}

void Transform::SetPositionDirty()
{
    m_IsPositionDirty = true;
    for(const auto& child : m_pChildren)
    {
        child->SetPositionDirty();
    }
}

void Transform::SetRotationDirty()
{
    m_IsRotationDirty = true;
    for(const auto& child: m_pChildren)
    {
        child->SetRotationDirty();
    }
}

void Transform::SetScaleDirty()
{
    m_IsScaleDirty = true;
    for(const auto& child: m_pChildren)
    {
        child->SetScaleDirty();
    }
}

void Transform::UpdatePosition()
{
    if(m_IsPositionDirty)
    {
        if (m_pParent == nullptr)
        {
            m_Position = GetLocalPosition();
        }
        else
        {
            m_Position = m_pParent->GetPosition() + GetLocalPosition();
        }
        m_IsPositionDirty = false;
    }
}

void Transform::UpdateRotation()
{
    if(m_IsRotationDirty)
    {
        if (m_pParent == nullptr)
        {
            m_Rotation = GetLocalRotation();
        }
        else
        {
            m_Rotation = m_pParent->GetRotation() + GetLocalRotation();
        }
        m_IsRotationDirty = false;
    }
}

void Transform::UpdateScale()
{
    if(m_IsScaleDirty)
    {
        if (m_pParent == nullptr)
        {
            m_Scale = GetLocalScale();
        }
        else
        {
            m_Scale = m_pParent->GetScale() + GetLocalScale();
        }
        m_IsScaleDirty = false;
    }
}

void Transform::CalculateForwardAndRight()
{
    glm::mat4 rotation{ glm::rotate(glm::mat4(1.f), glm::radians(GetLocalRotation().y), {0, 1, 0}) };
    rotation = glm::rotate(rotation, glm::radians(GetLocalRotation().x), { 1, 0, 0 });

    m_Forward = glm::normalize(glm::vec3(rotation * glm::vec4(0, 0, -1, 0))); // -1 sur Z pour la direction avant

    m_Right = glm::normalize(glm::cross(m_Forward, { 0, 1, 0 }));

    m_Up = glm::normalize(glm::cross(m_Right, m_Forward));  // Utilise Right et Forward pour recalculer Up
}

void Transform::CalculateModelMatrix()
{
    glm::mat4 model{ 1 };
    model = translate(model, GetLocalPosition());
    model = rotate(model, glm::radians(GetLocalRotation().y), { 0, 1, 0 });
    model = rotate(model, glm::radians(GetLocalRotation().x), { 1, 0, 0 });
    model = rotate(model, glm::radians(GetLocalRotation().z), { 0, 0, 1 });
    model = scale(model, GetLocalScale());
    m_ModelMatrix = model;
}

