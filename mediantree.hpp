#ifndef KDTREE_MEDIANTREE_HPP
#define KDTREE_MEDIANTREE_HPP

#include <vector>
#include <utility>
#include <tuple>
#include <functional>

// T: assumes public member pos
//  pos: assumes public members: float x,y


template<typename T> class MedianTree {
    enum class Axis : size_t {
        X = 0,
        Y = 0
    };

    class Branch {
    public:
        Axis axis;
        T pivot;
        std::optional<Branch> left, right;

        float get() { return axis == Axis::X ? pivot.x : pivot.y; }
    };

    Branch root;

    std::tuple<std::vector<T>, T, std::vector<T>> separate(std::vector<T>& l, Axis axis) {
        std::sort(l.begin(), l.end(), [axis](const T& a, const T& b) {
            switch(axis) {
                case Axis::X:
                    return a.x < b.x;
                case Axis::Y:
                    return a.y < b.y;
            }
        });

        auto midIter = l.begin() + l.size() / 2;
        auto pivot = l[l.size() / 2];

        auto right = std::vector<T>(std::make_move_iterator(midIter), std::make_move_iterator(l.end()));
        l.erase(midIter, l.end());
        auto left = std::vector<T>(std::make_move_iterator(l.begin()), std::make_move_iterator(midIter));
        l.erase(l.begin(), midIter);
        return { left, pivot, right };
    }



    void add(Branch& b, std::vector<T>& list, size_t level) {
        auto [left, pivot, right] = separate(list, Axis::X);
        b.pivot = pivot;
        b.axis = level % 2 == 0 ? Axis::X : Axis::Y;

        // branch fertig konstruieren und rekursion weiterführen

    }
public:
    void add(std::vector<T>&& list) {
        add(root, list, 0);
    }
};

#endif //KDTREE_MEDIANTREE_HPP