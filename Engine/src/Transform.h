#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

#include "Component.h"

class Transform : public Component
{
public:
    Transform();
    explicit Transform(GameObject* pGameObject);
    void SetPosition(glm::vec3 newPosition);
    void SetLocalPosition(glm::vec3 newPosition);
    void SetRotation(glm::vec3 newRotation);
    void SetLocalRotation(glm::vec3 newRotation);
	void SetScale(glm::vec3 newScale);
    void SetLocalScale(glm::vec3 newScale);

    void RemoveChild(Transform * child);
    void AddChild(Transform * child);

    void SetParent(Transform* pNewParent, bool keepWorldPosition = true);

    glm::vec3 GetPosition();
    glm::vec3 GetLocalPosition() const;
    glm::vec3 GetRotation();
    glm::vec3 GetLocalRotation() const;
    glm::vec3 GetScale();
    glm::vec3 GetLocalScale() const;
    Transform* GetParent() const;
    void RenderGUI() override;
    void Update() override;
    glm::mat4 GetModelMatrix() const;
    glm::vec3 GetForward() const;

private:
    bool IsChild(Transform* pParent);
    bool IsEqualToParent(Transform * child);
    Transform* m_pParent;
    std::vector<Transform*> m_pChildren;
    glm::mat4 m_ModelMatrix;
    void SetPositionDirty();
    void SetRotationDirty();
    void SetScaleDirty();

    void UpdatePosition();
    void UpdateRotation();
    void UpdateScale();

    void CalculateForwardAndRight();
    void CalculateModelMatrix();

    bool m_IsPositionDirty{};
    bool m_IsRotationDirty{};
    bool m_IsScaleDirty{};

    glm::vec3 m_Position{};
    glm::vec3 m_LocalPosition{};
    /**
     * \brief x: pitch, y : yaw, z : roll
     */
    glm::vec3 m_Rotation{};
    /**
     * \brief x: pitch, y : yaw, z : roll
     */
    glm::vec3 m_LocalRotation{};
    glm::vec3 m_Scale{1.f};
    glm::vec3 m_LocalScale{1.f};

    glm::vec3 m_Forward{0,0,1};
    glm::vec3 m_Right{1,0,0};
    glm::vec3 m_Up{0,1,0};
};
