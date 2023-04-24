#ifndef MAP_H
#define MAP_H

#include <QApplication>
#include <QDataStream>
#include <QVector>
#include <QLinkedList>
#include <QDebug>


struct Pos
{
    int x;
    int y;
    int id;
    bool isBuild;
    static int cnt;
    Pos(int x = 0, int y = 0, bool isBuild = false) {
        this->x = x;
        this->y = y;
        this->id = cnt ++;
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
        out << p.x << p.y << p.id << p.isBuild;
        return out;
    }
    friend QDataStream &operator>>(QDataStream &in, Pos &p) {
        in >> p.x >> p.y >> p.id >> p.isBuild;
        return in;
    }
};

class Edge{
public:
    int start_pos;  //存id
    int end_pos;
    double len;
    Edge() {}
    Edge(int a, int b, int len) {
        start_pos = a;
        end_pos = b;
        this->len = len;
    }
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
        return len < a.len;
    }
    bool operator<=(const Edge &a){
        return len <= a.len;
    }
    bool operator>(const Edge &a){
        return len > a.len;
    }
    bool operator>=(const Edge &a){
        return len >= a.len;
    }
    friend QDataStream &operator<<(QDataStream &out, const Edge &e) {
        out << e.start_pos << e.end_pos << e.len;
        return out;
    }
    friend QDataStream &operator>>(QDataStream &in, Edge &e) {
        in >> e.start_pos >> e.end_pos >> e.len;
        return in;
    }

public:
    static double dist(int x1, int y1, int x2, int y2);

private:
};


class Map
{
public:
    Map();

public:
    QVector<QLinkedList<Pos>> m_map;
    //QVector<Pos> m_all_pos;    //记录所有路径点
    //QVector<Edge> m_all_edges;
};

#endif // MAP_H
