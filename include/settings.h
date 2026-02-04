#pragma once
#ifndef CNC_GCODE_GENERATOR_SETTINGS_H
#define CNC_GCODE_GENERATOR_SETTINGS_H
#include <string>
using namespace std;


class Settings {
public:
    // For drill work
    static float drillHoleDepth;
    static float drillSpindleLiftHeight;
    static int drillSpindleRotationSpeed;
    static int drillSpindleSpeed;

    // For cut work
    static float cutDepth;
    static int cutPass;
    static float cutSpindleLiftHeight;
    static int cutSpindleRotationSpeed;
    static int cutSpindleZSpeed;
    static int cutFeedRate;
    static float toolDiameter;


    static int init();

private:

    static char *homeDir;
    static string configDirectory;
    static string configFile;

    static int checkOrCreateDirectory();
    static int checkOrCreateConfigFile();
};

#endif
