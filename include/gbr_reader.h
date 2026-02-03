#pragma once
#include <map>
#include <string>
#include <vector>
using namespace std;

struct Vertex {
    double x, y;
};

class GbrReader {
    public:
        static vector<Vertex> polygon;
        static int readFile(const string &gbrFilePath);

    private:
        static int pushVertex(const string &line);


};