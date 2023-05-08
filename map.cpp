#include "map.h"

int Pos::cnt = 0;    //节点的编号 从0开始

Map::Map()
{
}


bool Map::dijkstra(int start, int end, int n)
{
//    double d[n + 1];
//    for (int i = 0; i < n; i ++) {
//        d[i] = 1e9;
//    }
//    int path[n + 10];
//    memset(path, 0, sizeof path);
    QVector<double> dist(n, 1e9);   //距离
    QVector<int> path(n, -1);        //路径
    bool st[n];

    dist[start] = 0;

    for (int i = 0; i < n; i ++ )
    {
        int temp = -1;
        //找到当前未被确定的点中距离最小的点
        for (int j = 0; j < n; j ++ )
        {
            if (!st[j] && (temp == -1 || dist[j] < dist[temp])) temp = j;
        }
        st[temp] = true;
        //尝试用该点更新其他点的距离
        for (auto p : this->m_adjList[temp]) {
            if (dist[temp] + p.second < dist[p.first]) {
                dist[p.first] = dist[temp] + p.second;
                path[p.first] = temp;
            }
            //dist[p.first] = qMin(dist[p.first], dist[temp] + p.second);
        }

//        for (int j = 0; j < n; j ++ )
//        {
//            d[j] = min(d[j], d[temp] + g[temp][j]);
//        }
    }

    if (fabs(dist[end] - 1e9) < 1e-3) return false;

    this->dist_cache.insert(start, dist);
    this->path_cache.insert(start, path);
    return true;
}




double Edge::dist(int x1, int y1, int x2, int y2)
{
    int dx = x1 - x2, dy = y1 - y2;
    return sqrt(dx * dx + dy * dy);
}
