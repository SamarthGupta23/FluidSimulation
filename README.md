# Fluid Simulation Using Smoothed Particle Hydrodynamics (SPH)

This project implements a 2D fluid simulation using Smoothed Particle Hydrodynamics (SPH) principles in C++ with SFML. The simulation showcases fluid particle interactions based on physical properties like density, pressure, and velocity, offering a realistic representation of fluid behavior.

## Features
- **Particle-Based Simulation:** Models fluid as a collection of particles with SPH algorithms for density and pressure calculations.
- **Grid-Based Optimization:** Divides the simulation space into a grid to accelerate neighbor searches, improving computational efficiency.
- **Interactive Event Handling:** Click on the simulation window to view particle density and velocity at specific positions.
- **Texture-Based Rendering:** Each particle is rendered using a semi-transparent water droplet texture for enhanced visuals.
- **Realistic Physics:** Includes pressure forces, gravitational effects, and collision handling with restitution for accurate particle dynamics.

## Future Enhancements
- Adding dynamic obstacles.
- Introducing particle spawning and removal at runtime.
- Visual upgrades with shaders or advanced lighting effects.

This project serves as an excellent foundation for fluid simulations in games or educational physics applications.
