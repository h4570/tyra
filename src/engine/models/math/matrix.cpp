/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "../../include/models/math/matrix.hpp"

#include "../../include/utils/math.hpp"
#include <stdio.h>

// ----
// Constructors/Destructors
// ----

/** Create by specifying all points */
Matrix::Matrix(float m11, float m12, float m13, float m14,
               float m21, float m22, float m23, float m24,
               float m31, float m32, float m33, float m34,
               float m41, float m42, float m43, float m44)
{
    data[0] = m11;
    data[1] = m12;
    data[2] = m13;
    data[3] = m14;

    data[4] = m21;
    data[5] = m22;
    data[6] = m23;
    data[7] = m24;

    data[8] = m31;
    data[9] = m32;
    data[10] = m33;
    data[11] = m34;

    data[12] = m41;
    data[13] = m42;
    data[14] = m43;
    data[15] = m44;
}

/** Create with another matrix values */
Matrix::Matrix(const Matrix &v)
{
    data[0] = v.data[0];
    data[1] = v.data[1];
    data[2] = v.data[2];
    data[3] = v.data[3];

    data[4] = v.data[4];
    data[5] = v.data[5];
    data[6] = v.data[6];
    data[7] = v.data[7];

    data[8] = v.data[8];
    data[9] = v.data[9];
    data[10] = v.data[10];
    data[11] = v.data[11];

    data[12] = v.data[12];
    data[13] = v.data[13];
    data[14] = v.data[14];
    data[15] = v.data[15];
}

void Matrix::identity()
{
    asm volatile(
        "vsub.xyzw  vf4, vf0, vf0 \n\t"
        "vadd.w     vf4, vf4, vf0 \n\t"
        "vmr32.xyzw vf5, vf4      \n\t"
        "vmr32.xyzw vf6, vf5      \n\t"
        "vmr32.xyzw vf7, vf6      \n\t"
        "sqc2       vf4, 0x30(%0) \n\t"
        "sqc2       vf5, 0x20(%0) \n\t"
        "sqc2       vf6, 0x10(%0) \n\t"
        "sqc2       vf7, 0x0(%0)  \n\t"
        :
        : "r"(this->data));
}

void Matrix::translate(const Vector3 &t_val)
{
    this->data[12] += t_val.x; // 3,0
    this->data[13] += t_val.y; // 3,1
    this->data[14] += t_val.z; // 3,2
}

void Matrix::rotateX(const float &t_radians)
{
    Matrix temp = Matrix();
    temp.identity();
    float c = Math::cos(t_radians);
    float s = Math::sin(t_radians);
    this->data[5] = c;  // 1,1
    this->data[6] = s;  // 1,2
    this->data[9] = -s; // 2,1
    this->data[10] = c; // 2,2
}

void Matrix::rotateY(const float &t_radians)
{
    Matrix temp = Matrix();
    temp.identity();
    float c = Math::cos(t_radians);
    float s = Math::sin(t_radians);
    this->data[0] = c;  // 0,0
    this->data[2] = -s; // 0,3
    this->data[8] = s;  // 2,0
    this->data[10] = c; // 2,2
}

void Matrix::rotateZ(const float &t_radians)
{
    Matrix temp = Matrix();
    temp.identity();
    float c = Math::cos(t_radians);
    float s = Math::sin(t_radians);
    this->data[0] = c;  // 0,0
    this->data[1] = s;  // 0,1
    this->data[4] = -s; // 1,0
    this->data[5] = c;  // 1,1
}

Matrix Matrix::operator*(const Matrix &t)
{
    Matrix result;
    asm volatile(
        "lqc2         vf1, 0x00(%1) \n\t"
        "lqc2         vf2, 0x10(%1) \n\t"
        "lqc2         vf3, 0x20(%1) \n\t"
        "lqc2         vf4, 0x30(%1) \n\t"
        "lqc2         vf5, 0x00(%2) \n\t"
        "lqc2         vf6, 0x10(%2) \n\t"
        "lqc2         vf7, 0x20(%2) \n\t"
        "lqc2         vf8, 0x30(%2) \n\t"
        "vmulax.xyzw  ACC, vf5, vf1 \n\t"
        "vmadday.xyzw ACC, vf6, vf1 \n\t"
        "vmaddaz.xyzw ACC, vf7, vf1 \n\t"
        "vmaddw.xyzw  vf1, vf8, vf1 \n\t"
        "vmulax.xyzw  ACC, vf5, vf2 \n\t"
        "vmadday.xyzw ACC, vf6, vf2 \n\t"
        "vmaddaz.xyzw ACC, vf7, vf2 \n\t"
        "vmaddw.xyzw  vf2, vf8, vf2 \n\t"
        "vmulax.xyzw  ACC, vf5, vf3 \n\t"
        "vmadday.xyzw ACC, vf6, vf3 \n\t"
        "vmaddaz.xyzw ACC, vf7, vf3 \n\t"
        "vmaddw.xyzw  vf3, vf8, vf3 \n\t"
        "vmulax.xyzw  ACC, vf5, vf4 \n\t"
        "vmadday.xyzw ACC, vf6, vf4 \n\t"
        "vmaddaz.xyzw ACC, vf7, vf4 \n\t"
        "vmaddw.xyzw  vf4, vf8, vf4 \n\t"
        "sqc2         vf1, 0x00(%0) \n\t"
        "sqc2         vf2, 0x10(%0) \n\t"
        "sqc2         vf3, 0x20(%0) \n\t"
        "sqc2         vf4, 0x30(%0) \n\t"
        :
        : "r"(result.data), "r"(t.data), "r"(this->data)
        : "memory");
    return result;
}

void Matrix::operator*=(const Matrix &t)
{
    asm volatile(
        "lqc2         vf1, 0x00(%1) \n\t"
        "lqc2         vf2, 0x10(%1) \n\t"
        "lqc2         vf3, 0x20(%1) \n\t"
        "lqc2         vf4, 0x30(%1) \n\t"
        "lqc2         vf5, 0x00(%2) \n\t"
        "lqc2         vf6, 0x10(%2) \n\t"
        "lqc2         vf7, 0x20(%2) \n\t"
        "lqc2         vf8, 0x30(%2) \n\t"
        "vmulax.xyzw  ACC, vf5, vf1 \n\t"
        "vmadday.xyzw ACC, vf6, vf1 \n\t"
        "vmaddaz.xyzw ACC, vf7, vf1 \n\t"
        "vmaddw.xyzw  vf1, vf8, vf1 \n\t"
        "vmulax.xyzw  ACC, vf5, vf2 \n\t"
        "vmadday.xyzw ACC, vf6, vf2 \n\t"
        "vmaddaz.xyzw ACC, vf7, vf2 \n\t"
        "vmaddw.xyzw  vf2, vf8, vf2 \n\t"
        "vmulax.xyzw  ACC, vf5, vf3 \n\t"
        "vmadday.xyzw ACC, vf6, vf3 \n\t"
        "vmaddaz.xyzw ACC, vf7, vf3 \n\t"
        "vmaddw.xyzw  vf3, vf8, vf3 \n\t"
        "vmulax.xyzw  ACC, vf5, vf4 \n\t"
        "vmadday.xyzw ACC, vf6, vf4 \n\t"
        "vmaddaz.xyzw ACC, vf7, vf4 \n\t"
        "vmaddw.xyzw  vf4, vf8, vf4 \n\t"
        "sqc2         vf1, 0x00(%0) \n\t"
        "sqc2         vf2, 0x10(%0) \n\t"
        "sqc2         vf3, 0x20(%0) \n\t"
        "sqc2         vf4, 0x30(%0) \n\t"
        :
        : "r"(this->data), "r"(t.data), "r"(this->data)
        : "memory");
}

/** Create empty matrix */
Matrix::Matrix()
{
    data[0] = 0.0F;
    data[1] = 0.0F;
    data[2] = 0.0F;
    data[3] = 0.0F;

    data[4] = 0.0F;
    data[5] = 0.0F;
    data[6] = 0.0F;
    data[7] = 0.0F;

    data[8] = 0.0F;
    data[9] = 0.0F;
    data[10] = 0.0F;
    data[11] = 0.0F;

    data[12] = 0.0F;
    data[13] = 0.0F;
    data[14] = 0.0F;
    data[15] = 0.0F;
}

Matrix::~Matrix() {}

// ----
// Methods
// ----

/** Set up a perspective projection matrix
 * 
 * Clone of gluPerspective()
 * https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml
 * @param fov (FOV in radians)/2
 * @param aspect Aspect ratio
 * @param scrW Half of screen width
 * @param scrH Half of screen height
 * @param zNear Distance to near plane
 * @param zFar Distance to far plane
 * @param projScale Projection scale
 */
void Matrix::setPerspective(ScreenSettings &t_screen)
{
    float fovYdiv2 = Math::HALF_ANG2RAD * t_screen.fov;
    float cotFOV = 1.0F / (Math::sin(fovYdiv2) / Math::cos(fovYdiv2));
    float w = cotFOV * (t_screen.width / t_screen.projectionScale) / t_screen.aspectRatio;
    float h = cotFOV * (t_screen.height / t_screen.projectionScale);

    this->data[0] = w;
    this->data[1] = 0.0F;
    this->data[2] = 0.0F;
    this->data[3] = 0.0F;

    this->data[4] = 0.0F;
    this->data[5] = -h;
    this->data[6] = 0.0F;
    this->data[7] = 0.0F;

    this->data[8] = 0.0F;
    this->data[9] = 0.0F;
    this->data[10] =
        (t_screen.farPlaneDist + t_screen.nearPlaneDist) /
        (t_screen.farPlaneDist - t_screen.nearPlaneDist);
    this->data[11] = -1.0F;

    this->data[12] = 0.0F;
    this->data[13] = 0.0F;
    this->data[14] =
        (2.0F * t_screen.farPlaneDist * t_screen.nearPlaneDist) /
        (t_screen.farPlaneDist - t_screen.nearPlaneDist);
    this->data[15] = 0.0F;
}

/** Create a view matrix that transforms coordinates in
 * such a way that the user looks at a target vector
 * direction from a position vector.
 * 
 * Clone of OpenGL lookAt function
 * https://learnopengl.com/Getting-started/Camera
*/
void Matrix::lookAt(Vector3 &t_up, Vector3 &t_position, Vector3 &t_target)
{
    Vector3 camForward, camUp, camRight;

    camForward = t_position - t_target;
    camForward.normalize();
    camRight = t_up * camForward;
    camRight.normalize();
    camUp = camForward * camRight;

    data[0] = camRight.x;
    data[4] = camRight.y;
    data[8] = camRight.z;
    data[12] = -camRight.innerProduct(t_position);

    data[1] = camUp.x;
    data[5] = camUp.y;
    data[9] = camUp.z;
    data[13] = -camUp.innerProduct(t_position);

    data[2] = camForward.x;
    data[6] = camForward.y;
    data[10] = camForward.z;
    data[14] = -camForward.innerProduct(t_position);

    data[3] = 0;
    data[7] = 0;
    data[11] = 0;
    data[15] = 1;
}

const void Matrix::print() const
{
    printf("MATRIX(\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n)\n",
           data[0], data[1], data[2], data[3],
           data[4], data[5], data[6], data[7],
           data[8], data[9], data[10], data[11],
           data[12], data[13], data[14], data[15]);
}
