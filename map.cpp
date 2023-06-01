#include "map.h"

int Pos::cnt = 0;    //节点的编号 从0开始

Map::Map()
{
}


void Map::dijkstra(int startID, int n)
{
    qDebug() << startID << n;
    int start, end;
    QVector<double> dist(n, 1e9);   //距离
    QVector<int> path(n, -1);       //路径
    QVector<QPairI> realDist;
    QVector<QPair<int, int>> realPath;

    bool st[n];
    for (int i = 0; i < n; i ++) st[i] = false;

    start = idToIdx[startID];
    //end = idToIdx[endID];

    dist[start] = 0;

    for (int i = 0; i < n; i ++)
    {
        int temp = -1;
        //找到当前未被确定的点中距离最小的点
        for (int j = 0; j < n; j ++ ) {
            if (!st[j] && (temp == -1 || dist[j] < dist[temp])) temp = j;
        }

        //for (int k = 0; k < n; k ++) qDebug() << k << st[k];
        //qDebug() << i << temp;

        st[temp] = true;
        //尝试用该点更新其他点的距离
        int tempID = m_all_locs[temp]->id;
        for (auto lk : m_adjList) {
            if (lk.first().first == tempID) {
                for (auto p : lk) {
                    if (dist[temp] + p.second < dist[idToIdx[p.first]]) {
                        dist[idToIdx[p.first]] = dist[temp] + p.second;
                        path[idToIdx[p.first]] = temp;
                    }
                }
                break;
            }
        }
        //return true;
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
