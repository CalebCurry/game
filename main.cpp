#include <tgmath.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
int main() {
    // Create a window
    const int WIDTH = 1280;
    const int HEIGHT = 720;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML Window");

    // Load a font
    sf::Font font;
    if (!font.loadFromFile("Roboto-Regular.ttf")) {
        // Error handling if the font is not found
        std::cerr << "Error loading font" << std::endl;
        return 1;
    }

    // Create a text object for the counter display
    sf::Text counterText;
    counterText.setFont(font);
    counterText.setCharacterSize(24);
    counterText.setFillColor(sf::Color::Black);
    counterText.setPosition(10.0f, 10.0f);

    // Initialize the counter
    unsigned int counter = 0;

    // Create a rectangle shape for the character
    sf::RectangleShape character(sf::Vector2f(50.0f, 50.0f));
    character.setFillColor(sf::Color::Red);
    character.setPosition(295.0f, 430.0f);

    // Set the movement speed of the character
    float speed = 600.0f;

    // Create a rectangle shape for the enemy
    sf::RectangleShape enemy(sf::Vector2f(50.0f, 50.0f));
    enemy.setFillColor(sf::Color::Green);
    enemy.setPosition(295.0f, 50.0f);

    // Create a circle shape for the projectiles
    std::vector<sf::CircleShape> projectiles;
    std::vector<sf::Vector2f> directions;

    // Set the movement speed of the projectiles
    float projectileSpeed = 300.0f;

    // Time interval between spawning projectiles
    float spawnInterval = 1.0f;
    sf::Clock spawnClock;

    // Calculate delta time for consistent movement speed
    sf::Clock clock;
    sf::Time lastTime = clock.getElapsedTime();

    // Display the character, projectiles, and enemy
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Calculate delta time for consistent movement speed
        sf::Time currentTime = clock.getElapsedTime();
        sf::Time deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Spawn a new projectile if enough time has passed
        if (spawnClock.getElapsedTime().asSeconds() > spawnInterval) {
            sf::CircleShape newProjectile(10.0f);
            newProjectile.setFillColor(sf::Color::Blue);
            newProjectile.setPosition(
                enemy.getPosition().x + enemy.getSize().x / 2,
                enemy.getPosition().y + enemy.getSize().y);

            // Calculate the direction to shoot the projectile
            sf::Vector2f direction =
                character.getPosition() - newProjectile.getPosition();
            float distance =
                sqrt(direction.x * direction.x + direction.y * direction.y);
            direction.x /= distance;
            direction.y /= distance;

            projectiles.push_back(newProjectile);
            directions.push_back(direction);
            spawnClock.restart();
        }

        // Move the projectiles
        for (size_t i = 0; i < projectiles.size(); i++) {
            projectiles[i].move(directions[i] * projectileSpeed *
                                deltaTime.asSeconds());
        }

        // Check for collision between the character and the projectiles
        for (size_t i = 0; i < projectiles.size(); i++) {
            if (character.getGlobalBounds().intersects(
                    projectiles[i].getGlobalBounds())) {
                // Reset the projectile position
                projectiles.erase(projectiles.begin() + i);
                directions.erase(directions.begin() + i);

                // Increment the counter
                counter++;

                // Break out of the loop to prevent invalid access
                break;
            }
        }

        // Handle keyboard input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            character.move(0.0f, -speed * deltaTime.asSeconds());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            character.move(0.0f, speed * deltaTime.asSeconds());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            character.move(-speed * deltaTime.asSeconds(), 0.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            character.move(speed * deltaTime.asSeconds(), 0.0f);
        }

        // Update the counter display
        counterText.setString("Projectiles stopped: " +
                              std::to_string(counter));

        // Clear the window
        window.clear(sf::Color::White);

        // Draw the character, projectiles, enemy, and counter text
        window.draw(character);
        for (const auto &projectile : projectiles) {
            window.draw(projectile);
        }
        window.draw(enemy);
        window.draw(counterText);

        // Display the window
        window.display();
    }

    return 0;
}