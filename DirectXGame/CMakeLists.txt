project(DirectXGame)

################################################################################
# Source groups
################################################################################
set(App
    "main.cpp"
)
source_group("App" FILES ${App})

set(App__OldDemo
    "AppWindow.cpp"
    "AppWindow.h"
)
source_group("App\\OldDemo" FILES ${App__OldDemo})

set(App__SpaceShooterGame
    "SpaceShooterGame.cpp"
    "SpaceShooterGame.h"
)
source_group("App\\SpaceShooterGame" FILES ${App__SpaceShooterGame})

set(GameEngine__GraphicsEngine
    "GraphicsEngine.cpp"
    "GraphicsEngine.h"
    "Prerequisistes.h"
)
source_group("GameEngine\\GraphicsEngine" FILES ${GameEngine__GraphicsEngine})

set(GameEngine__GraphicsEngine__Material
    "Material.cpp"
    "Material.h"
)
source_group("GameEngine\\GraphicsEngine\\Material" FILES ${GameEngine__GraphicsEngine__Material})

set(GameEngine__GraphicsEngine__RenderSystem
    "RenderSystem.cpp"
    "RenderSystem.h"
)
source_group("GameEngine\\GraphicsEngine\\RenderSystem" FILES ${GameEngine__GraphicsEngine__RenderSystem})

set(GameEngine__GraphicsEngine__RenderSystem__ConstantBuffer
    "ConstantBuffer.cpp"
    "ConstantBuffer.h"
)
source_group("GameEngine\\GraphicsEngine\\RenderSystem\\ConstantBuffer" FILES ${GameEngine__GraphicsEngine__RenderSystem__ConstantBuffer})

set(GameEngine__GraphicsEngine__RenderSystem__DeviceContext
    "DeviceContext.cpp"
    "DeviceContext.h"
)
source_group("GameEngine\\GraphicsEngine\\RenderSystem\\DeviceContext" FILES ${GameEngine__GraphicsEngine__RenderSystem__DeviceContext})

set(GameEngine__GraphicsEngine__RenderSystem__IndexBuffer
    "IndexBuffer.cpp"
    "IndexBuffer.h"
)
source_group("GameEngine\\GraphicsEngine\\RenderSystem\\IndexBuffer" FILES ${GameEngine__GraphicsEngine__RenderSystem__IndexBuffer})

set(GameEngine__GraphicsEngine__RenderSystem__PixelShader
    "PixelShader.cpp"
    "PixelShader.h"
)
source_group("GameEngine\\GraphicsEngine\\RenderSystem\\PixelShader" FILES ${GameEngine__GraphicsEngine__RenderSystem__PixelShader})

set(GameEngine__GraphicsEngine__RenderSystem__SwapChain
    "SwapChain.cpp"
    "SwapChain.h"
)
source_group("GameEngine\\GraphicsEngine\\RenderSystem\\SwapChain" FILES ${GameEngine__GraphicsEngine__RenderSystem__SwapChain})

set(GameEngine__GraphicsEngine__RenderSystem__VertexBuffer
    "VertexBuffer.cpp"
    "VertexBuffer.h"
)
source_group("GameEngine\\GraphicsEngine\\RenderSystem\\VertexBuffer" FILES ${GameEngine__GraphicsEngine__RenderSystem__VertexBuffer})

set(GameEngine__GraphicsEngine__RenderSystem__VertexShader
    "VertexShader.cpp"
    "VertexShader.h"
)
source_group("GameEngine\\GraphicsEngine\\RenderSystem\\VertexShader" FILES ${GameEngine__GraphicsEngine__RenderSystem__VertexShader})

set(GameEngine__GraphicsEngine__ResourceManager
    "Resource.cpp"
    "Resource.h"
    "ResourceManager.cpp"
    "ResourceManager.h"
)
source_group("GameEngine\\GraphicsEngine\\ResourceManager" FILES ${GameEngine__GraphicsEngine__ResourceManager})

set(GameEngine__GraphicsEngine__ResourceManager__MehsManager
    "Mesh.cpp"
    "Mesh.h"
    "MeshManager.cpp"
    "MeshManager.h"
)
source_group("GameEngine\\GraphicsEngine\\ResourceManager\\MehsManager" FILES ${GameEngine__GraphicsEngine__ResourceManager__MehsManager})

set(GameEngine__GraphicsEngine__ResourceManager__TextureManager
    "Texture.cpp"
    "Texture.h"
    "TextureManager.cpp"
    "TextureManager.h"
)
source_group("GameEngine\\GraphicsEngine\\ResourceManager\\TextureManager" FILES ${GameEngine__GraphicsEngine__ResourceManager__TextureManager})

set(GameEngine__InputSystem
    "Gamepad.cpp"
    "Gamepad.h"
    "InputListner.h"
    "InputSystem.cpp"
    "InputSystem.h"
)
source_group("GameEngine\\InputSystem" FILES ${GameEngine__InputSystem})

set(GameEngine__Math
    "MathUtils.h"
    "Matrix4x4.h"
    "Point.h"
    "Vector2D.h"
    "Vector3D.h"
    "Vector4D.h"
    "VertexMesh.h"
)
source_group("GameEngine\\Math" FILES ${GameEngine__Math})

set(GameEngine__Utils
    "PingPongValue.cpp"
    "PingPongValue.h"
)
source_group("GameEngine\\Utils" FILES ${GameEngine__Utils})

set(GameEngine__WindowingSystem
    "Window.cpp"
    "Window.h"
)
source_group("GameEngine\\WindowingSystem" FILES ${GameEngine__WindowingSystem})

set(ALL_FILES
    ${App}
    ${App__OldDemo}
    ${App__SpaceShooterGame}
    ${GameEngine__GraphicsEngine}
    ${GameEngine__GraphicsEngine__Material}
    ${GameEngine__GraphicsEngine__RenderSystem}
    ${GameEngine__GraphicsEngine__RenderSystem__ConstantBuffer}
    ${GameEngine__GraphicsEngine__RenderSystem__DeviceContext}
    ${GameEngine__GraphicsEngine__RenderSystem__IndexBuffer}
    ${GameEngine__GraphicsEngine__RenderSystem__PixelShader}
    ${GameEngine__GraphicsEngine__RenderSystem__SwapChain}
    ${GameEngine__GraphicsEngine__RenderSystem__VertexBuffer}
    ${GameEngine__GraphicsEngine__RenderSystem__VertexShader}
    ${GameEngine__GraphicsEngine__ResourceManager}
    ${GameEngine__GraphicsEngine__ResourceManager__MehsManager}
    ${GameEngine__GraphicsEngine__ResourceManager__TextureManager}
    ${GameEngine__InputSystem}
    ${GameEngine__Math}
    ${GameEngine__Utils}
    ${GameEngine__WindowingSystem}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

#use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE DirectXGame)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
            CXX_STANDARD 17
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
            CXX_STANDARD 17
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE)

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
        "${PROJECT_SOURCE_DIR}/Libs/tinyobjloader/include"
        "${PROJECT_SOURCE_DIR}/Libs/DirectXTex/include"
        )

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /std:c++17
            >
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /std:c++17
            >
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES d3d11 d3dcompiler DirectXTexD)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")


target_link_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_SOURCE_DIR}/Libs/DirectXTex/bin;"
        )

