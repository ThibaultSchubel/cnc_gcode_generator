#include "../include/gbr_reader.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

vector<Vertex> GbrReader::polygon;

int GbrReader::readFile(const string &gbrFilePath) {

    ifstream file(gbrFilePath);
    string line; //gbr line we are reading

    if (!file.is_open()) {
        cerr << "Error : Cannot read GBR file : " << gbrFilePath << endl;
        return -1;
    }

    while (std::getline(file, line)) {

        // If line begin with 'X' ou 'x'
        if (line.find('X') != std::string::npos || line.find('x') != std::string::npos) {
            const int pushVertexResp = pushVertex(line);
            if (pushVertexResp != 0) {
                return pushVertexResp;
            }
        }
    }

    file.close();
    return 0;

}

int GbrReader::pushVertex(const string &line) {
    size_t xPos = line.find_first_of("Xx");
    size_t yPos = line.find_first_of("Yy", xPos);
    size_t dPos = line.find_first_of("Dd", yPos);

    if (xPos == std::string::npos || yPos == std::string::npos || dPos == std::string::npos) {
        std::cerr << "Warning: Invalid line format: " << line << std::endl;
        return -1;
    }

    // X extraction
    std::string xValueStr = line.substr(xPos + 1, yPos - xPos - 1);
    double xValue;
    try {
        xValue = std::stod(xValueStr) / 1000000; // convertion to mm
    } catch (const std::exception& e) {
        std::cerr << "Warning: Cannot convert X value: " << xValueStr << std::endl;
        return -1;
    }

    // Y extraction
    std::string yValueStr = line.substr(yPos + 1, dPos - yPos - 1);
    double yValue;
    try {
        yValue = std::stod(yValueStr) / 1000000; // convertion to mm
    } catch (const std::exception& e) {
        std::cerr << "Warning: Cannot convert Y value: " << yValueStr << std::endl;
        return -1;
    }

    const Vertex newVertex = {xValue, yValue};
    polygon.push_back(newVertex);
    return 0;
}