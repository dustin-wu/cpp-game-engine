#Sets minimum cmake version 
cmake_minimum_required(VERSION 3.14)

project(3DGameEngineDWU)

find_package(OpenGL REQUIRED)

#Tells compiler to use c++ 20
set(CMAKE_CXX_STANDARD 20)
#Makes c++ 20 required
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Setting path macros
set(GLFW_SOURCE_DIR "External/glfw-3.3.8")
set(GLEW_SOURCE_DIR "External/glew")
set(GLM_SOURCE_DIR "External/glm-master")
set(STB_SOURCE_DIR "External/stb")
set(FREETYPE_SOURCE_DIR "External/freetype")
set(TINYXML_SOURCE_DIR "External/tinyxml2")
set(RESOURCES_DIR "Resources")

#GLFW settings
set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")

#freetype settings
set(FT_DISABLE_ZLIB TRUE)
set(FT_DISABLE_BZIP2 TRUE)
set(FT_DISABLE_PNG TRUE)
set(FT_DISABLE_HARFBUZZ TRUE)
set(FT_DISABLE_BROTLI TRUE)


add_subdirectory(${GLFW_SOURCE_DIR})
add_subdirectory(${GLM_SOURCE_DIR})
add_subdirectory(${FREETYPE_SOURCE_DIR})

add_library(StaticGLEW STATIC ${GLEW_SOURCE_DIR}/src/glew.c)
add_library(TinyXML2 ${TINYXML_SOURCE_DIR}/tinyxml2.cpp)

include_directories(${GLFW_SOURCE_DIR}/include
                    ${GLFW_SOURCE_DIR}/deps
                    ${GLEW_SOURCE_DIR}/include
                    ${STB_SOURCE_DIR}
                    ${FREETYPE_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}
                    ${TINYXML_SOURCE_DIR}
                    )


add_executable(${PROJECT_NAME} MACOSX_BUNDLE
    main.cpp

    Engine/core.h
    Engine/core.cpp
    Engine/window.h
    Engine/window.cpp
    Engine/application.h
    Engine/application.cpp
    Engine/screen.h
    Engine/screen.cpp
    Engine/gameobject.h
    Engine/gameobject.cpp
    Engine/component.h
    Engine/component.cpp
    Engine/system.h
    Engine/system.cpp
    Engine/gameworld.h
    Engine/gameworld.cpp
    Engine/inputtracker.h
    Engine/inputtracker.cpp
    Engine/windowhandler.h
    Engine/windowhandler.cpp

    Engine/Components/nocomponent.h
    Engine/Components/nocomponent.cpp
    Engine/Components/drawablecomponent.h
    Engine/Components/drawablecomponent.cpp
    Engine/Components/transformcomponent.h
    Engine/Components/transformcomponent.cpp
    Engine/Components/cameracomponent.h
    Engine/Components/cameracomponent.cpp
    Engine/Components/playercomponent.h
    Engine/Components/playercomponent.cpp
    Engine/Components/collisioncomponent.h
    Engine/Components/collisioncomponent.cpp
    Engine/Components/rigidbodycomponent.h
    Engine/Components/rigidbodycomponent.cpp
    Engine/Components/CollisionComponents/cylindercollisioncomponent.h
    Engine/Components/CollisionComponents/cylindercollisioncomponent.cpp
    Engine/Components/CollisionComponents/ellipsoidcollisioncomponent.h
    Engine/Components/CollisionComponents/ellipsoidcollisioncomponent.cpp
    Engine/Components/CollisionComponents/trianglecollisioncomponent.h
    Engine/Components/CollisionComponents/trianglecollisioncomponent.cpp
    Engine/Components/navmeshcomponent.h
    Engine/Components/navmeshcomponent.cpp
    Engine/Components/aicomponent.h
    Engine/Components/aicomponent.cpp
    Engine/Components/BehaviorTree/btnode.h
    Engine/Components/BehaviorTree/btnode.cpp
    Engine/Components/BehaviorTree/selector.h
    Engine/Components/BehaviorTree/selector.cpp
    Engine/Components/BehaviorTree/sequence.h
    Engine/Components/BehaviorTree/sequence.cpp
    Engine/Components/BehaviorTree/blackboard.h
    Engine/Components/BehaviorTree/blackboard.cpp
    Engine/Components/BehaviorTree/action.h
    Engine/Components/BehaviorTree/action.cpp
    Engine/Components/BehaviorTree/condition.h
    Engine/Components/BehaviorTree/condition.cpp
    Engine/Components/drawableanimationcomponent.h
    Engine/Components/drawableanimationcomponent.cpp

    Engine/Systems/drawsystem.h
    Engine/Systems/drawsystem.cpp
    Engine/Systems/camerasystem.h
    Engine/Systems/camerasystem.cpp
    Engine/Systems/charactercontrollersystem.h
    Engine/Systems/charactercontrollersystem.cpp
    Engine/Systems/collisionsystem.h
    Engine/Systems/collisionsystem.cpp
    Engine/Systems/physicssystem.h
    Engine/Systems/physicssystem.cpp
    Engine/Systems/initializersystem.h
    Engine/Systems/initializersystem.cpp
    Engine/Systems/uniformgridsystem.h
    Engine/Systems/uniformgridsystem.cpp
    Engine/Systems/navmeshsystem.h
    Engine/Systems/navmeshsystem.cpp
    Engine/Systems/aisystem.h
    Engine/Systems/aisystem.cpp
    Engine/Systems/particlesystem.h
    Engine/Systems/particlesystem.cpp
    Engine/Systems/drawanimationsystem.h
    Engine/Systems/drawanimationsystem.cpp

    Engine/SpatialPartitioning/aabb.h
    Engine/SpatialPartitioning/aabb.cpp
    Engine/SpatialPartitioning/bvh.h
    Engine/SpatialPartitioning/bvh.cpp

    Game/Warmup/warmupapplication.h
    Game/Warmup/warmupapplication.cpp
    Game/Warmup/gamescreen.h
    Game/Warmup/gamescreen.cpp
    Game/Warmup/titlescreen.h
    Game/Warmup/titlescreen.cpp
    Game/Warmup/settingsscreen.h
    Game/Warmup/settingsscreen.cpp
    Game/Warmup/hintsscreen.h
    Game/Warmup/hintsscreen.cpp

    Game/PrairieKing3D/gamescreen.h
    Game/PrairieKing3D/gamescreen.cpp
    Game/PrairieKing3D/overlayscreen.h
    Game/PrairieKing3D/overlayscreen.cpp
    Game/PrairieKing3D/settingsscreen.h
    Game/PrairieKing3D/settingsscreen.cpp
    Game/PrairieKing3D/titlescreen.h
    Game/PrairieKing3D/titlescreen.cpp
    Game/PrairieKing3D/victoryscreen.h
    Game/PrairieKing3D/victoryscreen.cpp
    Game/PrairieKing3D/gameapplication.h
    Game/PrairieKing3D/gameapplication.cpp
    Game/PrairieKing3D/defeatscreen.h
    Game/PrairieKing3D/defeatscreen.cpp

    Game/PrairieKing3D/Components/guncomponent.h
    Game/PrairieKing3D/Components/guncomponent.cpp
    Game/PrairieKing3D/Components/bulletcomponent.h
    Game/PrairieKing3D/Components/bulletcomponent.cpp
    Game/PrairieKing3D/Components/enemycomponent.h
    Game/PrairieKing3D/Components/enemycomponent.cpp

    Game/PrairieKing3D/Systems/initializersystem.h
    Game/PrairieKing3D/Systems/initializersystem.cpp
    Game/PrairieKing3D/Systems/gunsystem.h
    Game/PrairieKing3D/Systems/gunsystem.cpp
    Game/PrairieKing3D/Systems/bulletsystem.h
    Game/PrairieKing3D/Systems/bulletsystem.cpp
    Game/PrairieKing3D/Systems/enemysystem.h
    Game/PrairieKing3D/Systems/enemysystem.cpp
    Game/PrairieKing3D/Systems/healthsystem.h
    Game/PrairieKing3D/Systems/healthsystem.cpp

    Game/Testing/gamescreen.h
    Game/Testing/gamescreen.cpp
    Game/Testing/gameapplication.h
    Game/Testing/gameapplication.cpp
    Game/Testing/Systems/gameinitializersystem.h
    Game/Testing/Systems/gameinitializersystem.cpp
    Game/Testing/Systems/specialcharactercontrollersystem.h
    Game/Testing/Systems/specialcharactercontrollersystem.cpp
    Game/Testing/Components/jumppadcomponent.h
    Game/Testing/Components/jumppadcomponent.cpp
    Game/Testing/Systems/jumppadsystem.h
    Game/Testing/Systems/jumppadsystem.cpp

    Game/ExtremeUnaliving/Systems/gameinitializersystem.h
    Game/ExtremeUnaliving/Systems/gameinitializersystem.cpp
    Game/ExtremeUnaliving/Systems/jumppadsystem.h
    Game/ExtremeUnaliving/Systems/jumppadsystem.cpp
    Game/ExtremeUnaliving/Systems/specialcharactercontrollersystem.h
    Game/ExtremeUnaliving/Systems/specialcharactercontrollersystem.cpp
    Game/ExtremeUnaliving/Systems/gunsystem.h
    Game/ExtremeUnaliving/Systems/gunsystem.cpp
    Game/ExtremeUnaliving/Systems/bulletsystem.h
    Game/ExtremeUnaliving/Systems/bulletsystem.cpp
    Game/ExtremeUnaliving/Systems/enemysystem.h
    Game/ExtremeUnaliving/Systems/enemysystem.cpp
    Game/ExtremeUnaliving/Systems/healthsystem.h
    Game/ExtremeUnaliving/Systems/healthsystem.cpp
    Game/ExtremeUnaliving/Systems/spawnersystem.h
    Game/ExtremeUnaliving/Systems/spawnersystem.cpp
    Game/ExtremeUnaliving/Systems/checkpointsystem.h
    Game/ExtremeUnaliving/Systems/checkpointsystem.cpp
    Game/ExtremeUnaliving/Systems/doorsystem.h
    Game/ExtremeUnaliving/Systems/doorsystem.cpp
    Game/ExtremeUnaliving/Systems/shopsystem.h
    Game/ExtremeUnaliving/Systems/shopsystem.cpp
    Game/ExtremeUnaliving/Systems/trophysystem.h
    Game/ExtremeUnaliving/Systems/trophysystem.cpp


    Game/ExtremeUnaliving/Components/jumppadcomponent.h
    Game/ExtremeUnaliving/Components/jumppadcomponent.cpp
    Game/ExtremeUnaliving/Components/guncomponent.h
    Game/ExtremeUnaliving/Components/guncomponent.cpp
    Game/ExtremeUnaliving/Components/bulletcomponent.h
    Game/ExtremeUnaliving/Components/bulletcomponent.cpp
    Game/ExtremeUnaliving/Components/enemycomponent.h
    Game/ExtremeUnaliving/Components/enemycomponent.cpp
    Game/ExtremeUnaliving/Components/spawnercomponent.h
    Game/ExtremeUnaliving/Components/spawnercomponent.cpp
    Game/ExtremeUnaliving/Components/checkpointcomponent.h
    Game/ExtremeUnaliving/Components/checkpointcomponent.cpp
    Game/ExtremeUnaliving/Components/doorcomponent.h
    Game/ExtremeUnaliving/Components/doorcomponent.cpp
    Game/ExtremeUnaliving/Components/shopcomponent.h
    Game/ExtremeUnaliving/Components/shopcomponent.cpp
    Game/ExtremeUnaliving/Components/trophycomponent.h
    Game/ExtremeUnaliving/Components/trophycomponent.cpp

    Game/ExtremeUnaliving/gameapplication.h
    Game/ExtremeUnaliving/gameapplication.cpp
    Game/ExtremeUnaliving/gamescreen.h
    Game/ExtremeUnaliving/gamescreen.cpp
    Game/ExtremeUnaliving/gameoverlayscreen.h
    Game/ExtremeUnaliving/gameoverlayscreen.cpp
    Game/ExtremeUnaliving/titlescreen.h
    Game/ExtremeUnaliving/titlescreen.cpp
    Game/ExtremeUnaliving/settingsscreen.h
    Game/ExtremeUnaliving/settingsscreen.cpp
    Game/ExtremeUnaliving/defeatscreen.h
    Game/ExtremeUnaliving/defeatscreen.cpp
    Game/ExtremeUnaliving/shopscreen.h
    Game/ExtremeUnaliving/shopscreen.cpp
    Game/ExtremeUnaliving/victoryscreen.h
    Game/ExtremeUnaliving/victoryscreen.cpp
    Game/ExtremeUnaliving/ShopHandler.h

    Game/ExtremeUnaliving/BehaviorTree/actionpursueplayer.h
    Game/ExtremeUnaliving/BehaviorTree/actionpursueplayer.cpp
    Game/ExtremeUnaliving/BehaviorTree/actionlungeatplayer.h
    Game/ExtremeUnaliving/BehaviorTree/actionlungeatplayer.cpp
    Game/ExtremeUnaliving/BehaviorTree/conditionplayerisclose.h
    Game/ExtremeUnaliving/BehaviorTree/conditionplayerisclose.cpp
    Game/ExtremeUnaliving/BehaviorTree/conditionhealthislow.h
    Game/ExtremeUnaliving/BehaviorTree/conditionhealthislow.cpp
    Game/ExtremeUnaliving/BehaviorTree/actionrunaway.h
    Game/ExtremeUnaliving/BehaviorTree/actionrunaway.cpp
    Game/ExtremeUnaliving/BehaviorTree/actionshootplayer.h
    Game/ExtremeUnaliving/BehaviorTree/actionshootplayer.cpp
    Game/ExtremeUnaliving/BehaviorTree/conditionplayerisfar.h
    Game/ExtremeUnaliving/BehaviorTree/conditionplayerisfar.cpp
    Game/ExtremeUnaliving/BehaviorTree/actionsmack.h
    Game/ExtremeUnaliving/BehaviorTree/actionsmack.cpp
    Game/ExtremeUnaliving/BehaviorTree/actionleap.h
    Game/ExtremeUnaliving/BehaviorTree/actionleap.cpp
    Game/ExtremeUnaliving/BehaviorTree/actionhit.h
    Game/ExtremeUnaliving/BehaviorTree/actionhit.cpp

    Graphics/graphics.h
    Graphics/graphics.cpp
    Graphics/global.h
    Graphics/debug.h
    Graphics/camera.h
    Graphics/camera.cpp
    Graphics/shaderloader.h
    Graphics/material.h
    Graphics/material.cpp
    Graphics/shapedata.h
    Graphics/shape.h
    Graphics/shape.cpp
    Graphics/modeltransform.h
    Graphics/modeltransform.cpp
    Graphics/light.h
    Graphics/light.cpp
    Graphics/font.h
    Graphics/font.cpp
    Graphics/textrenderer.h
    Graphics/textrenderer.cpp
    Graphics/GLWrappers/shader.h
    Graphics/GLWrappers/shader.cpp
    Graphics/GLWrappers/texture.h
    Graphics/GLWrappers/texture.cpp
    Graphics/GLWrappers/vbo.h
    Graphics/GLWrappers/vbo.cpp
    Graphics/GLWrappers/ibo.h
    Graphics/GLWrappers/ibo.cpp
    Graphics/GLWrappers/vao.h
    Graphics/GLWrappers/vao.cpp
    Graphics/GLWrappers/multibuffervao.h
    Graphics/GLWrappers/multibuffervao.cpp
    Graphics/tiny_obj_loader.h
    Graphics/GUI/guielement.h
    Graphics/GUI/guielement.cpp
    Graphics/GUI/guirenderer.h
    Graphics/GUI/guirenderer.cpp
    Graphics/GUI/guibutton.h
    Graphics/GUI/guibutton.cpp
    Graphics/Particles/particle.h
    Graphics/Particles/particle.cpp
    Graphics/Particles/particlerenderer.h
    Graphics/Particles/particlerenderer.cpp
    Graphics/Particles/particlemaster.h
    Graphics/Particles/particlemaster.cpp
    Graphics/Particles/particletexture.h
    Graphics/Particles/particletexture.cpp

    Graphics/Animation/animatedModel/joint.h
    Graphics/Animation/animatedModel/joint.cpp
    Graphics/Animation/animatedModel/animatedmodel.h
    Graphics/Animation/animatedModel/animatedmodel.cpp

    Graphics/Animation/animation/animation.h
    Graphics/Animation/animation/animation.cpp
    Graphics/Animation/animation/keyframe.h
    Graphics/Animation/animation/keyframe.cpp
    Graphics/Animation/animation/jointtransform.h
    Graphics/Animation/animation/jointtransform.cpp
    Graphics/Animation/animation/animator.h
    Graphics/Animation/animation/animator.cpp

    Graphics/Animation/loaders/animationdatastructures.h
    Graphics/Animation/loaders/skinloader.h
    Graphics/Animation/loaders/skinloader.cpp
    Graphics/Animation/loaders/skeletonloader.h
    Graphics/Animation/loaders/skeletonloader.cpp
    Graphics/Animation/loaders/geometryloader.h
    Graphics/Animation/loaders/geometryloader.cpp
    Graphics/Animation/loaders/colladaloader.h
    Graphics/Animation/loaders/colladaloader.cpp
    Graphics/Animation/loaders/animationloader.h
    Graphics/Animation/loaders/animationloader.cpp
    Graphics/Animation/loaders/animatedentityloader.h
    Graphics/Animation/loaders/animatedentityloader.cpp
    Graphics/Animation/loaders/entityanimationloader.h
    Graphics/Animation/loaders/entityanimationloader.cpp



)

target_link_libraries(${PROJECT_NAME} glfw StaticGLEW glm freetype TinyXML2 ${OPENGL_LIBRARIES})

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/Resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

# Set the destination directory within the .app bundle
set(MACOSX_BUNDLE_RESOURCES_DIR ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${PROJECT_NAME}.app/Contents/Resources)

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/Resources DESTINATION ${MACOSX_BUNDLE_RESOURCES_DIR} )

if (WIN32)
  add_compile_definitions(GLEW_STATIC)
  target_link_libraries(${PROJECT_NAME}
      opengl32
      glu32
  )
endif()

if (UNIX AND NOT APPLE)
  target_link_libraries(${PROJECT_NAME}
      GL
  )
endif()

