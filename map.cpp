#include "map.h"

int Pos::cnt = 0;    //节点的编号 从0开始

Map::Map()
{

}


double Edge::dist(int x1, int y1, int x2, int y2)
{
    int dx = x1 - x2, dy = y1 - y2;
    return sqrt(dx * dx + dy * dy);
}
