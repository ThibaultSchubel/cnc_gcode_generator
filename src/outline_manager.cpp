#include <vector>
#include "../include/outline_manager.h"
#include "../include/settings.h"

#include "../external/cavaliercontours/polylineoffset.hpp"
using namespace std;


vector<Vertex> OutlineManager::outline;

vector<Vertex>* OutlineManager::readFile(const vector<Vertex> &polygon) {

    cavc::Polyline<double> input;

    for (unsigned int i = 0; i < polygon.size()-1; i++) {
        input.addVertex(polygon[i].x, polygon[i].y, 0);
    }
    input.isClosed() = true;
    std::vector<cavc::Polyline<double>> results = cavc::parallelOffset(input, static_cast<double>(Settings::toolDiameter / 2));

    for (auto v : results[0].vertexes() ) {
        outline.push_back( {v.x(),  v.y()});
    }
    outline.push_back( {results[0].vertexes()[0].x(),  results[0].vertexes()[0].y()});
    return &outline;

}