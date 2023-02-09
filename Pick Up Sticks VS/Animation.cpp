#include "Animation.h"

Animation::Animation(sf::Sprite* newAnimatedSprite, 
	std::string newBaseFilePath, 
	float framesPerSecond, 
	std::string newFileType)
	: animatedSprite (newAnimatedSprite)
	, baseFilePath (newBaseFilePath)
	, fileType (newFileType)
	, currentClip (nullptr)
	, animations()
	, currentFrame(0)
	, timePerFrame(sf::seconds(1.0f/framesPerSecond))
	, animationClock()
	, isPlaying(false)
{
}

void Animation::AddClip(std::string clipName, int numFrames, bool newShouldLoop)
{
	// Construct the clip
	Clip newClip;

	for (int i = 0; i < numFrames; ++i)
	{
		newClip.textures.push_back(sf::Texture());
		newClip.textures[i].loadFromFile(baseFilePath + "_" + clipName + "_" + std::to_string(i + 1) + "." + fileType);
	}

	newClip.shouldLoop = newShouldLoop;

	// Add the clip to our map
	animations[clipName] = newClip;
}

void Animation::Update()
{
	// Only process animation if it is playing
	if (isPlaying && currentClip != nullptr)
	{
		sf::Time timePassedThisFrame = animationClock.getElapsedTime();
		if (timePassedThisFrame >= timePerFrame)
		{
			animationClock.restart();

			// Go to the next frame of the animation
			++currentFrame;
			if (currentFrame >= currentClip->textures.size())
			{
				if (currentClip->shouldLoop)
				{
					currentFrame = 0;
				}
				else
				{
					currentFrame = currentClip->textures.size() - 1;
					isPlaying = false;
				}
			}

			// Load the new current frame into our sprite
			animatedSprite->setTexture(currentClip->textures[currentFrame]);
		}
	}
}

void Animation::Play()
{
	isPlaying = true;
}

void Animation::Play(std::string clipToPlay)
{
	if (animations.find(clipToPlay) != animations.end())
	{
		if (currentClip != &(animations[clipToPlay]))
		{
			currentClip = &(animations[clipToPlay]);
			currentFrame = 0;
		}
		isPlaying = true;
	}
}

void Animation::Stop()
{
	isPlaying = false;
	currentFrame = 0;
}

void Animation::Pause()
{
	isPlaying = false;
}
