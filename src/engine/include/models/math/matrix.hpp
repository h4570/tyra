/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#ifndef _TYRA_MATRIX_
#define _TYRA_MATRIX_

#include "vector3.hpp"
#include "../screen_settings.hpp"
#include <string.h>

/** 4x4 Matrix class. */
class Matrix
{

public:
    float data[16] __attribute__((aligned(64)));

    // ----
    // Constructors/Destructors
    // ----

    /** Create empty matrix. */
    Matrix();

    /** 
     * Create identity/empty matrix.
     * @param t_do_identity Identity matrix if true, random matrix values otherwise
     */
    Matrix(const u8 &t_do_identity)
    {
        if (t_do_identity)
            identity();
    }

    /** Init matrix with given values. */
    Matrix(const float &m11, const float &m12, const float &m13, const float &m14,
           const float &m21, const float &m22, const float &m23, const float &m24,
           const float &m31, const float &m32, const float &m33, const float &m34,
           const float &m41, const float &m42, const float &m43, const float &m44);

    /** Init matrix with other matrix values. */
    Matrix(const Matrix &v) { memcpy(data, v.data, 16 * sizeof(float)); }

    // ----
    // Operators
    // ----

    Vector3 operator*(const Vector3 &v) const;

    Matrix operator*(const Matrix &v) const
    {
        Matrix result;
        cross(result.data, this->data, v.data);
        return result;
    }

    inline void operator*=(const Matrix &v) { cross(this->data, this->data, v.data); }

    float &operator[](const u8 &t_index) { return data[t_index]; }

    // ----
    // Functions
    // ----

    /** 
     * 1 0 0 0
     * 0 1 0 0
     * 0 0 1 0
     * 0 0 0 1
     */
    void identity();

    /** 
     * 1 0 0 0
     * 0 1 0 0
     * 0 0 1 0
     * 0 0 0 1
     */
    inline void unit() { identity(); };

    /** Translate matrix. */
    inline void translate(const Vector3 &t_val)
    {
        Matrix temp = Matrix(true);
        temp.translation(t_val);
        cross(this->data, temp.data, this->data);
    }

    /** Rotate matrix. */
    inline void rotate(const Vector3 &t_val)
    {
        Matrix temp = Matrix(true);

        temp.rotationZ(t_val.z);
        cross(this->data, temp.data, this->data);

        temp.identity();
        temp.rotationY(t_val.y);
        cross(this->data, temp.data, this->data);

        temp.identity();
        temp.rotationX(t_val.x);
        cross(this->data, temp.data, this->data);
    }

    /** Rotate matrix by X. */
    inline void rotateX(const float &t_radians)
    {
        Matrix temp = Matrix(true);
        temp.rotationX(t_radians);
        cross(this->data, temp.data, this->data);
    }

    /** Rotate matrix by Y. */
    inline void rotateY(const float &t_radians)
    {
        Matrix temp = Matrix(true);
        temp.rotationY(t_radians);
        cross(this->data, temp.data, this->data);
    }

    /** Rotate matrix by Z. */
    inline void rotateZ(const float &t_radians)
    {
        Matrix temp = Matrix(true);
        temp.rotationZ(t_radians);
        cross(this->data, temp.data, this->data);
    }

    /** Rotate matrix by angle. */
    void rotateByAngle(const float &t_angle, const Vector3 &t_axis)
    {
        Matrix temp;
        temp.rotationByAngle(t_angle, t_axis);
        cross(this->data, temp.data, this->data);
    }

    /** Scale matrix. */
    void scale(const Vector3 &t_val)
    {
        Matrix temp = Matrix(true);
        temp.setScale(t_val);
        cross(this->data, temp.data, this->data);
    }

    /** 
     * Create perspective projection matrix. 
     * 
     * Complies with OpenGL standard (gluPerspective). 
     * @see https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml
     * @param screen Screen settings
     */
    void setPerspective(const ScreenSettings &screen);

    /** 
     * Create a view matrix that transforms coordinates in 
     * such a way that the user looks at a target vector 
     * direction from a position vector. 
     * 
     * Complies with OpenGL standard (glLookAt).  
     * @see https://learnopengl.com/Getting-started/Camera
     */
    void lookAt(const Vector3 &t_position, const Vector3 &t_target);

    /** Print matrix (printf). */
    const void print() const;

private:
    void rotationX(const float &t_radians);
    void rotationY(const float &t_radians);
    void rotationZ(const float &t_radians);
    void rotationByAngle(const float &t_angle, const Vector3 &t_axis);
    void translation(const Vector3 &t_val);
    void setScale(const Vector3 &t_val);
    void setCamera(const float t_pos[4], const float t_vz[4], const float t_vy[4]);
    void cross(float res[16], const float a[16], const float b[16]) const;
};

#endif
