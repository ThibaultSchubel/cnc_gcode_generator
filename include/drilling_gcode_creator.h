#pragma once
#include <string>
#include <vector>
#include <map>

#include "./drl_reader.h"
using namespace std;


class DrillingGcodeCreator
{
    public:
        static int generateGCodeFiles (string dirPath, const vector<HoleObject>& holes, vector<std::pair<int, float>> toolDiametersList);

    private:
        static const vector<string> gCodeStart;
        static const vector<string> gCodeEnd;

        static vector<std::string> getGCodeStart();
        static vector<std::string> getGCodeEnd();

        static void addHoleToGcode (vector<string>& gcode, double x, double y);
        static int generateGcode (const string &path, const vector<HoleObject>& holes, const float& diameterMin);
        static string generateFileName (const string &path, const float & holeDiameter);

};
