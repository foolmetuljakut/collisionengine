#ifndef spheco_worldspace_hpp
#define spheco_worldspace_hpp

#include "obj.hpp"
#include "util.hpp"

#include <SFML/Graphics.hpp>

#include <vector>
#include <cmath>
#include <algorithm>

#include <limits>
#include "kd-tree/kdtree.hpp"

namespace spheco {

    class worldspace {

        float mW, mH;
        std::vector<obj> objects;
        kdt::KDTree<obj> objectTree;
        sf::Vector2f gForce;

        size_t cneighbours; // count collisions
        std::vector<size_t> cnBins; // bin collisions over multiple iterations for profiling

        void handlecollision(const unsigned& i, const unsigned& j) {
            cneighbours++;
            sf::Vector2f dist = objects[j].pos - objects[i].pos;
            float fdist = dist.x * dist.x + dist.y * dist.y;
            float dr = objects[i].radius + objects[j].radius;
            float d = fdist - dr*dr;
            if (d < 0) // -0.05*dr
            {
                fdist = sqrt(fdist);
                d = fdist - dr;
                sf::Vector2f displacement = -d / (2*fdist) * dist;

                objects[j].pos += displacement;
                objects[i].pos -= displacement;
            }
        }

    public:
        void updatecoords(float dt) {
            // verlet integration
            // x(n+1) = x(n) + v(n)dt + a(n)dt^2 / 2
            // update a(n)[x,v] -> a(n+1)[x,v]
            // v(n+1) = v(n) + a(n+1)dt

            for (auto& o : objects)
            {
                // update position
                o.pos += o.vel * dt + o.acc * dt * dt * 0.5f;

                // update accelerations
                sf::Vector2f tacc = 0.5f * (o.acc + gForce / o.mass) * dt;
                o.acc = gForce / o.mass;
                
                // update velocity
                o.vel += tacc * dt;
            }
        }
        
        void updatecollision() {
            
            cneighbours = 0; // collision tracker

            for(int i = 0; i < objects.size(); i++) {
                std::vector<int> nearestNeighbours = objectTree.radiusSearch(objects[i], objects[i].radius);
                
                for(auto j : nearestNeighbours) {
                    // process the colliding pair (i,j) only once
                    if(i < j) {
                        handlecollision(i,j);
                    }
                }
            }

            if(cneighbours < cnBins.size())
                    cnBins[cneighbours]++;
        }

        worldspace(float width, float height) : 
            mW{width}, mH{height}, cneighbours{0} {
                for(unsigned i{16}; i--;)
                    cnBins.push_back(0);
        }

        void create(sf::Vector2f&& pos, float radius, float mass) {
            objects.emplace_back(obj{pos, sf::Vector2f{0,0}, sf::Vector2f{0,0}, radius, mass});
        }
        
        obj& operator[](unsigned i) { return objects[i]; }
        
        unsigned objs() { return objects.size(); }
        
        void clear() {
            objects.clear();
        }

        float w() { return mW; }
        void w(float value) { mW = value; }
        float h() { return mH; }
        void h(float value) { mH = value; }
    };
}

#endif //spheco_worldspace_hpp