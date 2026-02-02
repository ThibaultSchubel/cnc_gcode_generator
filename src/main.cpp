#include <iostream>
#include "../include/settings.h"
#include "../include/drl_reader.h"
#include "../include/drilling_gcode_creator.h"
#include "../include/gcode-file-manager.h"


using namespace std;

int main() {

    string drlFilePath;

    cout << endl << "--------------------------"  << endl;
    cout << "CNC Gcode generator v0.0.1" << endl;
    cout << "--------------------------" << endl << endl;

    Settings::init ();

    cout << "Enter DRL file path: ";
    cin  >> drlFilePath;

    DrlReader::readFile(drlFilePath);
    DrillingGcodeCreator::generateGCodeFiles(GCodeFileManager::extractDirectoryPath(drlFilePath), DrlReader::holes, DrlReader::toolDiametersList);

    return 0;
}

// /Users/thibault/kDrive/Perso/Appart/Light-Sytem-Client_V2/LightSystemClient_V2/export/LightSystemClient_V2.drl