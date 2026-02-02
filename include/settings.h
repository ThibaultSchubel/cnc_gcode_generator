#pragma once
#ifndef CNC_GCODE_GENERATOR_SETTINGS_H
#define CNC_GCODE_GENERATOR_SETTINGS_H
#include <string>
using namespace std;


class Settings {
public:
    static float holeDepth;
    static float spindleLiftHeight;
    static int spindleRotationSpeed;
    static int spindleDrillingSpeed;

    static int init();

private:

    static char *homeDir;
    static string configDirectory;
    static string configFile;

    static int checkOrCreateDirectory();
    static int checkOrCreateConfigFile();
};

#endif
