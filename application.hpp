#ifndef spheco_window_hpp
#define spheco_window_hpp

#include <string>
#include <sstream>
#include <exception>
#include <chrono>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "worldspace.hpp"

namespace spheco {
    class Application {
        unsigned w,h;

        sf::RenderWindow window;
        bool backg; // window set to background => advance physics, stop rendering
        bool click;
        float fps, phtime, ctime, gtime, gotime;
        sf::Font font; 

        spheco::worldspace world;
        float radius, dt;
        unsigned substeps;
        std::vector<sf::CircleShape> shapes;

        void onmouseclicked() {
            std::ofstream f("worlddata.csv");
            
            f << "general world info\nobjs, width, height, radii, fps, physics[ms], collision[ms], grid ops[ms], graphics[ms]\n";
            f << world.objs() << ", "
                << world.getgrid().getw() << ", "
                << world.getgrid().geth() << ", "
                << world[0].radius << ", "
                << fps << ", "
                << phtime << ", "
                << ctime << ", "
                << gotime << ", "
                << gtime << "\n\n";
            
            f << "bin info\ngrid x grid y, #objects present (omitted if 0 or 1)\n";
            for(size_t x{world.getgrid().getw()}; x--;)
                for(size_t y{world.getgrid().geth()}; y--;)
                    if(world[sf::Vector2u{(unsigned)x, (unsigned)y}].size() > 1)
                        f << x << ", " << y << ", " << world[sf::Vector2u{(unsigned)x, (unsigned)y}].size() << "\n";

            f << "obj info\nx, y, grid x grid y\n";
            for(size_t i{world.objs()}; i--;)
                    f << world[i].pos.x << ", " 
                        << world[i].pos.y << ", " 
                        << world[i].gridpt.x << ", " 
                        << world[i].gridpt.y << "\n";

            f.flush();
            f.close();
        }
        void handleevents() {
            sf::Event event;
            while (window.pollEvent(event))
            {
                switch(event.type) {
                    case sf::Event::MouseButtonPressed:
                        click = true;
                        break;
                    case sf::Event::MouseButtonReleased:
                        click = false;
                        break;
                    case sf::Event::LostFocus:
                        backg = true;
                        break;
                    case sf::Event::GainedFocus:
                        backg = false;
                        break;
                    case sf::Event::KeyPressed:
                        // escape just passes through to closed event
                        if(event.key.code != sf::Keyboard::Escape)
                            break;
                    case sf::Event::Closed:
                        window.close();
                        break;
                    default: break;
                }
            }

            if(click) onmouseclicked();
        }
        void handlespawns() {
            if(fps > 59)
                for(int _{40}; _--;) { // get fucked
                unsigned i = world.objs() % 1000;
                unsigned j = world.objs() / 1000;
                auto tpos = sf::Vector2f{100, 58.6f} + (float)i * sf::Vector2f{0.001,0.7} + (float)j * sf::Vector2f{60,0}; 
                world.create(tpos, sf::Vector2f{0,0}, radius, 1);

                sf::CircleShape shape(world[i].radius);
                shape.setFillColor(sf::Color(100 + world.objs(), 250 + world.objs(), 50 + world.objs()));
                shape.setPosition(world[i].pos);
                shapes.push_back(shape);
            }
        }
        void showfps() {
            std::stringstream s;
            s   << "objs: " << world.objs() << "\n"
                << "fps: " << fps << "\n"
                << "physics: "  << phtime << " ms\n"
                << "collision: " << ctime << " ms\n"
                << "grid ops: " << gotime << " ms\n"
                << "graphics: " << gtime << " ms"
                ;
            sf::Text fpstext(s.str(), font, 12);
            window.draw(fpstext);
        }
        void updategraphics() {
            if(backg)
                return;
            window.clear(sf::Color::Black);
            
            for(size_t i{world.objs()}; i--;) {
                shapes[i].setPosition(world[i].pos);
                window.draw(shapes[i]);
            }

            showfps();
            window.display();
        }
    public:
        Application(const std::string& windowtitle, unsigned width, unsigned height, float dt, unsigned substeps, float radius, float mass) : 
            w{width}, h{height},
            window(sf::VideoMode(w, h), windowtitle),
            backg{false}, click{false}, fps{0}, phtime{0}, ctime{0}, gtime{0}, gotime{0}, 
            world(radius, radius, (unsigned)(width/radius), (unsigned)(height/radius)),
            radius{radius}, dt{dt}, substeps{substeps}
            {
                window.setVerticalSyncEnabled(true);
                window.setActive(true);
                if(!font.loadFromFile("resources/tuffy.ttf")) 
                    throw Exception("could not load resources/tuffy.ttf");
            }
        void run() {
            sf::Int64 dt = 0;
            while (window.isOpen())
            {
                sf::Clock clock;
                handleevents();
                handlespawns();
                sf::Int64 inittime = clock.restart().asMicroseconds();

                sf::Int64 ph0{0}, c0{0}, go0{0};
                for(unsigned i{substeps}; i--;) {
                    world.updatecoords(dt/substeps);
                    ph0 += clock.restart().asMicroseconds();
                    world.updatecollision();
                    c0 += clock.restart().asMicroseconds();
                    world.updategrid();
                    go0 += clock.restart().asMicroseconds();
                }

                updategraphics();
                sf::Int64 g0 = clock.getElapsedTime().asMicroseconds();
                
                sf::Int64 dt_cf = inittime + ph0 + c0 + go0 + g0;
                dt += dt_cf;
                if(dt > 100'000) { 
                    dt = 0;
                    fps = (float)1e6 / (float)dt_cf;
                    phtime = ph0 / 1'000;
                    ctime = c0 / 1'000;
                    gotime = go0 / 1'000;
                    gtime = g0 / 1'000;
                }

            }
        }
    };
}

/*
NOTES
    profiling -> launch.json
        ,
        "postDebugTask": "gprof gmon.out"
    Mouse position
        // get the global mouse position (relative to the desktop)
        sf::Vector2i globalPosition = sf::Mouse::getPosition();
        // get the local mouse position (relative to a window)
        sf::Vector2i localPosition = sf::Mouse::getPosition(window); // window is a sf::Window
    
    opengl in windows
        https://www.sfml-dev.org/tutorials/2.5/window-opengl.php

    multi threaded drawing
    drawing to texture
        https://www.sfml-dev.org/tutorials/2.5/graphics-draw.php

    anti aliasing
        sf::ContextSettings settings;
        settings.antialiasingLevel = 8;
        sf::RenderWindow window(sf::VideoMode(800, 600), "SFML shapes", sf::Style::Default, settings);
*/

#endif //spheco_window_hpp