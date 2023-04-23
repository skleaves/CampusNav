#ifndef MAP_H
#define MAP_H

#include <QApplication>
#include <QDataStream>

struct Pos
{
    int x;
    int y;
    bool isBuild;
    Pos(int x = 0, int y = 0, bool isBuild = false) {
        this->x = x;
        this->y = y;
        this->isBuild = isBuild;
    }
    void operator=(const Pos &a) {
        x = a.x;
        y = a.y;
    }
    bool operator!=(const Pos &a) {
        return (x != a.x && y != a.y);
    }
    bool operator==(const Pos &a) {
        if(std::abs(x-a.x) <= 15 && std::abs(y-a.y) <= 15) return true;
        return false;
    }
    friend QDataStream &operator<<(QDataStream &out, const Pos &p) {
        out << p.x << p.y << p.isBuild;
        return out;
    }
    friend QDataStream &operator>>(QDataStream &in, Pos &p) {
        in >> p.x >> p.y >> p.isBuild;
        return in;
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
    friend QDataStream &operator<<(QDataStream &out, const Edge &e) {
        out << e.start_pos << e.end_pos << e.len;
        return out;
    }
    friend QDataStream &operator>>(QDataStream &in, Edge &e) {
        in >> e.start_pos >> e.end_pos >> e.len;
        return in;
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
