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

    static float supportLength;
    static int cuttingMaxLength;

    static vector<Vertex> cuttingPath;
    static vector<std::string> getGCodeStart();
    static vector<std::string> getGCodeEnd();

    static void generatePass (vector<string>& gcode, float passDepth);
    static void generateSegment (Vertex startPoint, Vertex endPoint,vector<string>& gcode, float depth );
    static void calculatePointOnVector(const Vertex &start, const Vertex &end, float length, Vertex &point);
    //static void addPassToGcode (vector<string>& gcode, double x, double y);
    //static int generateGcode (const string &path, const vector<Vertex>& polygon, const float& diameterMin);
    //static string generateFileName (const string &path, const float & holeDiameter);
    static string formatCoordinate ( double coordinate);

};
