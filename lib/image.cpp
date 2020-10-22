/*********************************************************************************
 * Image
 *
 * File: image.cpp
 * Author: Fernando Peña (NIA: 756012)
 * Author: Jose Daniel Subias Sarrato (NIA: 759533)
 * Date: 6/10/2020
 * Coms: Informática Gráfica, 2020-2021
 **********************************************************************************/

#include "image.hpp"
#include "math.h"
#include <cfloat>

void Image::applyToneMappingOperator(const ToneMappingOperator &op) {
    std::cout << "Applying operator: " << op << " ..." << std::endl;
    for (RGB &pixel : v) {
        pixel = op(pixel);
    }
}

float max(const Image &img) {
    float maxValue = FLT_MIN;
    float aux = 0.0f;
    for (const RGB &pixel : img.v) {
        aux = max(pixel);
        if (aux > maxValue)
            maxValue = aux;
    }
    return maxValue;
};

float min(const Image &img) {
    float minValue = FLT_MAX;
    float aux = 0.0f;
    for (const RGB &pixel : img.v) {
        aux = min(pixel);
        if (aux < minValue)
            minValue = aux;
    }
    return aux;
};

float logAverageLuminance(const Image &img) {
    float avg_L_w = 0.0f;
    for (const RGB &pixel : img.v) {
        // std::cout << pixel.L() << std::endl;
        // std::cout << logf(1e-6f + pixel.L()) << std::endl;
        avg_L_w += logf(1e-6f + pixel.L());
    }
    std::cout << avg_L_w << std::endl;
    std::cout << expf(avg_L_w) << std::endl;

    avg_L_w = expf(avg_L_w / img.v.size());

    return avg_L_w;
}

void clamping(Image &img) {
    for (RGB &pixel : img.v) {
        if (pixel.r > 1.0f)
            pixel.r = 1.0f;

        if (pixel.g > 1.0f)
            pixel.g = 1.0f;
        if (pixel.b > 1.0f)
            pixel.b = 1.0f;
    }
};

void equalization(Image &img, float V) {
    for (RGB &pixel : img.v) {
        pixel = pixel / V;
    }
};

void equalizeAndClamp(Image &img, float V) {
    equalization(img, V);
    clamping(img);
};

void gammaCurve(Image &img, float V, float gamma) {
    equalization(img, V);
    for (RGB &pixel : img.v)
        pixel = pow(pixel, gamma);
};

void clampAndGammaCurve(Image &img, float V, float gamma) {
    gammaCurve(img, V, gamma);
    clamping(img);
};

void Reinhard(Image &img, float a) {
    // white luminity
    float L_w = 0.0f;

    // pixel luminity
    float L = 0.0f;

    // min wite in the image
    float min_L = max(img);

    // reinhard algorithm
    // http://erikreinhard.com/papers/s2002.pdf
    for (const RGB &pixel : img.v)
        L_w += logf(1e-6f + pixel.L());

    L_w = expf(L_w / (img.width * img.height));

    std::cout << "L_w" << L_w << std::endl;

    for (RGB &pixel : img.v) {
        L = (a / L_w) * pixel.r;
        pixel.r = (L * (1.0f + L / pow(min_L, 2))) / (1.0f + L);
        L = (a / L_w) * pixel.g;
        pixel.g = (L * (1.0f + L / pow(min_L, 2))) / (1.0f + L);
        L = (a / L_w) * pixel.b;
        pixel.b = (L * (1.0f + L / pow(min_L, 2))) / (1.0f + L);
    }
};

void Mantiuk(Image &img, float a, float s = 0.6f) {

    // white luminity
    float L_w = 0.0f;

    float L_tone_mapping = 0.0f;
    // pixel luminity
    float L = 0.0f;

    float L_pixel = 0.0f;

    // min wite in the image
    float min_L = max(img);

    // reinhard algorithm
    // https://www.researchgate.net/publication/317749456_A_comparative_review_of_tone-mapping_algorithms_for_high_dynamic_range_video
    for (const RGB &pixel : img.v)
        L_w += logf(1e-6f + pixel.L());

    L_w = expf(L_w / (img.width * img.height));

    for (RGB &pixel : img.v) {
        pixel = pixel * (1.0f / 4.0f);
        L_pixel = pixel.L();
        L = (a / L_w) * pixel.r;
        L_tone_mapping = (L * (1.0f + L / pow(min_L, 2))) / (1.0f + L);
        pixel.r = pow(pixel.r / L_pixel, s) * L_tone_mapping;

        L = (a / L_w) * pixel.g;
        L_tone_mapping = (L * (1.0f + L / pow(min_L, 2))) / (1.0f + L);
        pixel.g = pow(pixel.g / L_pixel, s) * L_tone_mapping;

        L = (a / L_w) * pixel.b;
        L_tone_mapping = (L * (1.0f + L / pow(min_L, 2))) / (1.0f + L);
        pixel.b = pow(pixel.b / L_pixel, s) * L_tone_mapping;
    }
};