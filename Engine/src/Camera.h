#pragma once
#include <vector>

#include "Component.h"
#include <glm/glm.hpp>

class Camera final : public Component
{
public:
    Camera();
    explicit Camera(GameObject* pGameObject);
    Camera(const Camera& camera) = delete;
    Camera(Camera&& camera) noexcept = delete;
    Camera& operator=(const Camera& camera) = delete;
    Camera& operator=(Camera&& camera) noexcept = delete;
    ~Camera() override;
    void SetupCamera(float fovAngle, float aspectRatio);
    void Update() override;
    void RenderGUI() override;
    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetViewMatrix() const;
    bool IsMainCamera() const;
    static Camera* GetMainCamera();

private:
    float m_FOV;
    float m_FOVAngle;
    float m_Aspect;
    float m_Near{.1f};
    float m_Far{1000.f};
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    bool m_IsMainCamera{false};

    void CalculateProjectionMatrix();
    void CalculateViewMatrix();
    static Camera* m_pMainCamera;
};