#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include "../include/gbr_reader.h"
#include "../include/cutting_gcode_creator.h"
#include <cmath>
#include <iostream>

#include "../include/settings.h"
#include "../include/gcode-file-manager.h"

#include <ostream>
using namespace std;

vector<Vertex> CuttingGcodeCreator::cuttingPath;
float CuttingGcodeCreator::supportLength = 10.0f;
int CuttingGcodeCreator::cuttingMaxLength = 30;

int CuttingGcodeCreator::generateCuttingGCodeFiles (string dirPath, const vector<Vertex>& polygon) {
    cuttingPath = polygon;
    std::vector<std::string> gCodeLines = getGCodeStart();
    const float passDepth = Settings::cutDepth / Settings::cutPass;

    for (int i = 1; i <= Settings::cutPass; i++) {
        generatePass(gCodeLines, passDepth * i);
    }

    for (auto i : gCodeLines) {
        cout << i << endl;
    }

    std::vector<std::string> gCodeEnd = getGCodeEnd();
    gCodeLines.insert(gCodeLines.end(), gCodeEnd.begin(), gCodeEnd.end());

    return GCodeFileManager::saveGcode(gCodeLines, dirPath+"/cutting_path.gcode");


}

void CuttingGcodeCreator::generatePass (vector<string>& gcode, const float passDepth) {
    for (int i = 0; i < cuttingPath.size()-1; i++) {
        generateSegment(cuttingPath[i], cuttingPath[i+1], gcode, passDepth);
    }
}

void CuttingGcodeCreator::generateSegment ( Vertex startPoint, Vertex endPoint, vector<string>& gcode, const float passDepth ) {
    gcode.push_back("G00 X" + to_string(startPoint.x) + " Y" +  to_string(startPoint.y)+ ";" );
    gcode.push_back("G00 Z-" + to_string(passDepth) + ";" );

    //Add supports
    float deltaX = endPoint.x - startPoint.x;
    float deltaY = endPoint.y - startPoint.y;
    float vectorLength =  std::hypot(deltaX, deltaY);

    if (vectorLength > cuttingMaxLength) {
        int supportNumber = static_cast<int>(std::ceil(vectorLength / cuttingMaxLength));
        float vectorPiece = static_cast<float>((vectorLength / (supportNumber+1))+1);


        for (int i = 0; i < supportNumber; i++) {
            Vertex supportStartPoint;
            Vertex supportEndPoint;
            calculatePointOnVector(startPoint, endPoint,  (vectorPiece * (i+1)) -(supportLength/2), supportStartPoint );
            calculatePointOnVector(startPoint, endPoint,  (vectorPiece * (i+1)) +(supportLength/2), supportEndPoint );

            gcode.push_back("G00 X" + to_string(supportStartPoint.x) + " Y" +  to_string(supportStartPoint.y)+ ";" );
            gcode.push_back("G00 Z" + formatCoordinate(Settings::cutSpindleLiftHeight) + ";" );
            gcode.push_back("G01 X" + to_string(supportEndPoint.x) + " Y" +  to_string(supportEndPoint.y)+ ";" );
            gcode.push_back("G00 Z-" + to_string(passDepth) + ";" );
        }
    }

    gcode.push_back("G00 X" + to_string(endPoint.x) + " Y" +  to_string(endPoint.y)+ ";" );
    gcode.push_back("G00 Z" + formatCoordinate(Settings::cutSpindleLiftHeight) + ";" );
    gcode.push_back(";;" );

}


void CuttingGcodeCreator::calculatePointOnVector(const Vertex& start, const Vertex& end,  float length, Vertex& point ){
    // Calcul de la longueur totale du vecteur
    float deltaX = end.x - start.x;
    float deltaY = end.y - start.y;
    float vectorLength = std::sqrt(std::pow(deltaX, 2) + std::pow(deltaY, 2));

    // Si la longueur du vecteur est nulle, le point calculé est le point de départ
    if (vectorLength <= 0.0f) {
        point.x = start.x;
        point.y = start.y;
        return;
    }

    // Calcul du ratio de la distance par rapport à la longueur totale du vecteur
    float ratio = length / vectorLength;

    // Limite le ratio à 1.0 pour éviter de dépasser le point de fin
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }

    // Calcul des coordonnées du point
    point.x = start.x + ratio * deltaX;
    point.y = start.y + ratio * deltaY;
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
