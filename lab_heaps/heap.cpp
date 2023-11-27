/**
 * @file heap.cpp
 * Implementation of a heap class.
 */

template <class T, class Compare>
size_t heap<T, Compare>::root() const
{
    return 1;
}

template <class T, class Compare>
size_t heap<T, Compare>::leftChild(size_t currentIdx) const
{
    return 2 * currentIdx;
}

template <class T, class Compare>
size_t heap<T, Compare>::rightChild(size_t currentIdx) const
{
    return 2* currentIdx + 1;
}

template <class T, class Compare>
size_t heap<T, Compare>::parent(size_t currentIdx) const
{
    return currentIdx / 2;
}

template <class T, class Compare>
bool heap<T, Compare>::hasAChild(size_t currentIdx) const
{
    return leftChild(currentIdx) < _elems.size();
}

template <class T, class Compare>
size_t heap<T, Compare>::maxPriorityChild(size_t currentIdx) const
{
    if (rightChild(currentIdx) >= _elems.size() || higherPriority(_elems[leftChild(currentIdx)], _elems[rightChild(currentIdx)])) {
        return leftChild(currentIdx);
    } else {
        return rightChild(currentIdx);
    }
}

template <class T, class Compare>
void heap<T, Compare>::heapifyDown(size_t currentIdx)
{
    if (hasAChild(currentIdx)) {
        size_t maxChildIdx = maxPriorityChild(currentIdx);
        if (higherPriority(_elems[maxChildIdx], _elems[currentIdx])) {
            std::swap(_elems[currentIdx], _elems[maxChildIdx]);
            heapifyDown(maxChildIdx);
        }
    }
    return;
}

template <class T, class Compare>
void heap<T, Compare>::heapifyUp(size_t currentIdx)
{
    if (currentIdx == root())
        return;
    size_t parentIdx = parent(currentIdx);
    if (higherPriority(_elems[currentIdx], _elems[parentIdx])) {
        std::swap(_elems[currentIdx], _elems[parentIdx]);
        heapifyUp(parentIdx);
    }
}

template <class T, class Compare>
heap<T, Compare>::heap()
{
    _elems.resize(1);
}

template <class T, class Compare>
heap<T, Compare>::heap(const std::vector<T>& elems)
{
    _elems.push_back(T());
    for (const auto& elem : elems) {
        _elems.push_back(elem);
    }
    for (size_t i = _elems.size()/2; i > 0; i--) {
        heapifyDown(i);
    }
}

template <class T, class Compare>
T heap<T, Compare>::pop()
{
    if (empty()) {
        throw std::out_of_range("Heap is empty");
    }
    T ret = peek();
    _elems[root()] = _elems.back();
    _elems.pop_back();
    heapifyDown(root());
    return ret;
}

template <class T, class Compare>
T heap<T, Compare>::peek() const
{
    if (empty()) {
        throw std::out_of_range("Heap is empty");
    }
    return _elems[root()];
}

template <class T, class Compare>
void heap<T, Compare>::push(const T& elem)
{
    _elems.push_back(elem);
    heapifyUp(_elems.size()-1);
}

template <class T, class Compare>
bool heap<T, Compare>::empty() const
{
    return _elems.size() <= 1;
}
