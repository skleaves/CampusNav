#ifndef MYPRIORITYQUEUE_H
#define MYPRIORITYQUEUE_H

#include <QPair>
#include <vector>
using std::vector;
typedef QPair<int, double> QPairI;

class MyPriorityQueue
{
private:
    int _size = 0;
    vector<QPairI> _pq;

public:
    MyPriorityQueue() {};
    ~MyPriorityQueue() {};

    void push(QPairI key);  //入队
    void pop(); //出队
    QPairI top(); //取队顶元素
    void clear();
    int size() { return _size; }
    bool empty() { return _size == 0 ? true : false; }
    //按照距离从小到大排序
    bool _cmp(const QPair<int, double>& a, const QPair<int, double>& b) {
        if (a.second != b.second) return a.second > b.second;
        return a.first > b.first;
    }
};


#endif // MYPRIORITYQUEUE_H
