/*********************************************************************************
 * Representation of homogeneous coordinates
 *
 * File: coordinate.hpp
 * Author: Fernando Peña (NIA: 756012)
 * Author: Jose Daniel Subias Sarrato (NIA: 759533)
 * Date: 22/10/2020
 * Coms: Informática Gráfica, 2020-2021
 **********************************************************************************/

#include "geometry.hpp"

Vector3 abs(const Vector3 &v)
{
    return Vector3(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

float dot(const Vector3 &v1, const Vector3 &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 cross(const Vector3 &v1, const Vector3 &v2)
{
    return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

float mod(const Vector3 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Point3 abs(const Point3 &v)
{
    return Point3(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

float distance(const Point3 &p1, const Point3 &p2)
{
    mod(p1 - p2);
}
