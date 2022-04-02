//
// Created by nicolaj on 3/27/22.
//
// Inspired by:
// Nicholas J. A. Harvey and Kevin C. Zatloukal.
// The post-order heap.
// In Proc. Third International Conference on Fun with Algorithms
// (FUN), 2004.
// http://people.csail.mit.edu/nickh/Publications/PostOrderHeap/FUN04-PostOrderHeap.pdf
//
// Adapted by Nicolaj Kuno Bræmhøj for variable degree heaps.
// Contact: nicolaj (at) braemhoej (dot) me
//

#include <cmath>
#include "postorder_heap.h"

template <typename T, typename Container, typename Compare>
postorder_heap<T, Container, Compare>::postorder_heap()
        :   postorder_heap(Compare(), 2) // Constructor delegation
{ }

template <typename T, typename Container, typename Compare>
postorder_heap<T, Container, Compare>::postorder_heap(Compare compare)
        :   postorder_heap(compare, 2) // Constructor delegation
{ }

template <typename T, typename Container, typename Compare>
postorder_heap<T, Container, Compare>::postorder_heap(Compare compare, int degree)
        :   _container(Container()),
            _comparator(compare),
            _degree(degree),
            _sizes(std::vector<int>())
{ }

template <typename T, typename Container, typename Compare>
void postorder_heap<T, Container, Compare>::heapify(int index, int size_of_subtree) {
    T initial_root = _container[index];
    while (size_of_subtree > 1) {
        size_of_subtree /= _degree;
        int right_child_index = (index - 1);
        int prioritised_child_index = right_child_index;
        T prioritised_child = _container[right_child_index];
        for (int child_offset = 1; child_offset < _degree; child_offset++) {
            int childIndex = right_child_index - (child_offset * size_of_subtree);
            T child = _container[childIndex];
            if (_comparator(child, prioritised_child)) {
                prioritised_child_index = childIndex;
                prioritised_child = child;
            }
        }
        if (!_comparator(prioritised_child, initial_root))
            break;
        _container[index] = prioritised_child;
        index = prioritised_child_index;
    }
    _container[index] = initial_root;
}

template <typename T, typename Container, typename Compare>
void postorder_heap<T, Container, Compare>::push(const value_type &element) {
    _container.push_back(element);
    if (_sizes.size() >= _degree) {
        bool should_merge_trees = true;
        for (int offset = 1; offset < _degree; offset++) {
            int tree_index = _sizes.size() - offset;
            should_merge_trees &= _sizes[tree_index] == _sizes[tree_index - 1];
        }
        if (should_merge_trees) {
            int size_of_subtree = 1 + (_degree * _sizes[_sizes.size()-1]);
            int root_of_subtree = _container.size() - 1;
            for (int index = 0; index < _degree; index++)
                _sizes.pop_back();
            _sizes.push_back(size_of_subtree);
            heapify(root_of_subtree, size_of_subtree);
            return;
        }
    }
    _sizes.push_back(1);
}

template <typename T, typename Container, typename Compare>
void postorder_heap<T, Container, Compare>::pop() {
    // TODO: Improve performance of top / pop combo.
    poll();
}

template <typename T, typename Container, typename Compare>
typename Container::const_reference postorder_heap<T, Container, Compare>::top() {
    // Initialise prioritised_root_index as root of rightmost heap, size of rightmost heap, and root cursor as second rightmost heap
    int prioritised_root_index = _container.size() - 1;
    int root_cursor = prioritised_root_index - _sizes.back();
    T largest_root = _container[prioritised_root_index];
    // Reverse scan the roots of the forest ...
    for (int size_index = _sizes.size() - _degree; size_index >= 0; size_index--) {
        int next_size = _sizes[size_index];
        T root = _container[root_cursor];
        if (_comparator(root, largest_root))
            prioritised_root_index = root_cursor;
        root_cursor -= next_size;
    }
    return _container[prioritised_root_index];
}

template <typename T, typename Container, typename Compare>
T postorder_heap<T, Container, Compare>::poll() {
    // Initialise prioritised_root_index as root of rightmost heap, size of rightmost heap, and root cursor as second rightmost heap
    int prioritised_root_index = _container.size() - 1;
    int size = _sizes.back();
    int root_cursor = prioritised_root_index - size;
    T prioritised_root = _container[prioritised_root_index];

    // Reverse scan the roots of the forest ...
    for (int size_index = _sizes.size() - _degree; size_index >= 0; size_index--) {
        int next_size = _sizes[size_index];
        T element = _container[root_cursor];
        if (_comparator(element, prioritised_root)) {
            prioritised_root = element;
            prioritised_root_index = root_cursor;
            size = next_size;
        }
        root_cursor -= next_size;
    }

    // Split rightmost heap at root.
    int size_of_subtree = _sizes.back() / _degree;
    _sizes.pop_back();

    // If rightmost heap > 1.
    if (size_of_subtree) {
        for (int index = 0; index < _degree; index++)
            _sizes.push_back(size_of_subtree);
    }
    // Remove root of rightmost heap.
    T last = _container.back();
    _container.pop_back();
    // If identified top is not root of rightmost heap...
    if (prioritised_root_index < _container.size()) {
        _container[prioritised_root_index] = last;
        heapify(prioritised_root_index, size);
    }

    return prioritised_root;
}

template <typename T, typename Container, typename Compare>
bool postorder_heap<T, Container, Compare>::empty() {
    return size() == 0;
}

template <typename T, typename Container, typename Compare>
typename Container::size_type postorder_heap<T, Container, Compare>::size() {
    return _container.size();
}