cmake_minimum_required(VERSION 3.10)

# Crée la bibliothèque
add_library(Engine STATIC 
    src/Engine.cpp
    src/Shader.cpp
	src/Camera.cpp
    src/Component.h
	src/Component.cpp
    src/GameObject.h
	src/GameObject.cpp
	src/Transform.cpp
    src/Scene.cpp
	src/MathHelper.cpp
	src/Singleton.h 
    src/SceneManager.cpp 
    src/Mesh.h
    src/Mesh.cpp 
    src/MeshRenderer.h 
    src/MeshRenderer.cpp 
    src/Vertex.h 
    src/Light.h 
    src/Light.cpp 
    src/Renderer.h
    src/Renderer.cpp
    src/TimeEngine.h 
    src/ShaderManager.h
    src/ShaderManager.cpp 
    src/Font.h
    src/Font.cpp 
    src/TextComponent.h
    src/TextComponent.cpp 
    src/FontManager.h
    src/FontManager.cpp 
    src/ImageComponent.h
    src/ImageComponent.cpp 
    src/Image.h
    src/Image.cpp)

# Spécifie la version de C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch GLM
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        1.0.1
   GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glm)

# Fetch GLEW
FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
  GIT_TAG        glew-cmake-2.2.0  # Use the desired GLEW version
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glew)

# Fetch stb
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG        master  # Or specify a specific commit or tag
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(stb)

FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
    GIT_TAG     master
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(freetype)

set_target_properties(glfw PROPERTIES
        FOLDER "External Libraries" )

set_target_properties(glm PROPERTIES
        FOLDER "External Libraries" )

set_target_properties(libglew_shared PROPERTIES
        FOLDER "External Libraries" )

set_target_properties(libglew_static PROPERTIES
        FOLDER "External Libraries" )

set(RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_custom_command(TARGET Engine POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${RESOURCE_DIR}"
	$<TARGET_FILE_DIR:Engine>/resources
)

target_include_directories(Engine PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}
    ${OPENGL_INCLUDE_DIRS} 
    ${glew_SOURCE_DIR}/include 
    ${stb_SOURCE_DIR}
    ${glm_SOURCE_DIR}
    ${IMGUI_DIR} ${IMGUI_DIR}/backends
    ${freetype_SOURCE_DIR}/include
)

target_link_libraries(Engine PUBLIC 
    OpenGL::GL
    glfw 
    ${OPENGL_LIBRARIES} 
    libglew_static
    ImGui
    freetype
)