#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "../include/gcode-file-manager.h"

using namespace std;
namespace fs = std::filesystem;

string GCodeFileManager::extractDirectoryPath(string filePath) {

    fs::path pathObj(filePath);
    if (pathObj.empty()) {
        throw std::invalid_argument("Path is empty");
    }

    return pathObj.parent_path().string();
}

int GCodeFileManager::saveGcode (const vector<string>& gcode, const string &fileName) {
    ofstream outputFile(fileName);

    if (!outputFile.is_open()) {
        cerr << "Cannot open/write the file: " << fileName  << endl;
        return -1;
    }

    //Write vector lines in the file
    for (const auto& line : gcode) {
        outputFile << line << endl;
    }

    outputFile.close();

    if (outputFile.fail()) {
        std::cerr << "Error : cannot save the file: " << fileName << std::endl;
        return -1;
    }

    std::cout << "Fichier G-code enregistré avec succès : " << fileName << std::endl;
    return 0;
}
