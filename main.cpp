#include <tgmath.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>  // Include the random number generator header
#include <random>
#include <vector>

const bool debug = true;
const int WIDTH = 1280;
const int HEIGHT = 720;
// Add a function to reset the game state

// Function to check AABB collision
bool checkCollision(const sf::Sprite &sprite1, const sf::Sprite &sprite2) {
    sf::FloatRect rect1 = sprite1.getGlobalBounds();
    sf::FloatRect rect2 = sprite2.getGlobalBounds();
    return rect1.intersects(rect2);
}

void resetGameState(unsigned int &counter, sf::Sprite &character,
                    sf::RectangleShape &enemy,
                    std::vector<sf::CircleShape> &projectiles,
                    std::vector<sf::Vector2f> &directions,
                    std::vector<sf::CircleShape> &playerProjectiles,
                    bool &gameOver, bool &win, std::vector<sf::Sprite> &walls) {
    gameOver = false;
    win = false;
    // Reset the counter
    counter = 0;

    character.setPosition(WIDTH - 100.0f, HEIGHT - 100.0f);
    character.setRotation(0);

    // Reset the enemy's position
    enemy.setPosition(295.0f, 50.0f);

    // Clear the projectiles and directions vectors
    projectiles.clear();
    directions.clear();

    // Clear the playerProjectiles vector
    playerProjectiles.clear();
    // Generate a new seed based on the current time
    auto currentTime =
        std::chrono::system_clock::now().time_since_epoch().count();
    unsigned newSeed = static_cast<unsigned>(currentTime);

    // Set the new seed for the random number generator
    std::srand(newSeed);
    for (auto &wall : walls) {
        float newX = static_cast<float>(
            std::rand() %
            (WIDTH - static_cast<int>(wall.getGlobalBounds().width)));
        float newY = static_cast<float>(
            std::rand() %
            (HEIGHT - static_cast<int>(wall.getGlobalBounds().height)));
        wall.setPosition(newX, newY);
    }
}
int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML Window");

    // Load a font
    sf::Font font;
    if (!font.loadFromFile("Roboto-Regular.ttf")) {
        // Error handling if the font is not found
        std::cerr << "Error loading font" << std::endl;
        return 1;
    }

    sf::Texture obstacleTexture;
    if (!obstacleTexture.loadFromFile("wall.png")) {
        // Handle loading error
        std::cerr << "Error loading obstacle texture." << std::endl;
    }

    sf::Sprite obstacle;
    obstacle.setTexture(obstacleTexture);

    // Inside the main function, after loading the texture
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    std::uniform_int_distribution<int> wallCountDistribution(2, 5);
    std::uniform_int_distribution<int> positionDistribution(
        50, window.getSize().x - 50);

    int wallCount = wallCountDistribution(generator);
    std::vector<sf::Sprite> walls(wallCount, sf::Sprite(obstacleTexture));

    obstacle.setPosition(220, 220);
    obstacle.setScale(0.5f, 0.5f);

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

    character.setPosition(WIDTH - 100.0f, HEIGHT - 100.0f);
    character.setRotation(0);
    float enemySpeed = 200.0f;

    sf::Vector2f enemyMovement(1.0f, 0.0f);
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

    float movementInterval = 1.0f;
    sf::Clock movementClock;

    // Time interval between spawning projectiles
    float spawnInterval = 1.0f;
    sf::Clock spawnClock;

    // Calculate delta time for consistent movement speed
    sf::Clock clock;
    sf::Time lastTime = clock.getElapsedTime();

    // Display the character, projectiles, and enemy
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over! Press R to restart");

    sf::Text winText;
    winText.setFont(font);
    winText.setString("You WIN!!!!! Press R to restart");

    winText.setCharacterSize(36);
    winText.setFillColor(sf::Color::Black);
    winText.setPosition(WIDTH / 2 - 200, HEIGHT / 2 - 30);

    gameOverText.setCharacterSize(36);
    gameOverText.setFillColor(sf::Color::Black);
    gameOverText.setPosition(WIDTH / 2 - 200, HEIGHT / 2 - 30);
    bool gameOver = false;
    bool win = false;

    // ...

    for (int i = 0; i < wallCount; ++i) {
        walls[i].setPosition(positionDistribution(generator),
                             positionDistribution(generator));
        walls[i].setScale(0.5f, 0.5f);
    }

    while (window.isOpen()) {
        sf::Vector2f previousCharacterPosition = character.getPosition();
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (gameOver && event.key.code == sf::Keyboard::R) {
                resetGameState(counter, character, enemy, projectiles,
                               directions, playerProjectiles, gameOver, win,
                               walls);
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

        enemy.move(enemyMovement * enemySpeed * deltaTime.asSeconds());

        // Reverse the movement vector if the enemy reaches the edges of the
        // screen
        if (enemy.getPosition().x < 0 ||
            enemy.getPosition().x + enemy.getSize().x > WIDTH) {
            enemyMovement.x = -enemyMovement.x;
        }
        if (enemy.getPosition().y < 0 ||
            enemy.getPosition().y + enemy.getSize().y > HEIGHT) {
            enemyMovement.y = -enemyMovement.y;
        }

        // Randomly change the enemy's direction
        if (movementClock.getElapsedTime().asSeconds() > movementInterval) {
            float randomAngle = (float)rand() / (float)RAND_MAX * 360.0f;
            enemyMovement.x = std::cos(randomAngle * 3.14159265f / 180.0f);
            enemyMovement.y = std::sin(randomAngle * 3.14159265f / 180.0f);
            movementInterval = ((float)rand() / (float)RAND_MAX) +
                               0.5f;  // Set new movement interval between 0.5f
                                      // and 1.5f seconds
            movementClock.restart();
        }
        // Move the enemy
        enemy.move(enemyMovement * enemySpeed * deltaTime.asSeconds());
        if (character.getGlobalBounds().intersects(enemy.getGlobalBounds())) {
            gameOver = true;
        }
        // Move the player's projectiles
        for (size_t i = 0; i < playerProjectiles.size(); ++i) {
            for (const auto &wall : walls) {
                if (wall.getGlobalBounds().intersects(
                        playerProjectiles[i].getGlobalBounds())) {
                    playerProjectiles.erase(playerProjectiles.begin() + i);

                    playerProjectileActive = false;

                    // playerProjectiles.erase(playerProjectiles.begin() + i);

                    break;
                }
            }
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

                // Check for collisions between the player's projectile and the
                // walls
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
                win = true;
                break;
            }
        }
        if (gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            // Reset game variables
            // (Reinitialize the player, enemy, projectiles, counter, etc.)

            // Set the game paused flag to false
            // Call the resetGameState function to set the
            // initial game state
            resetGameState(counter, character, enemy, projectiles, directions,
                           playerProjectiles, gameOver, win, walls);
            gameOver = false;
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

                if (!debug) {
                    gameOver = true;
                }
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

        for (const auto &wall : walls) {
            window.draw(wall);
            if (checkCollision(character, wall)) {
                character.setPosition(previousCharacterPosition);
            }
        }

        for (auto &projectile : projectiles) {
            if (character.getGlobalBounds().intersects(
                    projectile.getGlobalBounds())) {
                gameOver = true;
                break;
            }
        }
        // Clear the window
        window.clear(sf::Color::White);

        if (!gameOver) {
            // Draw the walls
            for (const auto &wall : walls) {
                window.draw(wall);
            }

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
        }
        if (gameOver && win) {
            window.draw(winText);

        } else if (gameOver) {
            window.draw(gameOverText);
        }  // Display the window

        window.display();
    }

    return 0;
}