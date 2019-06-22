#include "edge.h"

Edge::Edge(QPoint p1, QPoint p2)
{
    int p1x = p1.x(),p2x = p2.x();
    int p1y = p1.y(),p2y = p2.y();

    this->yMax = std::max(p1y,p2y);

    if(this->yMax == p1y){
        this->x = p2x;
    }
    else{
        this->x = p1x;
    }

    this->xInc = (p1x-p2x)/(float)(p1y-p2y);

}
