#pragma once
#include "./gbr_reader.h"


class OutlineManager {
    public:
        static  vector<Vertex>* readFile(const vector<Vertex> &polygon);

    private:
        static vector<Vertex> outline;

};