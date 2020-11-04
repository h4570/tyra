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
    Matrix operator*(Matrix &v);
    Matrix();
    ~Matrix();

    void lookAt(Vector3 &t_up, Vector3 &t_position, Vector3 &t_target);
    void identity();
    void makeZRotation(float t_radians);
    void translate(float t_x, float t_y, float t_z);
    void setPerspective(ScreenSettings &screen);
    const void print() const;
};

#endif
