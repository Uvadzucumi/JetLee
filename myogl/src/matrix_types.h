#ifndef MATRIX_TYPES_H_INCLUDED
#define MATRIX_TYPES_H_INCLUDED

// The elements of the matrix are stored as column major order.
// | 0 2 |    | 0 3 6 |    |  0  4  8 12 |
// | 1 3 |    | 1 4 7 |    |  1  5  9 13 |
//            | 2 5 8 |    |  2  6 10 14 |
//                         |  3  7 11 15 |

#include <iostream>
#include <iomanip>
#include "vector_types.h"

const float DEG2RAD = 3.141593f / 180;
const float EPSILON = 0.00001f;

namespace MyOGL{

///////////////////////////////////////////////////////////////////////////
// 2x2 matrix
///////////////////////////////////////////////////////////////////////////
template <class Type>
class Matrix2
{
public:
    // constructors
    Matrix2(){  // init with identity
        identity(); // initially identity matrix
    }
    Matrix2(const Type src[4]){
        set(src);
    }
    Matrix2(Type m0, Type m1, Type m2, Type m3){
        set(m0, m1, m2, m3);
    }

    void        set(const Type src[4]){
        m[0] = src[0];  m[1] = src[1];  m[2] = src[2];  m[3] = src[3];
    }
    void        set(Type m0, Type m1, Type m2, Type m3){
        m[0]= m0;  m[1] = m1;  m[2] = m2;  m[3]= m3;
    }
    void        setRow(int index, const Type row[2]){
        m[index] = row[0];  m[index + 2] = row[1];
    }
    void        setRow(int index, const Vector2<Type>& v){
        m[index] = v.x;  m[index + 2] = v.y;
    }
    void        setColumn(int index, const Type col[2]){
        m[index*2] = col[0];  m[index*2 + 1] = col[1];
    }
    void        setColumn(int index, const Vector2<Type>& v){
        m[index*2] = v.x;  m[index*2 + 1] = v.y;
    }

    const Type* get() const{
        return m;
    }
    Type      getDeterminant(){                         // return the determinant of 2x2 matrix
        return m[0] * m[3] - m[1] * m[2];
    }

    Matrix2&    identity(){
        m[0] = m[3] = 1.0f;
        m[1] = m[2] = 0.0f;
        return *this;
    }
    Matrix2&    transpose(){                            // transpose itself and return reference
        std::swap(m[1],  m[2]);
        return *this;
    }
    Matrix2&    invert(){                               // inverse of 2x2 matrix
        float determinant = getDeterminant();           // If cannot find inverse, set identity matrix
        if(fabs(determinant) <= EPSILON)
        {
            return identity();
        }

        Type tmp = m[0];   // copy the first element
        float invDeterminant = 1.0f / determinant;
        m[0] =  invDeterminant * m[3];
        m[1] = -invDeterminant * m[1];
        m[2] = -invDeterminant * m[2];
        m[3] =  invDeterminant * tmp;

        return *this;
    }

    // operators
    Matrix2     operator+(const Matrix2& rhs) const{    // add rhs
        return Matrix2(m[0]+rhs[0], m[1]+rhs[1], m[2]+rhs[2], m[3]+rhs[3]);
    }
    Matrix2     operator-(const Matrix2& rhs) const{    // subtract rhs
        return Matrix2(m[0]-rhs[0], m[1]-rhs[1], m[2]-rhs[2], m[3]-rhs[3]);
    }
    Matrix2&    operator+=(const Matrix2& rhs){         // add rhs and update this object
        m[0] += rhs[0];  m[1] += rhs[1];  m[2] += rhs[2];  m[3] += rhs[3];
        return *this;
    }
    Matrix2&    operator-=(const Matrix2& rhs){         // subtract rhs and update this object
        m[0] -= rhs[0];  m[1] -= rhs[1];  m[2] -= rhs[2];  m[3] -= rhs[3];
        return *this;
    }
    Vector2<Type>     operator*(const Vector2<Type>& rhs) const{    // multiplication: v' = M * v
        return Vector2<Type>(m[0]*rhs.x + m[2]*rhs.y,  m[1]*rhs.x + m[3]*rhs.y);
    }
    Matrix2     operator*(const Matrix2& rhs) const{    // multiplication: M3 = M1 * M2
        return Matrix2(m[0]*rhs[0] + m[2]*rhs[1],  m[1]*rhs[0] + m[3]*rhs[1],
               m[0]*rhs[2] + m[2]*rhs[3],  m[1]*rhs[2] + m[3]*rhs[3]);
    }
    Matrix2&    operator*=(const Matrix2& rhs){         // multiplication: M1' = M1 * M2
        *this = *this * rhs;
        return *this;
    }
    bool        operator==(const Matrix2& rhs) const{   // exact compare, no epsilon
        return (m[0] == rhs[0]) && (m[1] == rhs[1]) && (m[2] == rhs[2]) && (m[3] == rhs[3]);
    }
    bool        operator!=(const Matrix2& rhs) const{   // exact compare, no epsilon
        return (m[0] != rhs[0]) || (m[1] != rhs[1]) || (m[2] != rhs[2]) || (m[3] != rhs[3]);
    }
    Type       operator[](int index) const{            // subscript operator v[0], v[1]
        return m[index];
    }
    Type&      operator[](int index){                  // subscript operator v[0], v[1]
        return m[index];
    }

    friend Matrix2 operator-(const Matrix2& m){                     // unary operator (-)
        return Matrix2(-m[0], -m[1], -m[2], -m[3]);
    }
    friend Matrix2 operator*(Type scalar, const Matrix2& m){       // pre-multiplication
        return Matrix2(scalar*m[0], scalar*m[1], scalar*m[2], scalar*m[3]);
    }
    friend Vector2<Type> operator*(const Vector2<Type>& v, const Matrix2& m){ // pre-multiplication
        return Vector2<Type>(v.x*m[0] + v.y*m[1],  v.x*m[2] + v.y*m[3]);
    }
    friend std::ostream& operator<<(std::ostream& os, const Matrix2& m){
        os << std::fixed << std::setprecision(5);
        os << "[" << std::setw(10) << m[0] << " " << std::setw(10) << m[2] << "]\n";
        os << "[" << std::setw(10) << m[1] << " " << std::setw(10) << m[3] << "]\n";
        os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
        return os;
    }

protected:

private:
    Type m[4];

};

// MATRIX 3x3 ////////////////////////////////////////////////////////////////////////////////////////////
template <class Type>
class Matrix3
{
public:
    // constructors
    Matrix3(){  // init with identity
        identity();
    }
    Matrix3(const Type src[9]){
        set(src);
    }
    Matrix3(Type m0, Type m1, Type m2,           // 1st column
            Type m3, Type m4, Type m5,           // 2nd column
            Type m6, Type m7, Type m8){          // 3rd column
        set(m0, m1, m2,  m3, m4, m5,  m6, m7, m8);
    }

    void        set(const Type src[9]){
        m[0] = src[0];  m[1] = src[1];  m[2] = src[2];
        m[3] = src[3];  m[4] = src[4];  m[5] = src[5];
        m[6] = src[6];  m[7] = src[7];  m[8] = src[8];
    }
    void        set(Type m0, Type m1, Type m2,   // 1st column
                    Type m3, Type m4, Type m5,   // 2nd column
                    Type m6, Type m7, Type m8){  // 3rd column
        m[0] = m0;  m[1] = m1;  m[2] = m2;
        m[3] = m3;  m[4] = m4;  m[5] = m5;
        m[6] = m6;  m[7] = m7;  m[8] = m8;
    }
    void        setRow(int index, const Type row[3]){
        m[index] = row[0];  m[index + 3] = row[1];  m[index + 6] = row[2];
    }
    void        setRow(int index, const Vector3<Type>& v){
        m[index] = v.x;  m[index + 3] = v.y;  m[index + 6] = v.z;
    }

    void        setColumn(int index, const Type col[3]){
        m[index*3] = col[0];  m[index*3 + 1] = col[1];  m[index*3 + 2] = col[2];
    }
    void        setColumn(int index, const Vector3<Type>& v){
        m[index*3] = v.x;  m[index*3 + 1] = v.y;  m[index*3 + 2] = v.z;
    }

    const Type* get() const{
        return m;
    }
    Type       getDeterminant(){
        return m[0] * (m[4] * m[8] - m[5] * m[7]) -
               m[1] * (m[3] * m[8] - m[5] * m[6]) +
               m[2] * (m[3] * m[7] - m[4] * m[6]);
    }

    Matrix3&    identity(){
        m[0] = m[4] = m[8] = (Type)1;
        m[1] = m[2] = m[3] = m[5] = m[6] = m[7] = (Type)0;
        return *this;
    }
    Matrix3&    transpose(){;                            // transpose itself and return reference
        std::swap(m[1],  m[3]);
        std::swap(m[2],  m[6]);
        std::swap(m[5],  m[7]);
        return *this;
    }
    Matrix3&    invert(){
        float determinant, invDeterminant;
        float tmp[9];

        tmp[0] = m[4] * m[8] - m[5] * m[7];
        tmp[1] = m[2] * m[7] - m[1] * m[8];
        tmp[2] = m[1] * m[5] - m[2] * m[4];
        tmp[3] = m[5] * m[6] - m[3] * m[8];
        tmp[4] = m[0] * m[8] - m[2] * m[6];
        tmp[5] = m[2] * m[3] - m[0] * m[5];
        tmp[6] = m[3] * m[7] - m[4] * m[6];
        tmp[7] = m[1] * m[6] - m[0] * m[7];
        tmp[8] = m[0] * m[4] - m[1] * m[3];

    // check determinant if it is 0
        determinant = m[0] * tmp[0] + m[1] * tmp[3] + m[2] * tmp[6];
        if(fabs(determinant) <= EPSILON){
            return identity(); // cannot inverse, make it identity matrix
        }

    // divide by the determinant
        invDeterminant = 1.0f / determinant;
        m[0] = invDeterminant * tmp[0];
        m[1] = invDeterminant * tmp[1];
        m[2] = invDeterminant * tmp[2];
        m[3] = invDeterminant * tmp[3];
        m[4] = invDeterminant * tmp[4];
        m[5] = invDeterminant * tmp[5];
        m[6] = invDeterminant * tmp[6];
        m[7] = invDeterminant * tmp[7];
        m[8] = invDeterminant * tmp[8];

        return *this;

    }

    // operators
    Matrix3     operator+(const Matrix3& rhs) const{    // add rhs
        return Matrix3(m[0]+rhs[0], m[1]+rhs[1], m[2]+rhs[2],
                   m[3]+rhs[3], m[4]+rhs[4], m[5]+rhs[5],
                   m[6]+rhs[6], m[7]+rhs[7], m[8]+rhs[8]);
    }
    Matrix3     operator-(const Matrix3& rhs) const{    // subtract rhs
        return Matrix3(m[0]-rhs[0], m[1]-rhs[1], m[2]-rhs[2],
                   m[3]-rhs[3], m[4]-rhs[4], m[5]-rhs[5],
                   m[6]-rhs[6], m[7]-rhs[7], m[8]-rhs[8]);
    }
    Matrix3&    operator+=(const Matrix3& rhs){         // add rhs and update this object
        m[0] += rhs[0];  m[1] += rhs[1];  m[2] += rhs[2];
        m[3] += rhs[3];  m[4] += rhs[4];  m[5] += rhs[5];
        m[6] += rhs[6];  m[7] += rhs[7];  m[8] += rhs[8];
        return *this;
    }
    Matrix3&    operator-=(const Matrix3& rhs){         // subtract rhs and update this object
        m[0] -= rhs[0];  m[1] -= rhs[1];  m[2] -= rhs[2];
        m[3] -= rhs[3];  m[4] -= rhs[4];  m[5] -= rhs[5];
        m[6] -= rhs[6];  m[7] -= rhs[7];  m[8] -= rhs[8];
        return *this;
    }
    Vector3<Type>     operator*(const Vector3<Type>& rhs) const{    // multiplication: v' = M * v
        return Vector3<Type>(m[0]*rhs.x + m[3]*rhs.y + m[6]*rhs.z,
                             m[1]*rhs.x + m[4]*rhs.y + m[7]*rhs.z,
                             m[2]*rhs.x + m[5]*rhs.y + m[8]*rhs.z);
    }
    Matrix3     operator*(const Matrix3& rhs) const{    // multiplication: M3 = M1 * M2
        return Matrix3(m[0]*rhs[0] + m[3]*rhs[1] + m[6]*rhs[2],  m[1]*rhs[0] + m[4]*rhs[1] + m[7]*rhs[2],  m[2]*rhs[0] + m[5]*rhs[1] + m[8]*rhs[2],
                       m[0]*rhs[3] + m[3]*rhs[4] + m[6]*rhs[5],  m[1]*rhs[3] + m[4]*rhs[4] + m[7]*rhs[5],  m[2]*rhs[3] + m[5]*rhs[4] + m[8]*rhs[5],
                       m[0]*rhs[6] + m[3]*rhs[7] + m[6]*rhs[8],  m[1]*rhs[6] + m[4]*rhs[7] + m[7]*rhs[8],  m[2]*rhs[6] + m[5]*rhs[7] + m[8]*rhs[8]);
    }
    Matrix3&    operator*=(const Matrix3& rhs){         // multiplication: M1' = M1 * M2
        *this = *this * rhs;
        return *this;
    }
    bool        operator==(const Matrix3& rhs) const{   // exact compare, no epsilon
        return (m[0] == rhs[0]) && (m[1] == rhs[1]) && (m[2] == rhs[2]) &&
               (m[3] == rhs[3]) && (m[4] == rhs[4]) && (m[5] == rhs[5]) &&
               (m[6] == rhs[6]) && (m[7] == rhs[7]) && (m[8] == rhs[8]);
    }
    bool       operator!=(const Matrix3& rhs) const{   // exact compare, no epsilon
        return (m[0] != rhs[0]) || (m[1] != rhs[1]) || (m[2] != rhs[2]) ||
               (m[3] != rhs[3]) || (m[4] != rhs[4]) || (m[5] != rhs[5]) ||
               (m[6] != rhs[6]) || (m[7] != rhs[7]) || (m[8] != rhs[8]);
    }
    Type       operator[](int index) const{            // subscript operator v[0], v[1]
        return m[index];
    }
    Type&      operator[](int index){                  // subscript operator v[0], v[1]
        return m[index];
    }

    friend Matrix3 operator-(const Matrix3& rhs){                     // unary operator (-)
        return Matrix3(-rhs[0], -rhs[1], -rhs[2], -rhs[3], -rhs[4], -rhs[5], -rhs[6], -rhs[7], -rhs[8]);
    }
    friend Matrix3 operator*(float scalar, const Matrix3& rhs){       // pre-multiplication
        return Matrix3(scalar*rhs[0], scalar*rhs[1], scalar*rhs[2], scalar*rhs[3], scalar*rhs[4], scalar*rhs[5], scalar*rhs[6], scalar*rhs[7], scalar*rhs[8]);
    }
    friend Vector3<Type> operator*(const Vector3<Type>& v, const Matrix3<Type>& m){ // pre-multiplication
        return Vector3<Type>(v.x*m[0] + v.y*m[1] + v.z*m[2],  v.x*m[3] + v.y*m[4] + v.z*m[5],  v.x*m[6] + v.y*m[7] + v.z*m[8]);
    }
    friend std::ostream& operator<<(std::ostream& os, const Matrix3& m){
        os << std::fixed << std::setprecision(5);
        os << "[" << std::setw(10) << m[0] << " " << std::setw(10) << m[3] << " " << std::setw(10) << m[6] << "]\n"
           << "[" << std::setw(10) << m[1] << " " << std::setw(10) << m[4] << " " << std::setw(10) << m[7] << "]\n"
           << "[" << std::setw(10) << m[2] << " " << std::setw(10) << m[5] << " " << std::setw(10) << m[8] << "]\n";
        os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
        return os;
    }

protected:

private:
    Type m[9];
};

// MATRIX TYPES //////////////////////////////////////////////////////////////////////////////////////////
    typedef Matrix2 <int> Matrix2i;
    typedef Matrix2 <unsigned int> Matrix2ui;
    typedef Matrix2 <float> Matrix2f;

    typedef Matrix3 <int> Matrix3i;
    typedef Matrix3 <unsigned int> Matrix3ui;
    typedef Matrix3 <float> Matrix3f;
}

#endif // MATRIX_TYPES_H_INCLUDED
