/*********************************************************************************
 * Image
 *
 * File: BRDF.hpp
 * Author: Fernando Peña (NIA: 756012)
 * Author: Jose Daniel Subias Sarrato (NIA: 759533)
 * Date: 6/10/2020
 * Coms: Informática Gráfica, 2020-2021
 **********************************************************************************/

#include "BRDF.hpp"
#include "color.hpp"
#include "geometry.hpp"
#include "random.hpp"
#include "transform.hpp"
#include <cmath>
#include <iostream>
#include <memory>

Vector3 specular_reflection(const Vector3 &wi, const Vector3 &normal) {
    // wi rayo en coordenadas del mundo
    // Calcular el rayo de salida con la ley de la reflexión
    // Se devuelve en coordenadas del mundo
    float cosI = -dot(wi, normal);
    return wi + normal * 2.0f * cosI;
};

Vector3 diffuse_reflection(const Vector3 &wi, const Vector3 &normal, const Point3 intersection_point) {
    // wi rayo en coordenadas del mundo
    // Generar un rayo aleatorio dentro de la hemiesfera
    // Se devuelve en coordenadas del mundo

    // Hay que generar un espacio de coordenadas local al punto de intersección,
    // sabiendo la normal en ese punto
    // Generar una dirección aleatoria en la hemiesfera y cambiarla a coordendas del mundo

    float radious = modulus(normal);
    //unsigned int seed = rand() % 100;

    Vector3 Nt, x, y, z;

    if (std::fabs(normal.x) > std::fabs(normal.y))
        Nt = Vector3(normal.z, 0, -normal.x) / sqrtf(normal.x * normal.x + normal.z * normal.z);
    else
        Nt = Vector3(0, -normal.z, normal.y) / sqrtf(normal.y * normal.y + normal.z * normal.z);

    y = normalize(normal);
    z = cross(normal, Nt);
    z = normalize(z);
    x = cross(y, z);

    Vector3 out_dir = uniform_hemisphere_sample();

    return changeBasis(x, y, z, intersection_point)(out_dir);
};

Vector3 phong_reflection(const Vector3 &wr, const Vector3 &normal, const Point3 intersection_point, float alpha) {
    // wi rayo en coordenadas del mundo
    // Generar un rayo aleatorio dentro de la hemiesfera
    // Se devuelve en coordenadas del mundo

    // Hay que generar un espacio de coordenadas local al punto de intersección,
    // sabiendo la normal en ese punto
    // Generar una dirección aleatoria en la hemiesfera y cambiarla a coordendas del mundo

    float radious = modulus(normal);

    Vector3 Nt, x, y, z;

    if (std::fabs(wr.x) > std::fabs(wr.y))
        Nt = Vector3(wr.z, 0, -wr.x) / sqrtf(wr.x * wr.x + wr.z * wr.z);
    else
        Nt = Vector3(0, -wr.z, wr.y) / sqrtf(wr.y * wr.y + wr.z * wr.z);

    y = normalize(wr);
    z = cross(wr, Nt);
    z = normalize(z);
    x = cross(y, z);

    Vector3 out_dir = phong_uniform_hemisphere_sample(alpha);

    return changeBasis(x, y, z, intersection_point)(out_dir);
};

Vector3 reflection(const Vector3 &wi, const Vector3 &normal) {
    return wi + (normal * (2 * -dot(normal, wi)));
};

Vector3 refraction(const Vector3 &wi, const Vector3 &normal, float n1, float n2, bool &suscesfull) {

    float n = n1 / n2;
    float cosI = -dot(normal, wi);
    float sinT2 = n * n * (1.0f - cosI * cosI);
    if (sinT2 > 1.0f) {
        suscesfull = false;
        return Vector3();
    }

    float cosT = std::sqrt(1.0f - sinT2);
    suscesfull = true;
    return (wi * n) + (normal * (n * cosI - cosT));
};

float fresnel_ks(Vector3 const &wi, const Vector3 &normal, float n1, float n2) {

    float n = n1 / n2;
    float cosI = -dot(normal, wi);
    float sinT2 = n * n * (1.0f - cosI * cosI);
    if (sinT2 > 1.0f)
        return 1.0f;

    float cosT = sqrt(1.0f - sinT2);
    float r0rth = (n1 * cosI - n2 * cosT) / (n1 * cosI + n2 * cosT);
    float rPar = (n2 * cosI - n1 * cosT) / (n2 * cosI + n1 * cosT);

    return (r0rth * r0rth + rPar * rPar) / 2.0f;
};

void set_dielectric_properties(Material &material, const Vector3 direccion, const Vector3 normal) {

    float ks, kt;

    if (dot(direccion, normal) < 0.0f)
        ks = fresnel_ks(direccion, normal, AIR_N, material.n);
    else
        ks = fresnel_ks(direccion, -normal, material.n, AIR_N);

    kt = 1.0f - ks;
    ks = 0.95f * ks;
    kt = 0.95f * kt;

    material.set_max_ks(ks);
    material.set_max_kt(kt);

    material.set_ks(RGB(ks, ks, ks));
    material.set_kt(RGB(kt, kt, kt));
}
