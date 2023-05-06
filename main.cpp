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

    sf::Texture characterTexture;
    if (!characterTexture.loadFromFile("tank.png")) {
        std::cerr << "Error loading character texture" << std::endl;
        return 1;
    }

    sf::Sprite character;
    sf::Vector2u textureSize = characterTexture.getSize();
    character.setTexture(characterTexture);
    character.setScale(.25, .25);
    character.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);

    // Create a rectangle shape for the character
    // sf::RectangleShape character(sf::Vector2f(50.0f, 50.0f));
    // character.setFillColor(sf::Color::Red);
    // character.setPosition(295.0f, 430.0f);

    // Set the movement speed of the character
    float speed = 600.0f;

    // Create a rectangle shape for the enemy
    sf::RectangleShape enemy(sf::Vector2f(50.0f, 50.0f));
    enemy.setFillColor(sf::Color::Green);
    enemy.setPosition(295.0f, 50.0f);

    // Create a circle shape for the projectiles
    std::vector<sf::CircleShape> projectiles;
    std::vector<sf::Vector2f> directions;

    std::vector<sf::CircleShape> playerProjectiles;
    float playerProjectileSpeed = 500.0f;
    const int maxPlayerProjectiles = 4;

    sf::CircleShape playerProjectile(5.0f);
    playerProjectile.setFillColor(sf::Color::Green);

    float projectileSpeed = 500.0f;
    bool playerProjectileActive = false;

    // Add a shooting clock and interval
    sf::Clock shootingClock;
    float shootingInterval =
        0.1f;  // Time interval between each shot in seconds

    // Time interval between spawning projectiles
    float spawnInterval = 1.0f;
    sf::Clock spawnClock;

    // Calculate delta time for consistent movement speed
    sf::Clock clock;
    sf::Time lastTime = clock.getElapsedTime();

    // Display the character, projectiles, and enemy
    bool gameOver = false;

    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Replace the spacebar key-press handling code with this
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
            playerProjectiles.size() < maxPlayerProjectiles &&
            shootingClock.getElapsedTime().asSeconds() > shootingInterval) {
            sf::CircleShape newPlayerProjectile(5.0f);
            newPlayerProjectile.setFillColor(sf::Color::Green);
            newPlayerProjectile.setPosition(character.getPosition());
            newPlayerProjectile.setRotation(character.getRotation());
            playerProjectiles.push_back(newPlayerProjectile);
            shootingClock.restart();
        }

        sf::Time currentTime = clock.getElapsedTime();
        sf::Time deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Move the player's projectiles
        for (size_t i = 0; i < playerProjectiles.size(); ++i) {
            float angleRad =
                playerProjectiles[i].getRotation() * 3.14159265f / 180.f;
            playerProjectiles[i].move(
                -playerProjectileSpeed * deltaTime.asSeconds() *
                    std::cos(angleRad),
                -playerProjectileSpeed * deltaTime.asSeconds() *
                    std::sin(angleRad));

            // Remove the projectile if it goes off the screen
            if (playerProjectiles[i].getPosition().x < 0 ||
                playerProjectiles[i].getPosition().x > WIDTH ||
                playerProjectiles[i].getPosition().y < 0 ||
                playerProjectiles[i].getPosition().y > HEIGHT) {
                playerProjectiles.erase(playerProjectiles.begin() + i);
                --i;
            }
        }

        // Reset the player's projectile if it goes off the screen
        if (playerProjectile.getPosition().x < 0 ||
            playerProjectile.getPosition().x > WIDTH ||
            playerProjectile.getPosition().y < 0 ||
            playerProjectile.getPosition().y > HEIGHT) {
            playerProjectileActive = false;
        }

        for (size_t i = 0; i < playerProjectiles.size(); i++) {
            if (enemy.getGlobalBounds().intersects(
                    playerProjectiles[i].getGlobalBounds())) {
                // Remove the projectile
                playerProjectiles.erase(playerProjectiles.begin() + i);
                --i;

                // Set the game over flag
                gameOver = true;
                break;
            }
        }

        // Spawn a new projectile if enough time has passed
        if (spawnClock.getElapsedTime().asSeconds() > spawnInterval) {
            //... (previous code for spawning enemy projectiles)
        }

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

        for (size_t i = 0; i < projectiles.size(); i++) {
            if (character.getGlobalBounds().intersects(
                    projectiles[i].getGlobalBounds())) {
                // (existing code)

                // Set the game over flag
                gameOver = true;
                break;
            }
        }

        // Handle keyboard input

        float targetRotation = character.getRotation();

        bool moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        bool moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        bool moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        bool moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

        if (moveUp && moveLeft) {
            targetRotation = 45;
            character.move(-speed * deltaTime.asSeconds() / sqrt(2),
                           -speed * deltaTime.asSeconds() / sqrt(2));
        } else if (moveUp && moveRight) {
            targetRotation = 135;
            character.move(speed * deltaTime.asSeconds() / sqrt(2),
                           -speed * deltaTime.asSeconds() / sqrt(2));
        } else if (moveDown && moveLeft) {
            targetRotation = 315;
            character.move(-speed * deltaTime.asSeconds() / sqrt(2),
                           speed * deltaTime.asSeconds() / sqrt(2));
        } else if (moveDown && moveRight) {
            targetRotation = 225;
            character.move(speed * deltaTime.asSeconds() / sqrt(2),
                           speed * deltaTime.asSeconds() / sqrt(2));
        } else if (moveUp) {
            targetRotation = 90;
            character.move(0.0f, -speed * deltaTime.asSeconds());
        } else if (moveDown) {
            targetRotation = 270;
            character.move(0.0f, speed * deltaTime.asSeconds());
        } else if (moveLeft) {
            targetRotation = 0;
            character.move(-speed * deltaTime.asSeconds(), 0.0f);
        } else if (moveRight) {
            targetRotation = 180;
            character.move(speed * deltaTime.asSeconds(), 0.0f);
        }

        character.setRotation(targetRotation);

        character.setRotation(targetRotation);

        // Update the counter display
        counterText.setString("Projectiles stopped: " +
                              std::to_string(counter));

        if (gameOver) {
            // Close the window
            window.close();
        }
        // Clear the window
        window.clear(sf::Color::White);

        // Draw the player's projectiles
        for (const auto &playerProjectile : playerProjectiles) {
            window.draw(playerProjectile);
        }

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