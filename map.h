#ifndef MAP_H
#define MAP_H

#include <QApplication>
#include <QDataStream>
#include <QVector>
#include <QLinkedList>
#include <QDebug>
#include <QMap>

typedef QPair<int, double> QPairI;

struct Pos
{
    int x, y, id;
    bool isBuild;

    QString name; //地点名
    QStringList otherName; //地点别名

    static int cnt;
    Pos(int x = 0, int y = 0, bool isBuild = false) {
        this->x = x;
        this->y = y;
        this->id = cnt ++;
        this->isBuild = isBuild;
    }

    //重载操作符
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
        out << p.x << p.y << p.id << p.isBuild << p.name << p.otherName;
        return out;
    }
    friend QDataStream &operator>>(QDataStream &in, Pos &p) {
        in >> p.x >> p.y >> p.id >> p.isBuild >> p.name >> p.otherName;
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
    //重载操作符
    void operator=(const Edge &a){
        start_pos = a.start_pos;
        end_pos = a.end_pos;
        len = a.len;
    }
    bool operator==(const Edge &a){
        return (start_pos == a.start_pos && end_pos == a.end_pos);
    }
    bool operator!=(const Edge &a){
        return (start_pos != a.start_pos || end_pos != a.end_pos);
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

    QVector<Pos *> m_all_locs;          //记录所有路径点
    QVector<Edge *> m_all_edges;
    //邻接表
    QVector<QLinkedList<QPairI>> m_adjList;

    QMap<int, int> idToIdx;
    QMap<int, QVector<QPairI>> dist_cache; //距离数组缓存
    QMap<int, QVector<QPair<int, int>>> path_cache; //路径数组缓存

    void dijkstra(int startID, int n);
    //int[] spfa(int st, int ed);
};

#endif // MAP_H
