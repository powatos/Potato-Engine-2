/** @file Hierarchy.h */
#pragma once

#include <vector>

template <typename T>
struct Hierarchy
{
    std::vector< Hierarchy<T>* > Children;

    T* Object;

    void AddChild(T* child) {
        Hierarchy<T>* childHierarchy = new Hierarchy<T>();
        childHierarchy->Object = child;

        Children.push_back(childHierarchy);
    }

    ~Hierarchy() {
        for (Hierarchy<T>* child : Children) {
            delete child;
        }

        delete Object;
    }

};

