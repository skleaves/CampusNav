#ifndef MAP_H
#define MAP_H

#include <QApplication>

struct Pos
{
    int x;
    int y;
    Pos(int x = 0,int y = 0) {
        this->x = x;
        this->y = y;
    }
    void operator=(const Pos &a){
        x = a.x;
        y = a.y;
    }
    bool operator!=(const Pos &a){
        return (x != a.x && y != a.y);
    }
    bool operator==(const Pos &a){
        if(std::abs(x-a.x) <= 15 && std::abs(y-a.y) <= 15) return true;
        return false;
    }
};

class Edge{
public:
    Pos start_pos;
    Pos end_pos;
    double len;
    double Len(){ return dist(start_pos,end_pos); }
    Edge( Pos s=Pos(),Pos e=Pos() ){ start_pos=s; end_pos=e; len=dist(start_pos,end_pos); }
    void operator=(Edge& a){
        start_pos=a.start_pos;
        end_pos=a.end_pos;
        len=a.len;
    }
    void operator=(const Edge& a){
        start_pos=a.start_pos;
        end_pos=a.end_pos;
        len=a.len;
    }
    bool operator==(const Edge &a){
        return (start_pos==a.start_pos && end_pos==a.end_pos);
    }
    bool operator!=(const Edge &a){
        return (start_pos!=a.start_pos || end_pos!=a.end_pos);
    }
    bool operator<(const Edge &a){
        return dist(start_pos,end_pos)<dist(a.start_pos,a.end_pos);
    }
    bool operator<=(const Edge &a){
        return dist(start_pos,end_pos)<=dist(a.start_pos,a.end_pos);
    }
    bool operator>(const Edge &a){
        return dist(start_pos,end_pos)>dist(a.start_pos,a.end_pos);
    }
    bool operator>=(const Edge &a){
        return dist(start_pos,end_pos)>=dist(a.start_pos,a.end_pos);
    }

private:
    double dist(Pos a, Pos b);
};


class Map
{
public:
    Map();
};

#endif // MAP_H
