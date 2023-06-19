#include "application.hpp"

// #include <iostream>
// #include "mediantree.hpp"
// #include "obj.hpp"

using namespace spheco; 

int main(int argc, char **argv) {
    auto app = spheco::Application("hey there!", 1280, 800, 0.1, 8, 4, 1);
    app.run();

    // std::vector<obj> objects;
    // for(unsigned i{10}; i--;)
    //     objects.push_back(obj{  sf::Vector2f{static_cast<float>(i), static_cast<float>(2*i)},
    //                             sf::Vector2f{0,0}, sf::Vector2f{0,0},
    //                             sf::Vector2u{0,0},
    //                             1, 1});

    // auto tree = mediantree<obj>(objects.size(), [](const obj& o, size_t depth) { 
    //                                                 return depth % 2 ? o.pos.y : o.pos.x; 
    //                                             },
    //                                             [](const obj& a, const obj& b) {
    //                                                 auto fx = a.pos.x - b.pos.x,
    //                                                     fy = a.pos.y - b.pos.y;
    //                                                 return fx * fx + fy * fy - a.radius * a.radius - b.radius * b.radius;
    //                                             });
    // tree.fillfrom(objects, 0, objects.size());

    // for(int i{0}; i < 16; i++) 
    //     try {
    //         std::cout << i << ": " << tree[i].pos.x << std::endl;
    //     }
    //     catch(const NullPtrException& e) {
    //         std::cout << "-" << std::endl;
    //     }
    //     catch(const BranchLeafException& e) {
    //         std::cout << "branch(at " << (e.depth % 2 ? "y" : "x") << "=" << e.branchvalue << ")" << std::endl;
    //     }

    return 0;
}