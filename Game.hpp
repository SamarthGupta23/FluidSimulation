#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <vector>

class Game {
public:
    Game();  // Constructor
    void run();

private:
    void processEvents();
    void update();
    void render();

    // Variables
    sf::RenderWindow mWindow;
    void initVariables();
    sf::Texture water;
    std::vector<sf::Sprite> particle;
    std::vector<sf::Vector2f> positions;
    std::vector<sf::Vector2f> velocities;
    std::vector<std::vector<std::vector<sf::Vector2f>>> grid;
    std::vector<std::vector<std::vector<sf::Vector2f>>> velocityGrid;
    std::vector<std::vector<std::vector<sf::Vector2f>>> pressureForceGrid;
    float smoothingRadius;
    float pressureMultiplier;
    float targetDensity;
    float e; // Coefficient of restitution
    int mass;
    sf::Vector2f gravity;
    float tim; // Time step
    int numberParticles;

    // Functions
    float calculateDensity(sf::Vector2f);
    float calculateDistance(sf::Vector2f pos1, sf::Vector2f pos2);
    sf::Vector2f calculateSmoothingDerivative(sf::Vector2f iteratePos, sf::Vector2f particlePos);
    float calculatePressure(sf::Vector2f);
};
