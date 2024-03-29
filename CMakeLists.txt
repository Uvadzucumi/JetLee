cmake_minimum_required(VERSION 3.3)
project(JetLee)

set(EnableSound ON)
set(EnableMusic ON)

#if (WIN32)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Os")
#else()
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Os")
#endif()

#SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s -static-libgcc -static-libstdc++" )

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
    myogl/src/3rd/picopng.cpp
    myogl/src/fileformats/bmpimage.cpp
    myogl/src/fileformats/bmpimage.h
    myogl/src/ui/base_ui_class.h
    myogl/src/ui/bitmap_font.cpp
    myogl/src/ui/bitmap_font.h
    myogl/src/ui/panel.h
    myogl/src/ui/text_box.cpp
    myogl/src/ui/text_box.h
    myogl/src/animation.cpp
    myogl/src/animation.h
    myogl/src/base_object.cpp
    myogl/src/base_object.h
    myogl/src/camera.cpp
    myogl/src/camera.h
    myogl/src/config.h
    myogl/src/config_file.cpp
    myogl/src/config_file.h
    myogl/src/entity.cpp
    myogl/src/entity.h
    myogl/src/events.cpp
    myogl/src/events.h
    myogl/src/frame_buffer.cpp
    myogl/src/frame_buffer.h
    myogl/src/light.h
    myogl/src/log.cpp
    myogl/src/log.h
    myogl/src/material.cpp
    myogl/src/material.h
    myogl/src/matrix_types.cpp
    myogl/src/matrix_types.h
    myogl/src/myogl.cpp
    myogl/src/myogl.h
    myogl/src/render.cpp
    myogl/src/render.h
    myogl/src/shaders.h
    myogl/src/singleton.h
    myogl/src/sprites.cpp
    myogl/src/sprites.h
    myogl/src/texture.cpp
    myogl/src/texture.h
    myogl/src/tileset.cpp
    myogl/src/tileset.h
    myogl/src/types.h
    myogl/src/vbo.cpp
    myogl/src/vbo.h
    myogl/src/vector_types.h
    src/ai.cpp
    src/ai.h
    src/camera2d.h
    src/game.h
#    src/gamesound.h
    src/graphics.cpp
    src/graphics.h
    src/hero.cpp
    src/hero.h
    src/locations.cpp
    src/locations.h
    src/main.cpp
#    src/sound/sound.h
#    src/sound/sound.cpp
    src/json/jsoncpp.cpp
        )

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
if(EnableSound)
    add_definitions(-DENABLE_SOUND=true)
    find_package(OpenAL REQUIRED)
    list(APPEND SOURCE_FILES
        src/gamesound.h
        src/sound/sound.h
        src/sound/sound.cpp
    )
    if(EnableMusic)
        add_definitions(-DENABLE_MUSIC=true)
    else()
        add_definitions(-DENABLE_MUSIC=false)
    endif()
else()
    add_definitions(-DENABLE_SOUND=false)
endif()

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INCLUDE_CURRENT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY .)
add_executable(JetLee ${SOURCE_FILES})
if(${EnableSound})
    if(EnableMusic)
        target_link_libraries(JetLee ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${OPENAL_LIBRARY} -lvorbisfile -logg)
    else()
        target_link_libraries(JetLee ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${OPENAL_LIBRARY})
    endif()
else()
    target_link_libraries(JetLee ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
endif()


#target_link_libraries(JetLee ${OPENGL_LIBRARIES} -lSDL2 ${OPENAL_LIBRARY} -lvorbisfile -logg)

