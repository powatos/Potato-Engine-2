/** @file HTreeNode.h */
#pragma once

#include <vector>

/**
 * @brief Compile time functor that always returns a boolean
 * @tparam B Bool true/false
 */
template<bool B>
struct ReturnBool {
    template<typename T>
    constexpr bool operator ()(const T*, const T*) const { return B; }
};

/**
 * @brief Simple hierarchical tree node
 * @note HTrees may only be allocated on the heap, using `new`.
 * @warning Creating child nodes manually results in undefined behaviour! @sa AddChild()
 * @tparam T Hierarchy object type
 */
template <typename T>
struct HTreeNode
{
    std::vector< HTreeNode<T>* > Children;

    HTreeNode<T>* Parent;
    T* Object;

    /**
     * @brief Adds a child to the node
     * * @tparam Pred Predicate to customize insert location: Pred(const T* sibling, const T* insertingChild) -> bool; Defaults to always false
     * @param child Child to insert
     * @param setHierarchy Optional pointer to node pointer to set to newly created child node (associated to the added child); nullptr to skip
     * @param pred Optional custom predicate: return true to insert in front of the sibling; default places at the end
     */
    template<std::predicate<const T*, const T*> Pred = ReturnBool<false>>
    void AddChild(T* child, HTreeNode<T>** setHierarchy = nullptr, const Pred pred = {}) {
        HTreeNode<T>* childHierarchy = new HTreeNode<T>();
        childHierarchy->Parent = this;
        childHierarchy->Object = child;
        if (setHierarchy) { *setHierarchy = childHierarchy; }

        for (auto it = Children.begin(); it != Children.end(); ++it) {
            if (pred((*it)->Object, child)) {
                Children.insert(it, childHierarchy);
                return;
            }
        }

        Children.push_back(childHierarchy);
    }

    /**
     * @brief Reorders a child from beginning
     * @details If an invalid child is passed or the argument isn't actually a child, this function does nothing
     * * @tparam Pred Predicate to customize insert location: Pred(const T* sibling, const T* insertingChild) -> bool
     * @param child Child to reorder
     * @param pred Custom predicate: return true to insert in front of the sibling
     */
    template<std::predicate<const T*, const T*> Pred>
    void ReorderChild(T* child, const Pred pred) {
        if (child == nullptr) { return; }

        auto from = std::ranges::find_if(
            Children.begin(),
            Children.end(),
            [child](const HTreeNode<T>* hierarchy) -> bool {
                return hierarchy->Object == child;
            }
        );
        if (from == Children.end()) { return; }

        for (auto to = Children.begin(); to != Children.end(); ++to) {
            if (pred((*to)->Object, child)) {
                // moves *from to *to in-place

                if (from == to) { return; }

                if (to < from) {
                    std::rotate(to, from, from + 1);
                } else {
                    std::rotate(from, from+1, to + 1);
                }
            }
        }
    }

    /**
     * @brief Finds and returns a child of the node; nullptr if not found
     * @param child Child to find
     * @returns Child that was found or nullptr
     */
    T* FindChild(const T* child) {
        auto it = std::ranges::find(Children, child);
        if (it == Children.end()) { return nullptr; }
        return *it;
    }

    ~HTreeNode() {
        for (const HTreeNode<T>* child : Children) {
            delete child;
        }

        delete Object;
    }

};

