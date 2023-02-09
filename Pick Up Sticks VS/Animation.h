#pragma once

// Includes
#include <SFML/Graphics.hpp>



class Animation
{
	struct Clip {
		std::vector<sf::Texture> textures;
		bool shouldLoop;
	};

public:

	Animation(sf::Sprite* newAnimatedSprite, 
		std::string newBaseFilePath, 
		float framesPerSecond, 
		std::string newFileType = "png");

	void AddClip(std::string clipName, int numFrames, bool newShouldLoop = false);

	void Update();

	void Play();
	void Play(std::string clipToPlay);
	void Stop();
	void Pause();

private:
	// Data
	sf::Sprite* animatedSprite;
	std::string baseFilePath;
	std::string fileType;
	Clip* currentClip;
	std::map<std::string, Clip > animations;
	int currentFrame;
	sf::Time timePerFrame;
	sf::Clock animationClock;
	bool isPlaying;

};

