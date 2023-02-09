#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "Animation.h"

enum class GameState
{
    RUNNING,        // RUNNING = 0;
    GAME_OVER,       // GAME_OVER = 1;
    NUM_GAME_STATES
};

int main()
{

    // --------------------------------------------------
    // Setup
    // --------------------------------------------------
#pragma region Setup

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Pick Up Sticks", sf::Style::None);

    srand(time(NULL));

    sf::Texture playerTextureStand;
    playerTextureStand.loadFromFile("Assets/Player_Stand_1.png");
    sf::Texture grassTexture;
    grassTexture.loadFromFile("Assets/Grass.png");
    sf::Texture stickTexture;
    stickTexture.loadFromFile("Assets/Stick.png");



    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTextureStand);

    // Player animation setup
    Animation playerAnimation(&playerSprite, "Assets/Player", 12.0f);
    playerAnimation.AddClip("Stand", 1, false);
    playerAnimation.AddClip("Walk", 2, true);



    sf::Sprite grassSprite;
    grassSprite.setTexture(grassTexture);
    sf::Sprite stickSprite;
    stickSprite.setTexture(stickTexture);

    std::vector<sf::Sprite> grassSprites;
    int numGrassSpritesToAdd = 5;
    for (int i = 0; i < numGrassSpritesToAdd; ++i)
    {
        int scaleTen = 5 + rand() % 11; // 5-15
        float scale = scaleTen / 10.0f; // 0.5 - 1.5
        grassSprite.setScale(scale, scale);
        int colorTint = 100 + rand() % 155;
        grassSprite.setColor(sf::Color(colorTint, colorTint, colorTint));
        grassSprite.setPosition(sf::Vector2f(rand() % (window.getSize().x - grassTexture.getSize().x), rand() % (window.getSize().y - grassTexture.getSize().y)));
        grassSprites.push_back(grassSprite);
    }
    std::vector<sf::Sprite> stickSprites;
    int randomRotation = rand() % 360;
    stickSprite.setRotation(randomRotation);
    stickSprite.setPosition(sf::Vector2f(rand() % (window.getSize().x - stickTexture.getSize().x), rand() % (window.getSize().y - stickTexture.getSize().y)));
    stickSprites.push_back(stickSprite);


    // Position setup
    playerSprite.setPosition(sf::Vector2f(300.0f, 300.0f));

    // Colour Setup
    //playerSprite.setColor(sf::Color(200,200,200));

    // Rotation Example
    //playerSprite.setRotation(90);

    // Scale Example
    //playerSprite.setScale(1.0f, 3.0f);

    // Origin Example
    playerSprite.setOrigin(playerTextureStand.getSize().x / 2, playerTextureStand.getSize().y / 2);

    // Load Fonts
    sf::Font gameFont;
    gameFont.loadFromFile("Assets/GameFont.ttf");

    // Create Text Objects
    sf::Text gameTitle;
    gameTitle.setFont(gameFont);
    gameTitle.setString("Pick Up Sticks");
    gameTitle.setFillColor(sf::Color::Magenta);
    gameTitle.setOutlineThickness(2.0f);
    gameTitle.setOutlineColor(sf::Color::Black);
    gameTitle.setStyle(sf::Text::Style::Bold | sf::Text::Style::Italic);
    gameTitle.setCharacterSize(60);

    float textWidth = gameTitle.getLocalBounds().width;
    gameTitle.setPosition((float)window.getSize().x / 2.0f - textWidth / 2.0f, 10.0f);


    sf::Text timerText;
    timerText.setFont(gameFont);
    timerText.setString("Time: ");
    timerText.setFillColor(sf::Color::White);
    timerText.setOutlineThickness(2.0f);
    timerText.setOutlineColor(sf::Color::Black);
    timerText.setCharacterSize(30);

    timerText.setPosition((float)window.getSize().x - 300.0f, 10.0f);

    sf::Text scoreText;
    scoreText.setFont(gameFont);
    scoreText.setString("Score: 0");
    scoreText.setFillColor(sf::Color::White);
    scoreText.setOutlineThickness(2.0f);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setCharacterSize(30);

    scoreText.setPosition(10.0f, 10.0f);

    sf::Text gameOverMessage;
    gameOverMessage.setFont(gameFont);
    gameOverMessage.setString("GAME OVER!");
    gameOverMessage.setFillColor(sf::Color::Red);
    gameOverMessage.setOutlineThickness(2.0f);
    gameOverMessage.setOutlineColor(sf::Color::Black);
    gameOverMessage.setStyle(sf::Text::Style::Bold | sf::Text::Style::Italic);
    gameOverMessage.setCharacterSize(60);

    textWidth = gameOverMessage.getLocalBounds().width;
    gameOverMessage.setPosition((float)window.getSize().x / 2.0f - textWidth / 2.0f, (float)window.getSize().y/2 - 300);

    sf::Text restartText;
    restartText.setFont(gameFont);
    restartText.setString("< Press Enter to Restart >");
    restartText.setFillColor(sf::Color::White);
    restartText.setOutlineThickness(2.0f);
    restartText.setOutlineColor(sf::Color::Black);
    restartText.setCharacterSize(30);

    textWidth = restartText.getLocalBounds().width;
    restartText.setPosition((float)window.getSize().x / 2.0f - textWidth / 2.0f, (float)window.getSize().y/2 + 100);


    sf::SoundBuffer startSFXBuffer;
    startSFXBuffer.loadFromFile("Assets/Start.wav");

    sf::Sound startSFX;
    startSFX.setBuffer(startSFXBuffer);
    startSFX.play();
    
    sf::Music gameMusic;
    gameMusic.openFromFile("Assets/Music.ogg");
    gameMusic.setVolume(50);
    gameMusic.setLoop(true);
    gameMusic.play();

    float xDir = (10 - rand() % 21)/10.0f;
    float yDir = (10 - rand() % 21)/10.0f;
    sf::Vector2f direction(xDir, yDir);

    bool blinkPressedPrev = false;


    // Clocks and timers
    sf::Clock deltaTimeClock;
    sf::Clock overallTimeClock;

    sf::Clock gameTimer;
    float gameDuration = 30; // How long the game lasts

    //bool gameRunning = true;
    GameState currentState = GameState::RUNNING; // assign the value 0 to the currentState, which we know is RUNNING

    sf::Clock stickSpawnClock;
    float stickSpawnCooldownDuration = 1;

    int score = 0;


#pragma endregion
    // End Setup


    while (window.isOpen())
    {

        // --------------------------------------------------
        // Event Polling
        // --------------------------------------------------
#pragma region Event Polling

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }

            if (event.type == sf::Event::Closed)
                window.close();
        }

#pragma endregion


        // --------------------------------------------------
        // Update
        // --------------------------------------------------
#pragma region Update

        // Get time
        sf::Time deltaTime = deltaTimeClock.restart();
        sf::Time totalTime = overallTimeClock.getElapsedTime();

        // Game Timer
        float gameTimeFloat = gameTimer.getElapsedTime().asSeconds();
        float remainingTimeFloat = gameDuration - gameTimeFloat;
        std::string timerString = "Time: ";

        if (remainingTimeFloat <= 0)
        {
            remainingTimeFloat = 0;
            //gameRunning = false;
            currentState = GameState::GAME_OVER;
        }
        timerString += std::to_string((int)ceil(remainingTimeFloat));
        // Display time passed this game
        timerText.setString(timerString);

        // Player 1 controller
        int player1Controller = 0;

        // Only process game logic when game is running
        if (currentState == GameState::RUNNING)
        {
            // Move the character
            direction.x = 0;
            direction.y = 0;

            if (sf::Joystick::isConnected(player1Controller))
            {
                float axisX = sf::Joystick::getAxisPosition(player1Controller, sf::Joystick::X);
                float axisY = sf::Joystick::getAxisPosition(player1Controller, sf::Joystick::Y);

                float deadzone = 25;

                if (abs(axisX) > deadzone)
                    direction.x = axisX / 100.0f;
                if (abs(axisY) > deadzone)
                    direction.y = axisY / 100.0f;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                direction.x = -1;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                direction.x = 1;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                direction.y = -1;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                direction.y = 1;
            }

            if (direction.x != 0 || direction.y != 0)
            {
                playerAnimation.Play("Walk");
            }
            else
            {
                playerAnimation.Play("Stand");
            }


            // Update player position based on movement direction
            float speed = 500;
            // velocity = direction * speed
            sf::Vector2f velocity = direction * speed;
            // distance traveled = velocity * time
            sf::Vector2f distance = velocity * deltaTime.asSeconds();
            sf::Vector2f newPosition = playerSprite.getPosition() + distance;
            playerSprite.setPosition(newPosition);

            // Blink teleport
            bool blinkPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Joystick::isButtonPressed(player1Controller, 0);
            // If we've JUST NOW pressed the blink button...
            if (blinkPressed && !blinkPressedPrev)
            {
                sf::Vector2f blinkPosition = playerSprite.getPosition() + direction * 100.0f;
                playerSprite.setPosition(blinkPosition);
            }
            blinkPressedPrev = blinkPressed;

            // Spawn a stick when mouse clicked (debug only)
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                // Get the mouse position
                // get the local mouse position (relative to a window)
                sf::Vector2i localPosition = sf::Mouse::getPosition(window); // window is a sf::Window
                sf::Vector2f mousePositionFloat = (sf::Vector2f)localPosition;

                // Spawn a stick at that position
                stickSprite.setPosition(mousePositionFloat);
                stickSprites.push_back(stickSprite);
            }

            // If it is time to spawn a stick...
            if (stickSpawnClock.getElapsedTime().asSeconds() >= stickSpawnCooldownDuration)
            {
                // Spawn a stick and restart the clock
                stickSprite.setPosition(sf::Vector2f(rand() % (window.getSize().x - stickTexture.getSize().x), rand() % (window.getSize().y - stickTexture.getSize().y)));
                stickSprites.push_back(stickSprite);
                stickSpawnClock.restart();
            }


            // Check if player is colliding with sticks
            // TODO: Next week
            sf::FloatRect playerBounds = playerSprite.getGlobalBounds();

            // Loop through all our stick sprites and check if any are overlapping with our player
            // 
            for (auto it = stickSprites.begin(); it != stickSprites.end(); /*++it*/ )
            //for (int i = stickSprites.size() - 1; i >= 0; --i)
            {
                // it is an iterator, which can be used like a pointer to the type inside the vector
                sf::FloatRect stickBounds = it->getGlobalBounds();
                // If the stick and player overlap...
                if (playerBounds.intersects(stickBounds))
                {
                    // Delete the stick
                    it = stickSprites.erase(it);
                    // Add to the score
                    ++score;
                    // Update the score text
                    std::string scoreString = "Score: ";
                    scoreString += std::to_string(score);
                    scoreText.setString(scoreString);
                }
                else
                {
                    // Add to iterator here, since we didn't erase
                    ++it;
                }
            }

            // Process animation
            playerAnimation.Update();


        } // end of gameRunning if statement

        if (currentState == GameState::GAME_OVER)
        {
            // Restart the game!
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                currentState = GameState::RUNNING;
                stickSprites.clear();
                // TODO: Reset score
                gameTimer.restart();
                playerSprite.setPosition(sf::Vector2f(300.0f, 300.0f));
                score = 0;
                // Update the score text
                std::string scoreString = "Score: ";
                scoreString += std::to_string(score);
                scoreText.setString(scoreString);
            }
        }




#pragma endregion


        // --------------------------------------------------
        // Drawing
        // --------------------------------------------------
#pragma region Drawing

        window.clear(sf::Color(61, 204, 90));

        // Draw all the things
        for (int i = 0; i < grassSprites.size(); ++i)
            window.draw(grassSprites[i]);
        for (int i = 0; i < stickSprites.size(); ++i)
            window.draw(stickSprites[i]);
        window.draw(playerSprite);

        window.draw(gameTitle);
        window.draw(timerText);
        window.draw(scoreText);

        if (currentState == GameState::GAME_OVER)
        {
            window.draw(gameOverMessage);
            window.draw(restartText);
        }

        window.display();

#pragma endregion

    }

    return 0;
}