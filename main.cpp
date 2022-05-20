#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cmath>
#include <queue>

using namespace std;

const double G = 6.6753015e-11;

int limit_size = 10000;

struct Planet2D
{
    long id;
    double mass;
    double phy_x, phy_y;
    pair<double, double> velocity;
    pair<double, double> acceleration;
};

pair<double, double> calculate_acceleration(Planet2D& current, vector<Planet2D>& planets)
{
    pair<double, double> cur_a = {0.0, 0.0};
    for (Planet2D planet : planets)
    {
        if (planet.id != current.id)
        {
            double dist = max(5000000.0, sqrt(pow(current.phy_x - planet.phy_x, 2) + pow(current.phy_y - planet.phy_y, 2)));
            double f = G * planet.mass * current.mass / pow(dist, 2);
            double sin_f = (planet.phy_x - current.phy_x) / dist;
            double cos_f = (planet.phy_y - current.phy_y) / dist;

            pair<double, double> f_vector = {f * sin_f, f * cos_f};

            cur_a.first += f_vector.first / current.mass;
            cur_a.second += f_vector.second / current.mass;
        }
    }

    return cur_a;
}

pair<double, double> calculate_velocity(Planet2D& planet, double delta_time)
{
    return {planet.velocity.first + planet.acceleration.first * delta_time,
            planet.velocity.second + planet.acceleration.second * delta_time};
}

void update_ui(vector<Planet2D>& planets, vector<sf::CircleShape>& gui_planets, sf::RenderWindow& window, double x, int size, bool log, deque<pair<float, float>> trace)
{
        for (int i = 0; i < planets.size(); i++)
        {
            gui_planets[i].setPosition(planets[i].phy_x / x, planets[i].phy_y / x);
            window.draw(gui_planets[i]);

            if (i == 0)
            {
                if (trace.size() > limit_size)
                {
                    trace.pop_front();
                    trace.emplace_back(planets[i].phy_x / x, planets[i].phy_y / x);
                }
                for (auto p1 : trace)
                {
                    sf::CircleShape shape;
                    shape.setRadius(5);
                    shape.setPosition(p1.first, p1.second);
                    shape.setFillColor(sf::Color::Cyan);
                    window.draw(shape);
                }

            }
        }
}

void simulation_iteration(vector<Planet2D>& planets, double delta_time, bool log)
{
    for (int i = 0; i < planets.size(); i++)
    {
        planets[i].acceleration = calculate_acceleration(planets[i], planets);
        planets[i].velocity = calculate_velocity(planets[i], delta_time);
        planets[i].phy_x += planets[i].velocity.first * delta_time;
        planets[i].phy_y += planets[i].velocity.second * delta_time;

        if (log)
        {
            cout << "Planet #" << i << ", X = " << planets[i].phy_x << ", Y = " << planets[i].phy_y << ", velocity: "
                 << planets[i].velocity.first << ", " << planets[i].velocity.second << ", acceleration: " << planets[i].acceleration.first << ", " << planets[i].acceleration.second << endl;
        }
    }
    if (log)
    {
        cout << "############################" << endl;
    }
}

int main()
{
    srand( (unsigned)time( NULL ) );
    int size = 2000;
    long int physics_size = 1000000000;
    int N = 2;
    double initial_velocity = 300000;
    double x_time = 1000;
    double x = (double) physics_size / size;

    vector<Planet2D> planets;
    vector<sf::CircleShape> gui_planets;
    deque<pair<float, float>> trace;
    for (int i = 0; i < N; i++)
    {
        Planet2D planet;
        planet.id = i;
        planet.mass = 5.972E26;
        planet.velocity = {(((float) rand() / RAND_MAX) - 0.5) * initial_velocity, (((float) rand() / RAND_MAX) - 0.5) * initial_velocity};
        if (i == 0)
        {
            planet.phy_x = 2.53012e+08;
            planet.phy_y = 2.62842e+08;

        }
        if (i == 1)
        {
            planet.phy_x = 4.78754e+08;
            planet.phy_y = 3.2458e+08;
            planet.velocity = {0.0, 0.0};
            planet.mass = 5.972E28;
        }
        if (i == 1)
        {
            planet.phy_x = 5.78754e+08;
            planet.phy_y = 5.2458e+08;
        }
//        planet.phy_x = ((float) rand() / RAND_MAX) * physics_size;
//        planet.phy_y = ((float) rand() / RAND_MAX) * physics_size;
//        planet.velocity = {0, 0};
        planet.acceleration = {0, 0};
        planets.push_back(planet);

        sf::CircleShape shape;
        shape.setRadius(10.f);
        shape.setPosition(planet.phy_x / x, planet.phy_y / x);
        shape.setFillColor(sf::Color::Cyan);
        gui_planets.push_back(shape);
    }

    sf::RenderWindow window(sf::VideoMode(size, size), "Planets Simulation");


    clock_t prev_time = clock() - CLOCKS_PER_SEC;
    clock_t prev_time_log = clock() - CLOCKS_PER_SEC - 1;
    while (window.isOpen())
    {
        bool log = clock () - prev_time_log > CLOCKS_PER_SEC / 10;
        if (log)
        {
            prev_time_log = clock();
        }

        simulation_iteration(planets, x_time * float( clock () - prev_time ) /  CLOCKS_PER_SEC, log);
        prev_time = clock();
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
//        int i = 0;
//        gui_planets[i].setPosition(planets[i].phy_x / x, planets[i].phy_y / x);
//        window.draw(gui_planets[i]);
        update_ui(planets, gui_planets, window, x, size, log, trace);
        window.display();
    }
}
