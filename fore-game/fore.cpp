#include "pch.h"
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
	// create video mode object
	sf::VideoMode vm(1920, 1080);

	// los res video mode object
	//sf::VideoMode vm(1280, 720);

	// create and open a window for the game
	sf::RenderWindow window(vm, "Fore!", Style::Fullscreen);

	// create a texture to hold a graphic on the GPU
	sf::Texture textureBackground;

	// load a graphic into the texture
	textureBackground.loadFromFile("graphics/lame-background.png");

	// create sprite
	sf::Sprite spriteBackground;

	// attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);

	// set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	// make tree sprite
	sf::Texture textureTree;
	textureTree.loadFromFile("graphics/lame-tree.png");
	sf::Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	// make bee
	sf::Texture textureBee;
	textureBee.loadFromFile("graphics/lame-bee.png");
	sf::Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	// bee currently moving?
	bool beeActive = false;
	// how fast can bee fly?
	float beeSpeed = 0.0f;

	// make 3 cloud sprites from 1 texture
	sf::Texture textureCloud;
	// load 1 new texture
	textureCloud.loadFromFile("graphics/lame-cloud.png");
	// 3 new sprites with same texture
	sf::Sprite spriteCloud1;
	sf::Sprite spriteCloud2;
	sf::Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	// position the clouds off screen
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);
	// are the clouds currently on screen?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	// how fast is each cloud?
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	// variables to control time itself
	sf::Clock clock;

	// TIME BAR
	sf::RectangleShape timeBar; // declare rectangle shape
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(sf::Vector2f(timeBarStartWidth, timeBarHeight)); // set size
	timeBar.setFillColor(sf::Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	sf::Time gameTimeTotal; // new time object
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining; // amount of pixels that need to shrink each second

	// track if game is running
	bool paused = true;

	// draw some text
	int score = 0;
	sf::Text messageText;
	sf::Text scoreText;

	// we need to choose a font
	sf::Font font;
	font.loadFromFile("fonts/BebasNeue-Regular.tff");

	// set the font to message
	messageText.setFont(font);
	scoreText.setFont(font);

	// assign the message
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");

	// make it big
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	// choose color
	messageText.setFillColor(sf::Color::White);
	scoreText.setFillColor(sf::Color::White);

	// position the text
	sf::FloatRect textRect = messageText.getLocalBounds(); // textRect holds a rectangle with floating point coordinates
	messageText.setOrigin(textRect.left + // getLocalBounds() initializes textRect with the cordn. of the rect. that wraps messageText
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f); // this all sets origin at center of textRect
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f); // messageText is then posit. in the exact center of screen
	scoreText.setPosition(20, 20); // position top left with a little padding

	while (window.isOpen())
	{
		/* HANDLE PLAYER OUTPUT */
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}

		// start the game
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			paused = false;

			// reset the time and score
			score = 0; // on each game start
			timeRemaining = 5;
		}

		/* UPDATE SCENE */
		if (!paused) // game won't update when paused
		{
			// measure time
			sf::Time deltaTime = clock.restart();

			// subtract from amount of time remaining
			timeRemaining -= deltaTime.asSeconds(); // substract left var from the right
			// size up the time bar
			timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond *
				timeRemaining, timeBarHeight)); // adjust timeBar

			if (timeRemaining <= 0.0f) // time runs out
			{
				// pause game
				paused = true;

				// change the message shown to player
				messageText.setString("Out of time!");

				// re-position the text based on new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f); // re-calculate position
			}

			// setup the bee
			if (!beeActive)
			{
				// how fast the bee is
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200; // rand # between 199 and 399

				// how high the bee is
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500; // rand # between 499 & 999
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else // move bee when active
			{
				spriteBee.setPosition(
					spriteBee.getPosition().x - // get x coordinates
					(beeSpeed * deltaTime.asSeconds()),
					spriteBee.getPosition().y);

				// if bee reaches edge of screen
				if (spriteBee.getPosition().x < -100)
				{
					// set it up to be a new bee next frame
					beeActive = false; // create new bee at random height/speed
				}
			}

			// manage clouds
			// cloud 1
			if (!cloud1Active)
			{
				// speed of cloud
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				// height of cloud
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition(
					spriteCloud1.getPosition().x +
					(cloud1Speed * deltaTime.asSeconds()),
					spriteCloud1.getPosition().y);

				// has the cloud reached the right hand edge of screen
				if (spriteCloud1.getPosition().x < 1920)
				{
					// set it up for a new cloud next frame
					cloud1Active = false; // create new cloud
				}
			}

			// cloud 2
			if (!cloud2Active)
			{
				// speed of cloud
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				// height of cloud
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else
			{
				spriteCloud2.setPosition(
					spriteCloud2.getPosition().x +
					(cloud2Speed * deltaTime.asSeconds()),
					spriteCloud2.getPosition().y);

				// has the cloud reached the right hand edge of screen
				if (spriteCloud2.getPosition().x < 1920)
				{
					// set it up for a new cloud next frame
					cloud2Active = false; // create new cloud
				}
			}

			// cloud 3
			if (!cloud3Active)
			{
				// speed of cloud
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				// height of cloud
				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else
			{
				spriteCloud3.setPosition(
					spriteCloud3.getPosition().x +
					(cloud1Speed * deltaTime.asSeconds()),
					spriteCloud3.getPosition().y);

				// has the cloud reached the right hand edge of screen
				if (spriteCloud3.getPosition().x < 1920)
				{
					// set it up for a new cloud next frame
					cloud3Active = false; // create new cloud
				}
			}

			// update score text
			std::stringstream ss; // ss adapts each frame
			ss << "Score = " << score;
			scoreText.setString(ss.str());

		} // end if(!paused)
		
		/* DRAW SCENE */

		// clear everything from last frame
		window.clear();

		// Draw game scene here
		window.draw(spriteBackground);

		// draw clouds
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		// draw tree
		window.draw(spriteTree);

		// draw bee (in front of tree)
		window.draw(spriteBee);

		// draw score
		window.draw(scoreText);

		// draw timebar
		window.draw(timeBar); // draw after tree

		if (paused)
		{
			// draw message
			window.draw(messageText);
		}

		// show everything just drawn
		window.display();
	}

	return 0;
}