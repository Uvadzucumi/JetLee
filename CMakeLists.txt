cmake_minimum_required(VERSION 3.3)
project(JetLee)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

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
    #src/jsmn/example/jsondump.c
    #src/jsmn/example/simple.c
    src/jsmn/jsmn.c
    src/jsmn/jsmn.h
    #src/jsmn/jsmn_test.c
    src/ai.cpp
    src/ai.h
    src/camera2d.h
    src/game.h
    src/gamesound.h
    src/graphics.cpp
    src/graphics.h
    src/hero.cpp
    src/hero.h
    src/locations.cpp
    src/locations.h
    src/main.cpp
    src/sound/sound.h src/sound/sound.cpp)

find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
#find_package(SDL REQUIRED)
add_executable(JetLee ${SOURCE_FILES})
#target_link_libraries(JetLee ${OPENGL_LIBRARIES} ${OPENAL_LIBRARY} -lSDL2)
target_link_libraries(JetLee ${OPENGL_LIBRARIES} ${OPENAL_LIBRARY} -lSDL2 -logg -lvorbisfile)