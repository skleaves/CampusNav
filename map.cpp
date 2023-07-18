#include "map.h"

int Pos::cnt = 0;    //节点的编号 从0开始

Map::Map()
{
}


void Map::dijkstra(int startID, int n)
{
    //qDebug() << startID << n;
    int start;
    QVector<double> dist(n, 1e9); //距离
    QVector<int> path(n, -1); //路径
    QVector<QPairI> realDist; //将ID与距离关联
    QVector<QPair<int, int>> realPath; //将ID与路径关联
    MyPriorityQueue q;

    bool st[n];
    for (int i = 0; i < n; i ++) st[i] = false;

    start = idToIdx[startID];

    dist[start] = 0;
    q.push(QPairI(start, 0)); //压入起点

    while (q.size()) {
        auto t = q.top();
        q.pop();
        int idx = t.first;
        double dis = t.second;
        if (st[idx]) continue;
        st[idx] = true;
        //尝试用该点更新其他点的距离
        int tID = m_all_locs[idx]->id;
        for (auto lk : m_adjList) {
            if (lk.first().first == tID) {
                for (auto p : lk) {
                    if (dis + p.second < dist[idToIdx[p.first]]) {
                        dist[idToIdx[p.first]] = dis + p.second;
                        path[idToIdx[p.first]] = idx;
                        q.push(QPairI(idToIdx[p.first], dist[idToIdx[p.first]]));
                    }
                }
                break;
            }
        }
    }

    for (int i = 0; i < n; i ++) {
        realDist.push_back(QPairI(m_all_locs[i]->id, dist[i]));
        if (path[i] == -1) realPath.push_back(QPair<int, int>(m_all_locs[i]->id, -1));
        else realPath.push_back(QPair<int, int>(m_all_locs[i]->id, m_all_locs[path[i]]->id));
    }

    this->dist_cache.insert(startID, realDist);
    this->path_cache.insert(startID, realPath);
}




double Edge::dist(int x1, int y1, int x2, int y2)
{
    int dx = x1 - x2, dy = y1 - y2;
    return sqrt(dx * dx + dy * dy);
}
