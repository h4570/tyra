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

/** https://en.wikipedia.org/wiki/Matrix_(mathematics) */
class Matrix
{

public:
    float data[16];

    Matrix(float m11, float m12, float m13, float m14,
           float m21, float m22, float m23, float m24,
           float m31, float m32, float m33, float m34,
           float m41, float m42, float m43, float m44);
    Matrix(const Matrix &v);
    Matrix operator*(const Matrix &v);
    void operator*=(const Matrix &v);
    Matrix();
    ~Matrix();

    void lookAt(Vector3 &t_up, Vector3 &t_position, Vector3 &t_target);
    void identity();
    inline void unit() { identity(); };

    // Translation

    void translate(const Vector3 &t_val);

    inline void translation(const Vector3 &t_val)
    {
        identity();
        translate(t_val);
    }

    // Rotation

    void rotateX(const float &t_radians);
    void rotateY(const float &t_radians);
    void rotateZ(const float &t_radians);

    inline void rotationX(const float &t_radians)
    {
        identity();
        rotateX(t_radians);
    }

    inline void rotationY(const float &t_radians)
    {
        identity();
        rotateY(t_radians);
    }

    inline void rotationZ(const float &t_radians)
    {
        identity();
        rotateZ(t_radians);
    }

    inline void rotate(const Vector3 &t_val)
    {
        rotateX(t_val.x);
        rotateY(t_val.y);
        rotateX(t_val.z);
    }

    inline void rotation(const Vector3 &t_val)
    {
        identity();
        rotate(t_val);
    }

    //

    void setPerspective(ScreenSettings &screen);
    const void print() const;
};

#endif
