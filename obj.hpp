#ifndef spheco_obj_hpp
#define spheco_obj_hpp

#include <SFML/Graphics.hpp>
#include <iostream>

namespace spheco {
    class obj {
    public:
        sf::Vector2f pos, vel, acc;
        sf::Vector2u gridpt;
        float radius, mass;
        // obj() = default;
        // obj(const sf::Vector2f& pos, const sf::Vector2f& vel, const sf::Vector2f& acc,
        // const sf::Vector2u& gridpt,
        // const float& radius, const float& mass) : pos{pos}, vel{vel}, acc{acc}, gridpt{gridpt}, radius{radius}, mass{mass} {}
        // obj(const obj& o) : pos{o.pos}, vel{o.vel}, acc{o.acc}, gridpt{o.gridpt}, radius{o.radius}, mass{o.mass} {
        //     std::cout << "cpy: " << mass << std::endl;
        // }
    };
}

#endif //spheco_obj_hpp