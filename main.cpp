#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

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

    // Create a circle shape for the projectiles
    sf::CircleShape projectile(10.0f);
    projectile.setFillColor(sf::Color::Blue);

    // Set the movement speed of the projectiles
    float projectileSpeed = 300.0f;

    // Random number generator for the projectile's starting position
    std::srand(std::time(nullptr));
    int startX = std::rand() % WIDTH;

    projectile.setPosition(startX, 0.0f);

    // Calculate delta time for consistent movement speed
    sf::Clock clock;
    sf::Time lastTime = clock.getElapsedTime();

    // Display the character and projectiles
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

        // Move the projectiles from top to bottom
        projectile.move(0.0f, projectileSpeed * deltaTime.asSeconds());

        // Reset the projectile position if it goes off the bottom edge of the
        // screen
        if (projectile.getPosition().y > HEIGHT) {
            int startX = std::rand() % WIDTH;
            projectile.setPosition(startX, 0.0f);
        }

        // Check for collision between the character and the projectile
        if (character.getGlobalBounds().intersects(
                projectile.getGlobalBounds())) {
            // Reset the projectile position
            int startX = std::rand() % WIDTH;
            projectile.setPosition(startX, 0.0f);

            // Increment the counter
            counter++;
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

        // Draw the character, projectiles, and counter text
        window.draw(character);
        window.draw(projectile);
        window.draw(counterText);

        // Display the window
        window.display();
    }

    return 0;
}
