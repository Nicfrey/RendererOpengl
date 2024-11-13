#include <iostream>

#include "src/Camera.h"
#include "src/Engine.h"
#include "src/ImageComponent.h"
#include "src/Light.h"
#include "src/Mesh.h"
#include "src/MeshRenderer.h"
#include "src/Scene.h"
#include "src/SceneManager.h"
#include "src/Shader.h"
#include "src/ShaderManager.h"
#include "src/TextComponent.h"

void load()
{
    Shader* pShader{ new Shader("resources/vertex.glsl","resources/fragment.glsl") };
    ShaderManager::GetInstance().AddShader(pShader);
    Scene* pScene{ new Scene{"Main Scene"} };
    SceneManager::GetInstance().AddScene(pScene);
    GameObject* pGameObject{ new GameObject{"Cube"} };
    Mesh* pCubeMesh{ Mesh::CreateCube() };
    MeshRenderer* pMeshRenderer{ new MeshRenderer{pShader,pCubeMesh} };
    pGameObject->AddComponent(pMeshRenderer);
    pScene->AddGameObject(pGameObject);

    SceneManager::GetInstance().AddScene(new Scene{ "Test" });
    

    GameObject* pGameObject2{ new GameObject{"Light"} };
    Light* pLight{ new Light{} };
    pGameObject2->AddComponent(pLight);
	pScene->AddGameObject(pGameObject2);

    GameObject* pCameraObject{ new GameObject{"MainCamera"} };
    Camera* pCamera{ new Camera{} };
    pCameraObject->AddComponent(pCamera);
    pCamera->SetupCamera(60, 8 / 6.f);
    pScene->AddGameObject(pCameraObject);

    GameObject* pSphere{ new GameObject{"Sphere"} };
    MeshRenderer* pSphereRenderer{ new MeshRenderer{pShader,Mesh::CreateSphere()} };
    pSphere->AddComponent(pSphereRenderer);
    pScene->AddGameObject(pSphere);

    GameObject* pText(new GameObject{ "Text" });
    TextComponent* pTextComponent{ new TextComponent{"resources/Minecraft.ttf" , "Je te baise"} };
    pText->AddComponent(pTextComponent);
    pScene->AddGameObject(pText);

    GameObject* pImage{ new GameObject{"Image"} };
    ImageComponent* pImageComponent{ new ImageComponent{"resources/Test.png"} };
    pImage->AddComponent(pImageComponent);
    pScene->AddGameObject(pImage);
}

int main()
{
	Engine engine{};
    try
    {
        engine.Start(load);
    } catch(const std::exception& e)
    {
    	std::cerr << e.what() << '\n';
	}
    return 0;
}
