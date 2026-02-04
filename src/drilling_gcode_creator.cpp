#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include "../include/drilling_gcode_creator.h"
#include "../include/settings.h"
#include "../include/gcode-file-manager.h"
using namespace std;


int DrillingGcodeCreator::generateGCodeFiles (string dirPath, const vector<HoleObject>& holes, vector<std::pair<int, float>> toolDiametersList )
{
    for (int i = 0; i < toolDiametersList.size(); i++) {
        if (generateGcode(generateFileName(dirPath, toolDiametersList[i].second), holes, toolDiametersList[i].first) != 0) return -1;
    }
    return 0;
}

int DrillingGcodeCreator::generateGcode (const string &path, const vector<HoleObject>& holes, const float& diameterMin) {

    std::vector<std::string> gCodeLines = getGCodeStart();

    for (const auto & hole : holes) {
        if(hole.tool >= diameterMin) {
            addHoleToGcode(gCodeLines, hole.x, hole.y);
        }
    }

    std::vector<std::string> gCodeEnd = getGCodeEnd();
    gCodeLines.insert(gCodeLines.end(), gCodeEnd.begin(), gCodeEnd.end());


    return GCodeFileManager::saveGcode(gCodeLines, path);
}

void DrillingGcodeCreator::addHoleToGcode (vector<string>& gcode, const double x, const double y) {
    gcode.push_back("G00 X"+ formatCoordinate(x)+ " Y" + formatCoordinate(y)+ ";");
    gcode.push_back("G00 Z1;");
    gcode.push_back("G01 Z-" + formatCoordinate(Settings::drillHoleDepth) + " F"+ to_string(Settings::drillSpindleSpeed) + ";");
    gcode.push_back("G01 Z1 F" + to_string(Settings::drillSpindleSpeed) +";");
    gcode.push_back("G00 Z" + formatCoordinate(Settings::drillSpindleLiftHeight) +";");
}

vector<std::string> DrillingGcodeCreator::getGCodeStart() {
    vector<string> gCodeStart;
    gCodeStart.push_back("G21 G90 G94;");
    gCodeStart.push_back("G00 Z" + formatCoordinate(Settings::drillSpindleLiftHeight) + ";");
    gCodeStart.push_back("G00 X0 Y0;");
    gCodeStart.push_back("M03 S" + to_string(Settings::drillSpindleRotationSpeed) + ";");
    return gCodeStart;
}

vector<std::string> DrillingGcodeCreator::getGCodeEnd() {
    vector<string> gCodeEnd;
    gCodeEnd.push_back("G00 Z" + formatCoordinate(Settings::drillSpindleLiftHeight) + ";");
    gCodeEnd.push_back("G00 X0 Y0;");
    gCodeEnd.push_back("M30;");
    return gCodeEnd;
}


string DrillingGcodeCreator::generateFileName (const string &path, const float & holeDiameter) {
    return path+"/drilling_tool-" + to_string(holeDiameter) + "mm.gcode";
}

string DrillingGcodeCreator::formatCoordinate ( double coordinate) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << coordinate;
    return oss.str();
}