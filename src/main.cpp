#include <iostream>
#include "../include/settings.h"

/*--Drill files generation--*/
#include "../include/drl_reader.h"
#include "../include/drilling_gcode_creator.h"
#include "../include/gcode-file-manager.h"

/*--Cut files generation--*/
#include "cutting_gcode_creator.h"
#include "../include/gbr_reader.h"
#include "outline_manager.h"


using namespace std;

int main() {

    string answer;

    cout << endl << "--------------------------"  << endl;
    cout << "CNC Gcode generator v0.0.1" << endl;
    cout << "--------------------------" << endl << endl;

    Settings::init ();

    /*--Drill files generation--*/
    /*
    cout << "Do you want to create files for drills (Y/n): ";
    getline(cin, answer);

    if (answer == "Y" | answer == "y" || answer.empty()) {
        string drlFilePath;
        cout << "Enter DRL file path: ";
        cin  >> drlFilePath;
        cout << endl;

        DrlReader::readFile(drlFilePath);
        DrillingGcodeCreator::generateGCodeFiles(GCodeFileManager::extractDirectoryPath(drlFilePath), DrlReader::holes, DrlReader::toolDiametersList);
        cout << "--------------------------" << endl << endl;
    }
*/
    /*--Cut files generation--*/
    cout << "Do you want to create files for edges cut(Y/n): ";
    getline(cin, answer);

    if (answer == "Y" | answer == "y" || answer.empty()) {
        string gbrFilePath;
        cout << "Enter GBR file path: ";
        cin  >> gbrFilePath;
        cout << endl;

        GbrReader::readFile(gbrFilePath);
        CuttingGcodeCreator::generateCuttingGCodeFiles (GCodeFileManager::extractDirectoryPath(gbrFilePath), *OutlineManager::readFile(GbrReader::polygon));

        cout << "--------------------------" << endl << endl;
    }

    return 0;
}

// /Users/thibault/kDrive/Perso/Appart/Light-Sytem-Client_V2/LightSystemClient_V2/export2/LightSystemClient_V2.drl
// /Users/thibault/kDrive/Perso/Appart/Light-Sytem-Client_V2/LightSystemClient_V2/export2/LightSystemClient_V2-Edge_Cuts.gbr