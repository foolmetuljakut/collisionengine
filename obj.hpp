#ifndef spheco_obj_hpp
#define spheco_obj_hpp

#include <SFML/Graphics.hpp>
#include <iostream>

namespace spheco {
    class obj {
    public:
        sf::Vector2f pos, vel, acc;
        float radius, mass;
        // obj() = default;
        // obj(const sf::Vector2f& pos, const sf::Vector2f& vel, const sf::Vector2f& acc,
        // const sf::Vector2u& gridpt,
        // const float& radius, const float& mass) : pos{pos}, vel{vel}, acc{acc}, gridpt{gridpt}, radius{radius}, mass{mass} {}
        // obj(const obj& o) : pos{o.pos}, vel{o.vel}, acc{o.acc}, gridpt{o.gridpt}, radius{o.radius}, mass{o.mass} {
        //     std::cout << "cpy: " << mass << std::endl;
        // }

        // necessary for kd_tree implementation to access position?
	    static const int DIM = 2;
        float operator[](size_t dimIndex) { return dimIndex == 0 ? pos.x : pos.y; } 
    };
}

#endif //spheco_obj_hpp