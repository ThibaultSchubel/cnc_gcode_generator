#pragma once
#include <map>
#include <string>
#include <vector>
using namespace std;

struct HoleObject {
    double x, y;
    int tool;
};

class DrlReader {
public:
    static vector<std::pair<int, float>> toolDiametersList;

    //index: tool number - value: tool diameter
    static vector<HoleObject> holes;

    static int readFile(const string &drlFilePath);

private:
    static void addTool(const string &drlLine);
    static bool isValidToolString (const std::string& str);
    static int extractToolNumber(const std::string& str);
    static bool isValidCoordinates (const std::string& str);
    static void pushHole(const string &drlLine, int tool );

    static void reorderToolDiametersList ();

};