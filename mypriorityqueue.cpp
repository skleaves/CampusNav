#include "mypriorityqueue.h"

//添加新元素到队尾 然后上浮
void MyPriorityQueue::push(QPairI key)
{
    //添加到队尾
    _pq.push_back(key);
    _size++;
    //如果只有一个元素
    int idx = _size - 1;
    if (idx == 0) return;

    //上浮 key大于其父节点上浮 idx的父节点是(idx -1) / 2
    while (idx > 0 && _cmp(_pq[(idx -1) / 2], key)) {
        _pq[idx] = _pq[(idx - 1) / 2]; //将父节点换到下面
        idx = (idx - 1) / 2; //更新idx直到idx的父节点大于key
    }
    _pq[idx] = key; //将key放到正确的位置
}

//将顶部元素删除，将尾部元素替换到顶部，然后下沉该元素
void MyPriorityQueue::pop()
{
    if (_size == 0) return;
    if (_size == 1) {
        _pq.clear();
        _size = 0;
        return;
    }
    //将末尾的元素换到头部，末尾的pop出去
    _pq[0] = _pq[_size-1];
    _pq.pop_back();
    //然后下沉新换到头部的元素
    _size--;
    int idx = 0;
    QPairI tmp = _pq[0];
    int leftChild = 2 * (idx + 1) - 1;
    int rightChild = 2 * (idx + 1);
    while (leftChild < _size) {
        //child记录下沉到左子节点/右子节点/不动
        int child = idx;
        if (_cmp(tmp, _pq[leftChild]))child = leftChild;
        if (rightChild < _size && _cmp(_pq[child], _pq[rightChild]))child = rightChild;
        if (idx == child)break;
        _pq[idx] = _pq[child]; //child上浮 child的位置暂存
        idx = child;
        leftChild = 2 * (idx + 1) - 1; //如果idx还存在左右子节点继续
        rightChild = 2 * (idx + 1);
    }
    _pq[idx] = tmp;
}

//返回堆顶元素
QPairI MyPriorityQueue::top()
{
    if (empty()) throw("Priority queue is empty!");
    return _pq[0];
}

//清空堆
void MyPriorityQueue::clear()
{
    _pq.clear();
    _size = 0;
}
