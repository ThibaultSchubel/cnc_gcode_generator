#pragma once
#include <string>
#include <vector>
#include <map>

#include "./gbr_reader.h"
using namespace std;


class CuttingGcodeCreator
{
public:
    static int generateCuttingGCodeFiles (string dirPath, const vector<Vertex>& polygon);

private:
    static vector<Vertex> cuttingPath;
    static vector<std::string> getGCodeStart();
    static vector<std::string> getGCodeEnd();

    static void generatePass (vector<string>& gcode, const float passDepth);
    static void generateSegment (vector<Vertex> startPoint, vector<Vertex> endPoint,vector<string>& gcode, const float depth );
    //static void addPassToGcode (vector<string>& gcode, double x, double y);
    //static int generateGcode (const string &path, const vector<Vertex>& polygon, const float& diameterMin);
    //static string generateFileName (const string &path, const float & holeDiameter);
    static string formatCoordinate ( double coordinate);

};
