#include "Game.hpp"
#include <random>
#include <iostream>
#include <cmath>
#include <algorithm> // Keep this for other algorithm functionalities

// Constructor
Game::Game()
    : mWindow(sf::VideoMode(1600, 900), "Fluid Simulation")
{
    this->initVariables();
}

// Game Loop
void Game::run()
{
    while (mWindow.isOpen()) {
        processEvents();
        update();
        render();
    }
}

// Event Handling
void Game::processEvents()
{
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            mWindow.close();
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            std::cout << "Density at (" << mousePos.x << ", " << mousePos.y << ") = " << calculateDensity(mousePos) << std::endl;
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            for (int i = 0; i < numberParticles; i++) {
                if (particle[i].getGlobalBounds().contains(mousePos)) {
                    std::cout << "veclotity vector is:" << velocities[i].x << " " << velocities[i].y << std::endl;
                }
            }
        }
    }
}

// Update Logic
void Game::update()
{
    // Populate grid
    //predict positions
    for (int i = 0; i < numberParticles; i++) {
        int gridX = (positions[i].x / 100 < 0) ? 0 : ((positions[i].x / 100 > 15) ? 15 : static_cast<int>(positions[i].x / 100));
        int gridY = (positions[i].y / 100 < 0) ? 0 : ((positions[i].y / 100 > 8) ? 8 : static_cast<int>(positions[i].y / 100));

        grid[gridX][gridY].push_back(positions[i]);
        velocityGrid[gridX][gridY].push_back(velocities[i]);
        pressureForceGrid[gridX][gridY].push_back(sf::Vector2f(0.f, 0.f));
    }

    // Calculate pressure forces
    for (int row = 0; row < 16; row++) {
        for (int column = 0; column < 9; column++) {
            for (int i = 0; i < grid[row][column].size(); i++) {
                float particlePressure = calculatePressure(grid[row][column][i]);
                sf::Vector2f summable = sf::Vector2f(0.f, 0.f);

                for (int compareRow = std::max(0, row - 1); compareRow <= std::min(15, row + 1); compareRow++) {
                    for (int compareColumn = std::max(0, column - 1); compareColumn <= std::min(8, column + 1); compareColumn++) {
                        for (int j = 0; j < grid[compareRow][compareColumn].size(); j++) {
                            float comparePressure = calculatePressure(grid[compareRow][compareColumn][j]);
                            float neighborDensity = calculateDensity(grid[compareRow][compareColumn][j]);

                            if (neighborDensity > 0) {
                                summable += mass * (particlePressure + comparePressure) *
                                    calculateSmoothingDerivative(grid[compareRow][compareColumn][j], grid[row][column][i]) /
                                    (2 * neighborDensity);
                            }
                        }
                    }
                }
                pressureForceGrid[row][column][i] = -1.f * summable;
            }
        }
    }

    // Update positions and velocities
    int index = 0;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 9; j++) {
            for (int k = 0; k < grid[i][j].size(); k++) {
                positions[index] = grid[i][j][k];
                velocities[index] += calculateDensity(positions[index]) * pressureForceGrid[i][j][k] * tim;
                index++;
            }
        }
    }

    // Apply gravity and handle collisions
    for (int i = 0; i < numberParticles; i++) {
        velocities[i] += gravity * tim;

        // Wall collisions
        if (positions[i].y >= 900.f - 15.f && velocities[i].y > 0)
            velocities[i].y *= -1.f * e; // Reverse Y velocity on collision
        if (positions[i].y <= 15.f && velocities[i].y < 0)
            velocities[i].y *= -1.f * e;

        if (positions[i].x >= 1600.f - 15.f && velocities[i].x > 0)
            velocities[i].x *= -1.f * e; // Reverse X velocity on collision
        if (positions[i].x <= 15.f && velocities[i].x < 0)
            velocities[i].x *= -1.f * e;

        // Update position
        positions[i] += velocities[i] * tim;
        particle[i].setPosition(positions[i]);
    }



    // Clear grid
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 9; j++) {
            grid[i][j].clear();
            velocityGrid[i][j].clear();
            pressureForceGrid[i][j].clear();
        }
    }
}

// Render Logic
void Game::render()
{
    mWindow.clear();
    for (const auto& p : particle) {
        mWindow.draw(p);
    }
    mWindow.display();
}

// Initialization
void Game::initVariables()
{
    mWindow.setFramerateLimit(30);
    numberParticles = 100;
    mass = 1;
    gravity = sf::Vector2f(0.f, 0.f); // No gravity for now
    tim = 1.f / 30.f;
    e = 0.4f; // Restitution
    smoothingRadius = 7.f * 10.f;
    pressureMultiplier = 40000.f;
    targetDensity = 2.76f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disx(1400.f, 1590.f);
    std::uniform_real_distribution<float> disy(50.f, 890.f);

    particle.resize(numberParticles);
    positions.reserve(numberParticles);
    velocities.reserve(numberParticles);
    this->water.loadFromFile("C:/Users/samar/OneDrive/Desktop/Game Development/Simulations/Fluid Simulation/water.png");
    for (int i = 0; i < numberParticles; i++) {
        sf::Vector2f position(disx(gen), disy(gen));
        sf::Vector2f velocity(0.f, 0.f);

        positions.push_back(position);
        velocities.push_back(velocity);

        particle[i].setTexture(water);
        particle[i].setPosition(position);
    }

    grid.resize(16, std::vector<std::vector<sf::Vector2f>>(9));
    velocityGrid.resize(16, std::vector<std::vector<sf::Vector2f>>(9));
    pressureForceGrid.resize(16, std::vector<std::vector<sf::Vector2f>>(9));
}

// Density Calculation
float Game::calculateDensity(sf::Vector2f pos)
{
    float density = 0.f;
    for (int i = 0; i < numberParticles; i++) {
        float distance = calculateDistance(pos, positions[i]);
        if (distance < smoothingRadius) {
            density += mass * std::pow(smoothingRadius - distance, 3) * 4 / std::pow(smoothingRadius, 4);
        }
    }
    return density;
}

// Distance Calculation
float Game::calculateDistance(sf::Vector2f pos1, sf::Vector2f pos2)
{
    return std::sqrt(std::pow(pos1.x - pos2.x, 2) + std::pow(pos1.y - pos2.y, 2));
}

// Smoothing Derivative
sf::Vector2f Game::calculateSmoothingDerivative(sf::Vector2f iteratePos, sf::Vector2f particlePos)
{
    float distance = calculateDistance(iteratePos, particlePos);
    if (distance == 0 || distance > smoothingRadius) {
        return sf::Vector2f(0.f, 0.f);
    }
    float factor = -12.f * std::pow(smoothingRadius - distance, 2) / std::pow(smoothingRadius, 4);
    return factor * (iteratePos - particlePos) / distance;
}

// Pressure Calculation
float Game::calculatePressure(sf::Vector2f pos)
{
    return pressureMultiplier * (calculateDensity(pos) - targetDensity);
}
