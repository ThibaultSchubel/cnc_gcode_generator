#include "../include/drl_reader.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

vector<std::pair<int, float>> DrlReader::toolDiametersList;
vector<HoleObject> DrlReader::holes;

int DrlReader::readFile(const string &drlFilePath) {

    ifstream file(drlFilePath);
    string line; //drl line we are reading

    int currentToolNumber {0};


    if (!file.is_open()) {
        cerr << "Erreur : Impossible d'ouvrir le fichier DRL : " << drlFilePath << endl;
        return -1;
    }

    while (std::getline(file, line)) {
        // Ignore commentes
        if (line.empty() || line[0] == ';' || line[0] == '#' || line[0] == '%') {
            continue;
        }

        // Ignore unused GCode
        if (line[0] == 'g' || line[0] == 'G' || line[0] == 'm' || line[0] == 'M' || line[0] == 'f' || line[0] == 'F') {
            continue;
        }

        // get tools diameters
        if ((line[0] == 'T' && line[2] == 'C') || (line[0] == 't' && line[3] == 'c')) {
            addTool(line);
        }

        // switch tool number
        if (isValidToolString(line)) {
            currentToolNumber = extractToolNumber(line);
        }

        // save hole position
        if (isValidCoordinates(line)) {
            pushHole(line, currentToolNumber);
        }
    }

    file.close();
    reorderToolDiametersList();
    return 0;
}

    void DrlReader::addTool(const std::string &drlLine) {
        const size_t tPos = drlLine.find('T');
        const size_t cPos = drlLine.find('C');

        int toolNumber {0};
        float toolDiameter {0};

        if (tPos == std::string::npos || cPos == std::string::npos) {
            throw std::invalid_argument("Format invalide : 'T' ou 'C' introuvable dans la chaîne.");
        }

        // Extract tool number
        const std::string tStr = drlLine.substr(tPos + 1, cPos - tPos - 1);
        try {
            toolNumber = std::stoi(tStr);
        } catch (const std::exception& e) {
            throw std::invalid_argument("Cannot extract tool number : " + tStr);
        }

        // Extract tool diameter
        const std::string cStr = drlLine.substr(cPos + 1);
        try {
            toolDiameter = std::stof(cStr);
        } catch (const std::exception& e) {
            throw std::invalid_argument("Cannot extract tool diameter : " + cStr);
        }

        // Verify the diemeter dont exist
        for (auto &pair : toolDiametersList) {
            if (pair.second == toolDiameter) {
                return;
            }
        }
        // Add tool
        toolDiametersList.emplace_back(toolNumber, toolDiameter);
}

bool DrlReader::isValidToolString (const std::string& str) {
    // Begin with T or t
    if (str.empty() || (str[0] != 'T' && str[0] != 't')) {
        return false;
    }

    // Length: 2 ou 3 digits
    if (str.length() < 2 || str.length() > 3) {
        return false;
    }

    // if Letter after T is a number
    for (size_t i = 1; i < str.length(); ++i) {
        if (!isdigit(str[i])) {
            return false;
        }
    }

    return true;
}

int DrlReader::extractToolNumber(const std::string& str) {

    // Extract number
    string numberStr = str.substr(1);

    // Vérifie que la partie numérique ne contient que des chiffres
    for (char c : numberStr) {
        if (!isdigit(c)) {
            throw std::invalid_argument("Cannot extract 'T'/'t' value : " + str);
        }
    }

    // Convertit la chaîne en entier
    return stoi(numberStr);
}

bool DrlReader::isValidCoordinates (const string& str) {
    if (str.length() < 4) {
        return false;
    }

    bool hasX = false;
    bool hasY = false;
    size_t xPos = std::string::npos;
    size_t yPos = std::string::npos;

    //Is x and y in the string
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == 'X' || str[i] == 'x') {
            hasX = true;
            xPos = i;
        } else if (str[i] == 'Y' || str[i] == 'y') {
            hasY = true;
            yPos = i;
        }
    }

    if (!hasX || !hasY) {
        return false;
    }

    //Is X before Y
    if (xPos >= yPos) {
        return false;
    }

    return true;
}

void DrlReader::pushHole(const string& drlLine, const int tool)
{
    size_t xPos = std::string::npos;
    size_t yPos = std::string::npos;

    for (size_t i = 0; i < drlLine.length(); ++i) {
        if (drlLine[i] == 'X' || drlLine[i] == 'x') {
            xPos = i;
        } else if (drlLine[i] == 'Y' || drlLine[i] == 'y') {
            yPos = i;
        }
    }

    // Extrait la valeur de X
    std::string xValueStr = drlLine.substr(xPos + 1, yPos - xPos - 1);
    double xValue;
    try {
        xValue = std::stod(xValueStr);
    } catch (const std::exception& e) {
        throw std::invalid_argument("Erreur de conversion pour la valeur X : " + xValueStr);
    }

    // Extrait la valeur de Y
    std::string yValueStr = drlLine.substr(yPos + 1);
    double yValue;
    try {
        yValue = std::stod(yValueStr);
    } catch (const std::exception& e) {
        throw std::invalid_argument("Erreur de conversion pour la valeur Y : " + yValueStr);
    }



    HoleObject newHole = {xValue, yValue, tool};
    holes.push_back(newHole);
}

void DrlReader::reorderToolDiametersList () {
    ranges::sort(toolDiametersList,
                 [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
                     return a.second < b.second;
    });
}



