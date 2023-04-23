#include "map.h"

Map::Map()
{

}

double Edge::dist(Pos a, Pos b)
{
    int dx = a.x - b.x, dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}
