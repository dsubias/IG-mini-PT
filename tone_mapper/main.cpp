/*********************************************************************************
 * main
 *
 * File: main.cpp
 * Author: Fernando Peña (NIA: 756012)
 * Author: Jose Daniel Subias Sarrato (NIA: 759533)
 * Date: 6/10/2020
 * Coms: Informática Gráfica, 2020-2021
 **********************************************************************************/

#include "file.hpp"
#include "image.hpp"

#include <fstream>
#include <iostream>
#include <string>

void help() {
    std::cout << "Usage: tone_mapper [options] file.ppm\n\n"
                 "Options:\n"
                 "  -o <filename>   Specify output filename\n"
                 "  -clamp <value>  Clamp the image at <value>\n"
                 "  -h              Show this help screen and quit\n"
              << std::endl;
}

void invalidOption(const std::string &arg) {
    std::cerr << "Invalid option: " << arg << std::endl
              << std::endl;
    help();
}

void missingArgument(const std::string &arg) {
    std::cerr << "Missing value after " << arg << " argument" << std::endl
              << std::endl;
    help();
}

int main(int argc, char **argv) {
    std::string inFile;
    std::string outFile;
    int clamp = 1;

    // Process arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        std::cout << "argumento: " << arg << std::endl;
        if (arg[0] == '-') {
            if (arg == "-o") {
                if (i + 1 >= argc) {
                    missingArgument(arg);
                    return 1;
                }
                outFile = argv[i + 1];
                i++;
            } else if (arg == "-clamp") {
                if (i + 1 >= argc) {
                    missingArgument(arg);
                    return 1;
                }
                clamp = std::stof(argv[i + 1]);
                i++;
            } else if (arg == "-h") {
                help();
                return 0;
            } else {
                invalidOption(arg);
                return 1;
            }
        } else {
            // file name
            inFile = arg;
            break;
        }
    }

    if (!inFile.empty()) {
        if (outFile.empty()) {
            outFile = createOutFilename(inFile);
        }

        std::cout << "Antes de procesar" << std::endl;
        std::cout << "inFile: " << inFile << " outFile: " << outFile << std::endl;
        std::cout << "clamp: " << clamp << std::endl;

        Image img = readPPM(inFile);
        clamping(img);
        writePPM(img, outFile);

    } else {
        help();
        return 0;
    }

    return 0;
}
