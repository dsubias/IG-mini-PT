/*********************************************************************************
 * Image
 *
 * File: image.hpp
 * Author: Fernando Peña (NIA: 756012)
 * Author: Jose Daniel Subias Sarrato (NIA: 759533)
 * Date: 6/10/2020
 * Coms: Informática Gráfica, 2020-2021
 **********************************************************************************/

#pragma once

#include "color.hpp"
#include "tone_mapping.hpp"

#include <fstream>
#include <math.h>
#include <string>
#include <vector>

enum FileFormat { PPM,
                  HDR };

struct Image {

    std::vector<RGB> v;
    int width;
    int height;

    Image(){};

    // Constructors
    Image(const std::vector<RGB> _v, int _width, int _height)
        : v(_v), width(_width), height(_height){};

    void applyToneMappingOperator(const ToneMappingOperator &op);
};

float maxLum(const Image &img);
float max(const Image &img);
float min(const Image &img);

float logAverageLuminance(const Image &img);

// Tone mapping functions
void clamping(Image &img);

void equalization(Image &img, float V);

void equalizeAndClamp(Image &img, float V);

void gammaCurve(Image &img, float V, float gamma);

void clampAndGammaCurve(Image &img, float V, float gamma);

void Reinhard(Image &img, float a);

void Mantiuk(Image &img, float a, float s);
