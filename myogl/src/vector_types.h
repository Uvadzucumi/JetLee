#ifndef VECTOR_TYPES_H_INCLUDED
#define VECTOR_TYPES_H_INCLUDED

#include <cmath>
#include <iostream>

//#define epsilon 0.0001

namespace MyOGL{

// VECTOR2 ///////////////////////////////////////////////////////////////////////////////////////////////

    template <class Type>
    class Vector2{
        public:
        union{
            struct{
                Type x,y;
            };
            struct{
                Type u,v;
            };
            struct{
                Type width,height;
            };
            Type data[2];
        };
        // constructors
        Vector2() : x(0), y(0) {};
        Vector2(Type x, Type y) : x(x), y(y) {};
        // utils
        void set(Type x_value, Type y_value){
            this->x=x_value; this->y=y_value;
        };
        float       length() const{
            return sqrtf(x*x + y*y);
        }
        float       distance(const Vector2& vec) const {     // distance between two vectors
            return sqrtf((vec.x-x)*(vec.x-x) + (vec.y-y)*(vec.y-y));
        }
        Vector2&    normalize(){                            //
            float xxyy = x*x + y*y;
            float invLength = 1.0f / sqrtf(xxyy);
            x *= invLength;
            y *= invLength;
            return *this;
        }
        float       dot(const Vector2& vec) const{          // dot product
            return (x*vec.x + y*vec.y);
        }
        bool        equal(const Vector2& vec, float epsilon) const{ // compare with epsilon
            return fabs(x - vec.x) < epsilon && fabs(y - vec.y) < epsilon;
        }

        // operators
        Vector2     operator-() const{
             return Vector2(-x, -y);
        }                      // unary operator (negate)
        Vector2     operator+(const Vector2& vec) const {   // add vector
            return Vector2(x+vec.x, y+vec.y);
        }
        Vector2     operator-(const Vector2& vec) const {   // subtract vector
            return Vector2(x-vec.x, y-vec.y);
        }
        Vector2&    operator+=(const Vector2& vec) {        // add vector and update this object
            x += vec.x; y += vec.y; return *this;
        }
        Vector2&    operator-=(const Vector2& vec) {         // subtract vector and update this object
            x -= vec.x; y -= vec.y; return *this;
        }
        Vector2     operator*(const float scale) const {     // scale
            return Vector2(x*scale, y*scale);
        }
        Vector2     operator*(const Vector2& vec) const {    // multiply each element
            return Vector2(x*vec.x, y*vec.y);
        }
        Vector2&    operator*=(const float scale) {          // scale and update this object
            x *= scale; y *= scale; return *this;
        }
        Vector2&    operator*=(const Vector2& vec) {         // multiply each element and update this object
            x *= vec.x; y *= vec.y; return *this;
        }
        Vector2     operator/(const float scale) const {     // inverse scale
            return Vector2(x/scale, y/scale);
        }
        Vector2&    operator/=(const float scale){          // scale and update this object
            x /= scale; y /= scale; return *this;
        }
        bool        operator==(const Vector2& vec) const {   // exact compare, no epsilon
            return (x == vec.x) && (y == vec.y);
        }
        bool        operator!=(const Vector2& vec) const {   // exact compare, no epsilon
            return (x != vec.x) || (y != vec.y);
        }
        bool        operator<(const Vector2& vec) const {    // comparison for sort
            if(x < vec.x) return true;
            if(x > vec.x) return false;
            if(y < vec.y) return true;
            if(y > vec.y) return false;
            return false;
        }
        Type        operator[](int index) const {            // subscript operator v[0], v[1]
            return (&x)[index];
        }
        Type&       operator[](int index) {                  // subscript operator v[0], v[1]
            return (&x)[index];
        }

        friend Vector2 operator*(const float a, const Vector2 vec){
            return Vector2(a*vec.x, a*vec.y);
        }
        friend std::ostream& operator<<(std::ostream& os, const Vector2& vec){
            os << "(" << vec.x << ", " << vec.y << ")";
            return os;
        }
    };

// VECTOR3 ///////////////////////////////////////////////////////////////////////////////////////////////

    template <class Type>
    struct Vector3{
        union{
            struct{
                Type x,y,z;
            };
            struct{
                Type r,g,b;
            };
            Type data[3];
        };
        // constructors
        Vector3(){this->x=0; this->y=0; this->z=0;}
        Vector3(Type x, Type y, Type z){ this->x=x; this->y=y; this->z=z;}

        void set(Type x_value, Type y_value, Type z_value){
            this->x=x_value; this->y=y_value; this->z=z_value;
        };
        float       length() const{                         //
            return sqrtf(x*x + y*y + z*z);
        }
        float       distance(const Vector3& vec) const{     // distance between two vectors
            return sqrtf((vec.x-x)*(vec.x-x) + (vec.y-y)*(vec.y-y) + (vec.z-z)*(vec.z-z));
        }
        Vector3&    normalize(){
            float xxyyzz = x*x + y*y + z*z;
            float invLength = 1.0f / sqrtf(xxyyzz);
            x *= invLength;
            y *= invLength;
            z *= invLength;
            return *this;                            //
        }
        Type       dot(const Vector3& vec) const{          // dot product
            return (x*vec.x + y*vec.y + z*vec.z);
        }
        Vector3     cross(const Vector3& vec) const {       // cross product
            return Vector3(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
        }
        bool        equal(const Vector3& vec, float epsilon) const{ // compare with epsilon
            return fabs(x - vec.x) < epsilon && fabs(y - vec.y) < epsilon && fabs(z - vec.z) < epsilon;
        }

        // operators
        Vector3     operator-() const{                      // unary operator (negate)
            return Vector3(-x, -y, -z);
        }
        Vector3     operator+(const Vector3& vec) const{    // add vector
            return Vector3(x+vec.x, y+vec.y, z+vec.z);
        }
        Vector3     operator-(const Vector3& vec) const{    // subtract vector
            return Vector3(x-vec.x, y-vec.y, z-vec.z);
        }
        Vector3&    operator+=(const Vector3& vec){         // add vector and update this object
            x += vec.x; y += vec.y; z += vec.z; return *this;
        }
        Vector3&    operator-=(const Vector3& vec){         // subtract vector and update this object
            x -= vec.x; y -= vec.y; z -= vec.z; return *this;
        }
        Vector3     operator*(const float scale) const{     // scale
            return Vector3(x*scale, y*scale, z*scale);
        }
        Vector3     operator*(const Vector3& vec) const{    // multiply each element
            return Vector3(x*vec.x, y*vec.y, z*vec.z);
        }
        Vector3&    operator*=(const float scale){          // scale and update this object
            x *= scale; y *= scale; z *= scale; return *this;
        }
        Vector3&    operator*=(const Vector3& vec){         // product each element and update this object
            x *= vec.x; y *= vec.y; z *= vec.z; return *this;
        }
        Vector3     operator/(const float scale) const{     // inverse scale
            return Vector3(x/scale, y/scale, z/scale);
        }
        Vector3&    operator/=(const float scale){          // scale and update this object
            x /= scale; y /= scale; z /= scale; return *this;
        }
        bool        operator==(const Vector3& vec) const{   // exact compare, no epsilon
            return (x == vec.x) && (y == vec.y) && (z == vec.z);
        }
        bool        operator!=(const Vector3& vec) const{   // exact compare, no epsilon
            return (x != vec.x) || (y != vec.y) || (z != vec.z);
        }
        bool        operator<(const Vector3& vec) const{    // comparison for sort
            if(x < vec.x) return true;
            if(x > vec.x) return false;
            if(y < vec.y) return true;
            if(y > vec.y) return false;
            if(z < vec.z) return true;
            if(z > vec.z) return false;
            return false;
        }
        Type       operator[](int index) const{            // subscript operator v[0], v[1]
            return (&x)[index];
        }
        Type&      operator[](int index){                  // subscript operator v[0], v[1]
            return (&x)[index];
        }

        friend Vector3 operator*(const float a, const Vector3 vec){
            return Vector3(a*vec.x, a*vec.y, a*vec.z);
        }
        friend std::ostream& operator<<(std::ostream& os, const Vector3& vec){
            os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
            return os;
        }

    };

// VECTOR4 ///////////////////////////////////////////////////////////////////////////////////////////////

    template <class Type>
    struct Vector4{
        union{
            struct{
                Type left,top,width,height;
            };
            struct{
                Type x,y,z,w;
            };
            struct{
                Type r,g,b,a;
            };
            Type data[4];
        };
        // utils
        void        set(Type x_value, Type y_value, Type z_value, Type w_value){
            this->x=x_value; this->y=y_value; this->z=z_value; this->w=w_value;
        };
        float       length() const{                         //
            return sqrtf(x*x + y*y + z*z + w*w);
        }
        float       distance(const Vector4& vec) const{     // distance between two vectors
            return sqrtf((vec.x-x)*(vec.x-x) + (vec.y-y)*(vec.y-y) + (vec.z-z)*(vec.z-z) + (vec.w-w)*(vec.w-w));
        }
        Vector4&    normalize(){                            //
            float xxyyzz = x*x + y*y + z*z;
            float invLength = 1.0f / sqrtf(xxyyzz);
            x *= invLength;
            y *= invLength;
            z *= invLength;
            return *this;
        }
        float       dot(const Vector4& vec) const{          // dot product
            return (x*vec.x + y*vec.y + z*vec.z + w*vec.w);
        }
        bool        equal(const Vector4& vec, float epsilon) const{ // compare with epsilon
            return fabs(x - vec.x) < epsilon && fabs(y - vec.y) < epsilon &&
                fabs(z - vec.z) < epsilon && fabs(w - vec.w) < epsilon;
        }

        // operators
        Vector4     operator-() const{                      // unary operator (negate)
            return Vector4(-x, -y, -z, -w);
        }
        Vector4     operator+(const Vector4& vec) const{    // add vector
            return Vector4(x+vec.x, y+vec.y, z+vec.z, w+vec.w);
        }
        Vector4     operator-(const Vector4& vec) const{    // subtract vector
            return Vector4(x-vec.x, y-vec.y, z-vec.z, w-vec.w);

        }
        Vector4&    operator+=(const Vector4& vec){         // add vector and update this object
            x += vec.x; y += vec.y; z += vec.z; w += vec.w; return *this;
        }
        Vector4&    operator-=(const Vector4& vec){         // subtract vector and update this object
            x -= vec.x; y -= vec.y; z -= vec.z; w -= vec.w; return *this;
        }
        Vector4     operator*(const float scale) const{     // scale
            return Vector4(x*scale, y*scale, z*scale, w*scale);
        }
        Vector4     operator*(const Vector4& vec) const{    // multiply each element
            return Vector4(x*vec.x, y*vec.y, z*vec.z, w*vec.w);
        }
        Vector4&    operator*=(const float scale){          // scale and update this object
            x *= scale; y *= scale; z *= scale; w *= scale; return *this;
        }
        Vector4&    operator*=(const Vector4& vec){         // multiply each element and update this object
            x *= vec.x; y *= vec.y; z *= vec.z; w *= vec.w; return *this;
        }
        Vector4     operator/(const float scale) const{     // inverse scale
            return Vector4(x/scale, y/scale, z/scale, w/scale);
        }
        Vector4&    operator/=(const float scale){          // scale and update this object
            x /= scale; y /= scale; z /= scale; w /= scale; return *this;
        }
        bool        operator==(const Vector4& vec) const{   // exact compare, no epsilon
            return (x == vec.x) && (y == vec.y) && (z == vec.z) && (w == vec.w);
        }
        bool        operator!=(const Vector4& vec) const{   // exact compare, no epsilon
            return (x != vec.x) || (y != vec.y) || (z != vec.z) || (w != vec.w);
        }
        bool        operator<(const Vector4& vec) const{    // comparison for sort
            if(x < vec.x) return true;
            if(x > vec.x) return false;
            if(y < vec.y) return true;
            if(y > vec.y) return false;
            if(z < vec.z) return true;
            if(z > vec.z) return false;
            if(w < vec.w) return true;
            if(w > vec.w) return false;
            return false;
        }
        Type       operator[](int index) const{            // subscript operator v[0], v[1]
            return (&x)[index];
        }
        Type&      operator[](int index){                  // subscript operator v[0], v[1]
            return (&x)[index];
        }

        friend Vector4 operator*(const float a, const Vector4 vec){
            return Vector4(a*vec.x, a*vec.y, a*vec.z, a*vec.w);
        }
        friend std::ostream& operator<<(std::ostream& os, const Vector4& vec){
            os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
            return os;
        }

    };

// VECTOR TYPES //////////////////////////////////////////////////////////////////////////////////////////

    typedef Vector2 <int> Vector2i;
    typedef Vector2 <unsigned int> Vector2ui;
    typedef Vector2 <float> Vector2f;
    typedef Vector3 <int> Vector3i;
    typedef Vector3 <float> Vector3f;
    typedef Vector4 <int> Vector4i;
    typedef Vector4 <unsigned int> Vector4ui;
    typedef Vector4 <float> Vector4f;

}


#endif // VECTORS_H_INCLUDED
