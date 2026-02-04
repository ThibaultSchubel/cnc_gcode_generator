#include <filesystem>
#include <fstream>
#include <iostream>
#include "../external/minIni.h"
#include "../include/settings.h"

using namespace std;
using namespace std::filesystem;

// Initialisation des variables statiques
float Settings::drillHoleDepth = 3.0f;
float Settings::drillSpindleLiftHeight = 5.0f;
int Settings::drillSpindleRotationSpeed = 10000;
int Settings::drillSpindleSpeed = 50;

float Settings::cutDepth = 2.0f;
int Settings::cutPass = 3;
float Settings::cutSpindleLiftHeight = 5.0f;
int Settings::cutSpindleRotationSpeed= 10000;
int Settings::cutSpindleZSpeed = 50;
int Settings::cutFeedRate = 80;
float Settings::toolDiameter = 3.0f;

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

        drillHoleDepth = static_cast<float>(ini_getf("drill_work", "hole_depth", drillHoleDepth, configPath));
        drillSpindleLiftHeight = static_cast<float>(ini_getf("drill_work", "spindle_lift_height", drillSpindleLiftHeight, configPath));
        drillSpindleRotationSpeed = static_cast<int>(ini_getl("drill_work", "spindle_rotation_speed", drillSpindleRotationSpeed, configPath));
        drillSpindleSpeed = static_cast<int>(ini_getl("drill_work", "spindle_drilling_speed", drillSpindleSpeed, configPath));

        cutDepth = static_cast<float>(ini_getf("cut_work", "cut_depth", cutDepth, configPath));
        cutPass = static_cast<int>(ini_getf("cut_work", "cut_pass", cutPass, configPath));
        cutSpindleLiftHeight = static_cast<float>(ini_getf("cut_work", "spindle_lift_height", cutSpindleLiftHeight, configPath));
        cutSpindleRotationSpeed = static_cast<int>(ini_getf("cut_work", "spindle_rotation_speed", cutSpindleRotationSpeed, configPath));
        cutSpindleZSpeed = static_cast<int>(ini_getf("cut_work", "spindle_z_speed", cutSpindleZSpeed, configPath));
        cutFeedRate = static_cast<int>(ini_getf("cut_work", "feed_rate", cutFeedRate, configPath));
        toolDiameter = static_cast<float>(ini_getf("cut_work", "tool_diameter", toolDiameter, configPath));


    } else {
        std::cout << endl<< "Cannot find config file" << endl;

        path configPath(configFile);
        create_directories(configPath.parent_path());

        std::ofstream configOut(configFile);
        if (!configOut.is_open()) {
            std::cerr << "Error : Cannot create a config file " << configFile << endl;
            return -1;
        }

        configOut << "[drill_work]\n";
        configOut << "hole_depth = " << to_string(drillHoleDepth) <<"\n";
        configOut << "spindle_lift_height = "<< to_string(drillSpindleLiftHeight) <<"\n";
        configOut << "spindle_rotation_speed = "<< to_string(drillSpindleRotationSpeed) <<"\n";
        configOut << "spindle_drilling_speed = "<< to_string(drillSpindleSpeed) <<"\n";

        configOut << "[cut_work]\n";
        configOut << "cut_depth = " << to_string(cutDepth) <<"\n";
        configOut << "cut_pass = " << to_string(cutPass) <<"\n";

        configOut << "spindle_lift_height = "<< to_string(cutSpindleLiftHeight) <<"\n";
        configOut << "spindle_rotation_speed = "<< to_string(cutSpindleRotationSpeed) <<"\n";
        configOut << "spindle_z_speed = "<< to_string(cutSpindleZSpeed) <<"\n";
        configOut << "feed_rate = "<< to_string(cutFeedRate) <<"\n";
        configOut << "tool_diameter = "<< to_string(toolDiameter) <<"\n";

        configOut.close();

        cout << "New config file created: " << configFile << endl << endl;
    }

    // Show used values
    std::cout << "Drill work" << endl;
    std::cout << "-------------------" << endl;

    std::cout << "- Hole depth: " << drillHoleDepth << "mm" <<  endl;
    std::cout << "- Spindle lift height: " << drillSpindleLiftHeight << "mm" << endl;
    std::cout << "- Spindle rotation speed: " << drillSpindleRotationSpeed << "rpm" << endl;
    std::cout << "- Spindle drilling speed: " << drillSpindleSpeed << "mm/s" << endl << endl;

    std::cout << "Cut work" << endl;
    std::cout << "-------------------" << endl;

    std::cout << "- Cut depth: " << cutDepth << "mm" <<  endl;
    std::cout << "- Cut pass: " << cutPass <<  endl;
    std::cout << "- Spindle lift height: " << cutSpindleLiftHeight << "mm" << endl;
    std::cout << "- Spindle rotation speed: " << cutSpindleRotationSpeed << "rpm" << endl;
    std::cout << "- Spindle Z axe speed: " << cutSpindleZSpeed << "mm/s" << endl;
    std::cout << "- Spindle feed rate: " << cutFeedRate << "mm/s" << endl;
    std::cout << "- Tool diameter: " << toolDiameter << "mm" << endl << endl;

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


