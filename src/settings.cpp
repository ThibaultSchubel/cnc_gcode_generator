#include <filesystem>
#include <fstream>
#include <iostream>
#include "../external/minIni.h"
#include "../include/settings.h"

using namespace std;
using namespace std::filesystem;

// Initialisation des variables statiques
float Settings::holeDepth = 3.0f;
float Settings::spindleLiftHeight = 5.0f;
int Settings::spindleRotationSpeed = 10000;
int Settings::spindleDrillingSpeed = 50;

char* Settings::homeDir = getenv("HOME");
string Settings::configDirectory = string(homeDir)+"/.cnc_gcode_generator";
string Settings::configFile = configDirectory+"/config.ini";

int Settings::init() {
    if (checkOrCreateDirectory() != 0) {return -1;}
    if (checkOrCreateConfigFile() != 0) {return -1;}
    return 0;
}

int Settings::checkOrCreateConfigFile () {

    if (exists(configFile)) {
        const char* configPath = configFile.c_str();

        holeDepth = static_cast<float>(ini_getf("spindle", "hole_depth", -3.0, configPath));
        spindleLiftHeight = static_cast<float>(ini_getf("spindle", "spindle_lift_height", 5.0, configPath));
        spindleRotationSpeed = ini_getl("spindle", "spindle_rotation_speed", 10000, configPath);
        spindleDrillingSpeed = ini_getl("spindle", "spindle_drilling_speed", 50, configPath);

    } else {
        std::cout << endl<< "Cannot find config file" << endl;

        path configPath(configFile);
        create_directories(configPath.parent_path());

        std::ofstream configOut(configFile);
        if (!configOut.is_open()) {
            std::cerr << "Error : Cannot create a config file " << configFile << endl;
            return -1;
        }

        configOut << "[spindle]\n";
        configOut << "hole_depth = " << to_string(holeDepth) <<"\n";
        configOut << "spindle_lift_height = "<< to_string(spindleLiftHeight) <<"\n";
        configOut << "spindle_rotation_speed = "<< to_string(spindleRotationSpeed) <<"\n";
        configOut << "spindle_drilling_speed = "<< to_string(spindleDrillingSpeed) <<"\n";
        configOut.close();

        cout << "New config file created: " << configFile << endl << endl;
    }

    // Show used values
    std::cout << endl<< "loaded configuration:" << endl;
    std::cout << "- Hole depth: " << holeDepth << "mm" <<  endl;
    std::cout << "- Spindle lift height: " << spindleLiftHeight << "mm" << endl;
    std::cout << "- Spindle rotation speed: " << spindleRotationSpeed << "rpm" << endl;
    std::cout << "- Spindle drilling speed: " << spindleDrillingSpeed << "mm/s" << endl << endl;
    return 0;

}

int Settings::checkOrCreateDirectory() {
    if (!exists(configDirectory)) {
        if (create_directory(configDirectory)) {
            cout << "Directory created: " << configDirectory << endl;
            return 0;
        } else {
            cerr << "Directory creation failed: " << configDirectory << endl;
            return -1;
        }
    }
    return 0;
}


