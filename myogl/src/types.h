#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED


#define PI							3.14159265358979323846f
// degrease to radian
#define DEGTORAD(a)					(((a) * PI) / 180.0f)
// maximum value
#define MAX(a,b)					((a < b) ? (b) : (a))
// minimum value
#define MIN(a,b)					((a < b) ? (a) : (b))

namespace MyOGL{

    // Constants
    enum MyEnum {
        MYOGL_TEXTURE_BORDER_REPEAT,
        MYOGL_TEXTURE_BORDER_CLAMP,
        MYOGL_TEXTURE_COMBINE_REPLACE,
        MYOGL_TEXTURE_COMBINE_ADD,
        MYOGL_TEXTURE_COMBINE_MODULATE,
    };


}

#endif // TYPES_H_INCLUDED
