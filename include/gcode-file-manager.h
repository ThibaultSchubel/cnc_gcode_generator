#pragma once
#include <string>
#include <vector>
using namespace std;

class GCodeFileManager {
    public:
        static string extractDirectoryPath(string filePath);
        static int saveGcode (const vector<string>& gcode, const string &fileName);
};
