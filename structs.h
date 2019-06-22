#ifndef STRUCTS_H
#define STRUCTS_H

#include <vector>
#include <QPoint>

using namespace std;

typedef struct PolygonS {
    vector<float[3]> *vertices;
    float color[4];
} PolygonS;

enum ShadingMethods {
    FLAT    = 0,
    GOURAUD = 1,
    PHONG   = 2
};

#endif // STRUCTS_H
