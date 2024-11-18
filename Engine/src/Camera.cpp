#include "Camera.h"

#include "MathHelper.h"
#include "glm/ext/matrix_clip_space.hpp"

#include "GameObject.h"
#include "imgui.h"
#include "glm/ext/matrix_transform.hpp"

Camera* Camera::m_pMainCamera{nullptr};

Camera::Camera(): Component(nullptr)
{
    if(!m_pMainCamera)
	{
        m_IsMainCamera = true;
        m_pMainCamera = this;
	}
}

Camera::Camera(GameObject *pGameObject): Component(pGameObject)
{
}

Camera::~Camera()
{
    if(m_pMainCamera == this)
    {
    	m_pMainCamera = nullptr;
	}
}

void Camera::SetupCamera(float fovAngle, float aspectRatio)
{
    m_FOVAngle = fovAngle;
    m_FOV = tanf(MathHelper::DegToRad(fovAngle) / 2.f);
    m_Aspect = aspectRatio;

    CalculateProjectionMatrix();
    CalculateViewMatrix();
}

void Camera::Update()
{
    CalculateProjectionMatrix();
    CalculateViewMatrix();
}

void Camera::RenderGUI()
{
    ImGui::Text("Camera");
    ImGui::BeginDisabled();
    ImGui::Checkbox("IsMainCamera", &m_IsMainCamera);
    ImGui::EndDisabled();
    ImGui::DragFloat("FOV", &m_FOVAngle);
	ImGui::Text("Aspect: %.2f", m_Aspect);
	ImGui::Text("Near: %.2f", m_Near);
	ImGui::Text("Far: %.2f", m_Far);
    m_FOV = tanf(MathHelper::DegToRad(m_FOVAngle) / 2.f);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return m_ProjectionMatrix;
}

glm::mat4 Camera::GetViewMatrix() const
{
    return m_ViewMatrix;

}

bool Camera::IsMainCamera() const
{
	return m_IsMainCamera;
}

float Camera::GetFOVAngle() const
{
    return m_FOVAngle;
}

Camera* Camera::GetMainCamera()
{
    return m_pMainCamera;
}

void Camera::CalculateProjectionMatrix()
{
    m_ProjectionMatrix = glm::perspective(m_FOV, m_Aspect, m_Near, m_Far);
}

void Camera::CalculateViewMatrix()
{
    m_ViewMatrix = glm::lookAt(GetGameObject()->GetTransform()->GetLocalPosition(),
    GetGameObject()->GetTransform()->GetLocalPosition() + GetGameObject()->GetTransform()->GetForward(),
        { 0, 1, 0 });
}
