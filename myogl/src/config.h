#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define MYOGL_MAJOR_VERSION 0
#define MYOGL_MINOR_VERSION 1

// if defined - used double buffering
#define MYOGL_DOUBLE_BUFFER
// if defined - created resizable application window
//#define MYOGL_RESIZABLE_WINDOW
// if defined - enable VSYNC
#define MYOGL_VSYNC

// if defined - used vbo for render text, else - display list
#define MYOGL_VBO_TEXT

// if defined - engine save log file in user directory
//#define ENABLE_LOG_FILE

#endif // CONFIG_H_INCLUDED
