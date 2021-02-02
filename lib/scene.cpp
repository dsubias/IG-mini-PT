/*********************************************************************************
 * Scene
 *
 * File: scene.cpp
 * Author: Fernando Peña (NIA: 756012)
 * Author: Jose Daniel Subias Sarrato (NIA: 759533)
 * Date: 6/10/2020
 * Coms: Informática Gráfica, 2020-2021
 **********************************************************************************/

#include <cfloat>

#include "random.hpp"
#include "scene.hpp"
#include "surface_interaction.hpp"

Scene::Scene() {
    background = RGB(0, 0, 0);
    ambient_ri = 1;
    total_power = 0;
}

Scene::Scene(RGB _background, float _ambient_ri) {
    background = _background;
    ambient_ri = _ambient_ri;
    total_power = 0;
}

void Scene::set_background(RGB _background) {
    background = _background;
}

RGB Scene::get_background() const {
    return background;
}

void Scene::add_shape(const std::shared_ptr<Shape> shape) {
    // Añadir shape a la BVH

    // Si es una malla de triángulos, añadirlos por separado
    std::shared_ptr<TriangleMesh> tm = std::dynamic_pointer_cast<TriangleMesh>(shape);

    if (!(tm == nullptr)) {
        for (Triangle &t : tm->faces) {
            shapes.push_back(std::make_shared<Triangle>(t));
        }
    } else {
        shapes.push_back(shape);
    }
}

// void Scene::add_shape(const std::shared_ptr<TriangleMesh> mesh) {
//     // Añadir todos los triángulos
//     // shapes.emplace_back(mesh->faces);
// }

int Scene::get_num_shapes() const {
    // Llamar también a las bounding box
    return shapes.size();
}

void Scene::add_light(const std::shared_ptr<LightSource> light) {
    lights.push_back(light);
    total_power += max(light->power);
}

int Scene::get_num_lights() const {
    return lights.size();
}

std::shared_ptr<LightSource> Scene::get_light(int idx) const {
    return lights[idx];
}

float Scene::sample_light(std::shared_ptr<LightSource> &light) const {
    // Recorrer todas las luces hasta que se llege a sumar el número aleatorio
    float rand = random_float(0, total_power);
    float acum = 0;
    for (std::shared_ptr<LightSource> l : lights) {
        if (acum > rand) {
            light = l;
            return max(l->power) / total_power;
        } else {
            acum += max(l->power);
        }
    }
    return 0;
}

bool Scene::first_intersection(const Ray &ray, SurfaceInteraction &si) const {
    return bvh.intersect(ray, si);
    // return shapes_first_intersection(shapes, ray, si);
}

void Scene::fix() {
    // Construir la BVH con las shapes actuales
    bvh = BVH(shapes, 0, shapes.size(), 10);
}