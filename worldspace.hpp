#ifndef spheco_worldspace_hpp
#define spheco_worldspace_hpp

#include "obj.hpp"
#include "util.hpp"
#include "grid.hpp"

#include <SFML/Graphics.hpp>

#include <vector>
#include <cmath>
#include <algorithm>

namespace spheco {

    class worldspace
    {
        float w, h;
        unsigned subdivx, subdivy;
        unsigned cneighbours;
        std::vector<unsigned> cnbin;
        std::vector<obj> objects;               // stores all objects. everything else is done with indices appended to that list
        grid igrid; // {x,y} -> [{i at x,y}, {j at x,y}, ....]
        std::vector<unsigned> transfer;
        std::vector<sf::Vector2f> forces; // emphasis on constant forces
        sf::Vector2u gridpt(obj &o, float w, float h) { return sf::Vector2u{(unsigned)(o.pos.x / w), (unsigned)(o.pos.y / h)}; }
        void updategpt(const unsigned& i) {
            sf::Vector2u npos = gridpt(objects[i], w, h);
            if( npos.x >= subdivx || npos.y >= subdivy)
                return;
            if(npos != objects[i].gridpt) 
                transfer.push_back(i);
        }
    public:
        void updatecoords(float dt)
        {
            // verlet integration
            // x(n+1) = x(n) + v(n)dt + a(n)dt^2 / 2
            // update a(n)[x,v] -> a(n+1)[x,v]
            // v(n+1) = v(n) + a(n+1)dt
            sf::Vector2f f{0,0};
            for(auto& force : forces)
                f += force;

            for (auto& o : objects)
            {
                // update position
                o.pos += o.vel * dt + o.acc * dt * dt * 0.5f;

                // update accelerations
                sf::Vector2f tacc = 0.5f * (o.acc + f / o.mass) * dt;
                o.acc = f / o.mass;
                
                // update velocity
                o.vel += tacc * dt;
            }
        }
        void handlecollision(const unsigned& i, const unsigned& j)
        {
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
        void updatecollision()
        {
            // for every grid cell
            for(unsigned i = 0; i < objects.size(); i++) {
                //nobody here to collide with
                if(igrid[objects[i].gridpt].size() == 0)
                    continue;

                cneighbours = 0;
                // for every neighbouring cell
                for (auto dx{-1}; dx <= 1; dx++)
                    for (auto dy{-1}; dy <= 1; dy++) {
                        sf::Vector2u npos{objects[i].gridpt.x + dx, objects[i].gridpt.y + dy};

                        //no neighbour here for gpos to collide with
                        if( npos.x < 0 || npos.x >= subdivx || 
                            npos.y < 0 || npos.y >= subdivy)
                            continue; 
                        if(igrid[npos].size() == 0)
                            continue;
                        // for every local and neighbouring cell
                        for(auto& j : igrid[npos])
                            if(i < j) // which are not identical and only processed once
                                handlecollision(i,j);
                    }

                if(cneighbours < cnbin.size())
                    cnbin[cneighbours]++;
            }
        }
        void updategrid() {
            for(size_t i{objects.size()}; i--;)
                updategpt(i);
            
            for(auto& obji : transfer) {
                auto frompos = objects[obji].gridpt;
                size_t index2del = std::index_of(igrid[frompos].begin(), igrid[frompos].end(), obji);
                if(index2del < igrid[frompos].size())
                    igrid[frompos].erase(igrid[frompos].begin() + index2del);
                
                auto newpos = gridpt(objects[obji], w, h); 
                if( newpos.x >= subdivx || newpos.y >= subdivy)
                    continue;

                igrid[newpos].push_back(obji);
                objects[obji].gridpt = newpos;
            }
            transfer.clear();
        }

        worldspace(float width, float height, unsigned subdivx, unsigned subdivy) : 
            w{width}, h{height}, subdivx{subdivx}, subdivy{subdivy}, cneighbours{0}, igrid(subdivx, subdivy) { 
                objects.reserve(5000);
                for(unsigned i{16}; i--;)
                    cnbin.push_back(0);
        }
        void create(const sf::Vector2f& pos, const sf::Vector2f& vel, const float& radius, const float& mass)
        {
            const sf::Vector2f f = sf::Vector2f{0,0};
            const sf::Vector2u u = sf::Vector2u{0,0};
            const float s = (float)objects.size();
            objects.push_back(obj{pos, vel, f, u, radius, s}); // mass
            objects.back().gridpt = gridpt(objects.back(), w, h);
            
            objects.back().gridpt.x = std::min(subdivx, objects.back().gridpt.x);
            objects.back().gridpt.y = std::min(subdivx, objects.back().gridpt.y);

            igrid[objects.back().gridpt].push_back(objects.size()-1);


            for(unsigned i{16}; i--;)
                std::cout << i << ": " << cnbin[i] << std::endl;
        }
        obj& operator[](unsigned i) { return objects[i]; }
        unsigned objs() { return objects.size(); }
        void clear() { 
            objects.clear(); 
            for(unsigned i = 0; i < igrid.getw(); i++)
                for(unsigned j = 0; j < igrid.geth(); j++)
                    igrid[sf::Vector2u{i,j}].clear();
        }
        std::vector<unsigned>& operator[](const sf::Vector2u& gn) { return igrid[gn]; }
        const grid& getgrid() const { return igrid; }
    };
}

#endif //spheco_worldspace_hpp