#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include "../include/gbr_reader.h"
#include "../include/cutting_gcode_creator.h"
#include "../include/settings.h"

#include <ostream>
using namespace std;

vector<Vertex> CuttingGcodeCreator::cuttingPath;


int CuttingGcodeCreator::generateCuttingGCodeFiles (string dirPath, const vector<Vertex>& polygon) {
    cuttingPath = polygon;
    std::vector<std::string> gCodeLines = getGCodeStart();
    const float passDepth = Settings::cutDepth / Settings::cutPass;

    for (int i = 1; i <= Settings::cutPass; i++) {
        generatePass(gCodeLines, passDepth * i);
    }

    return 0;

}

void CuttingGcodeCreator::generatePass (vector<string>& gcode, const float passDepth) {
}

void CuttingGcodeCreator::generateSegment (vector<Vertex> startPoint, vector<Vertex> endPoint, vector<string>& gcode, const float passDepth ) {
    gcode.push_back("G00 Z-" + to_string(passDepth) + ";" );
}



vector<std::string> CuttingGcodeCreator::getGCodeStart() {
    vector<string> gCodeStart;
    gCodeStart.push_back("G21 G90 G94;");
    gCodeStart.push_back("G00 Z" + formatCoordinate(Settings::cutSpindleLiftHeight) + ";");
    gCodeStart.push_back("G00 X0 Y0;");
    gCodeStart.push_back("M03 S" + to_string(Settings::cutSpindleRotationSpeed) + ";");
    return gCodeStart;
}


vector<std::string> CuttingGcodeCreator::getGCodeEnd() {
    vector<string> gCodeEnd;
    gCodeEnd.push_back("G00 Z" + formatCoordinate(Settings::cutSpindleLiftHeight) + ";");
    gCodeEnd.push_back("G00 X0 Y0;");
    gCodeEnd.push_back("M30;");
    return gCodeEnd;
}


string CuttingGcodeCreator::formatCoordinate ( double coordinate) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << coordinate;
    return oss.str();
}
