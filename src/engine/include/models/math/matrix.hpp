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

    Matrix operator*(const Matrix &v) const
    {
        Matrix result;
        cross(result.data, this->data, v.data);
        return result;
    }

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

    /** Create translation matrix. */
    inline void translation(const Vector3 &t_val)
    {
        identity();
        translate(t_val);
    }

    /** Create X rotation matrix. */
    inline void rotationX(const float &t_radians)
    {
        identity();
        rotateX(t_radians);
    }

    /** Create Y rotation matrix. */
    inline void rotationY(const float &t_radians)
    {
        identity();
        rotateY(t_radians);
    }

    /** Create Z rotation matrix. */
    inline void rotationZ(const float &t_radians)
    {
        identity();
        rotateZ(t_radians);
    }

    /** Create angle rotation matrix. */
    void rotationByAngle(const float &t_angle, const Vector3 &t_axis);

    /** Create scale matrix. */
    void setScale(const Vector3 &t_val);

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
    void rotateX(const float &t_radians);
    void rotateY(const float &t_radians);
    void rotateZ(const float &t_radians);
    void translate(const Vector3 &t_val);
    void cross(float res[16], const float a[16], const float b[16]) const;
    void setCamera(const float t_pos[4], const float t_vz[4], const float t_vy[4]);
};

#endif
