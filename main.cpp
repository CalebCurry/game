#include <SFML/Graphics.hpp>

int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Window");

    // Create a rectangle shape for the character
    sf::RectangleShape character(sf::Vector2f(50.0f, 50.0f));
    character.setFillColor(sf::Color::Red);
    character.setPosition(100.0f, 100.0f);

    // Set the movement speed of the character
    float speed = 200.0f;

    // Calculate delta time for consistent movement speed
    sf::Clock clock;
    sf::Time lastTime = clock.getElapsedTime();

    // Display the character
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

        // Clear the window
        window.clear(sf::Color::White);

        // Draw the character
        window.draw(character);

        // Display the window
        window.display();
    }

    return 0;
}
