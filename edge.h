#ifndef EDGE_H
#define EDGE_H

#include <QtGui>

class Edge
{
public:
    Edge(QPoint p1, QPoint p2);

public:
    int yMax;
    float xInc,x;
    bool operator == (const Edge& e) const { return yMax == e.yMax;}
};


#endif // EDGE_H
