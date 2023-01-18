#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

int main()
{

    // --------------------------------------------------
    // Setup
    // --------------------------------------------------
#pragma region Setup

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Pick Up Sticks", sf::Style::None);

    srand(time(NULL));

    sf::Texture playerTexture;
    playerTexture.loadFromFile("Assets/Player_Stand.png");
    sf::Texture grassTexture;
    grassTexture.loadFromFile("Assets/Grass.png");
    sf::Texture stickTexture;
    stickTexture.loadFromFile("Assets/Stick.png");

    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
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
    playerSprite.setPosition(sf::Vector2f(0.0f, 0.0f));

    // Colour Setup
    //playerSprite.setColor(sf::Color(200,200,200));

    // Rotation Example
    playerSprite.setRotation(90);

    // Scale Example
    //playerSprite.setScale(1.0f, 3.0f);

    // Origin Example
    playerSprite.setOrigin(playerTexture.getSize().x / 2, playerTexture.getSize().y / 2);

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

        window.display();

#pragma endregion

    }

    return 0;
}