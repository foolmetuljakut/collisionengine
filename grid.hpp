#ifndef spheco_grid_hpp
#define spheco_grid_hpp

#include "obj.hpp"
#include "util.hpp"

#include <SFML/Graphics.hpp>

#include <vector>
#include <cmath>
#include <algorithm>

namespace spheco
{
    /*next: replace vector with fixed length array containing up to X integers,
            mimic arraylist locally
            what X is more performant than a std::vector?*/
    class grid {
        size_t width, height;
        std::vector<unsigned> **gridspace;
    public:
        grid(size_t width, size_t height) : width{width}, height{height} {
            gridspace = new std::vector<unsigned>*[width];
            for(size_t i{width}; i--;)
                gridspace[i] = new std::vector<unsigned>[height];
        }
        ~grid() {
            for(size_t i{width}; i--;)
                delete[] gridspace[i];
            delete[] gridspace;
        }
        std::vector<unsigned>& operator[](const sf::Vector2u& pos) { return gridspace[pos.x][pos.y]; }
        const size_t& getw() const { return width; }
        const size_t& geth() const { return height; }
    };

}

#endif // spheco_grid_hpp