#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

#include "pch.h"

using namespace sf;

// function declaration
void updateBranches(int seed);  // function prototype, no value returned
const int NUM_BRANCHES = 6;     // 6 moving branches
sf::Sprite branches[NUM_BRANCHES];

// where is player/branch?
// left or right
enum class side { LEFT, RIGHT, NONE };
side branchPosition[NUM_BRANCHES];  // init array

int main() {
  // create video mode object
  sf::VideoMode vm(1920, 1080);

  // los res video mode object
  // sf::VideoMode vm(1280, 720);

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
  // make cloud array
  const int NUM_CLOUDS = 6;
  sf::Sprite clouds[NUM_CLOUDS];
  int cloudSpeeds[NUM_CLOUDS];
  bool cloudsActive[NUM_CLOUDS];

  for (int i = 0; i < NUM_CLOUDS; i++) {
    clouds[i].setTexture(textureCloud);
    clouds[i].setPosition(-300, i * 150);
    cloudsActive[i] = false;
    cloudSpeeds[i] = 0;
  }

  // make 3 cloud sprites from 1 texture
  // sf::Texture textureCloud;
  // load 1 new texture
  // textureCloud.loadFromFile("graphics/lame-cloud.png");
  // 3 new sprites with same texture
  // sf::Sprite spriteCloud1;
  // sf::Sprite spriteCloud2;
  // sf::Sprite spriteCloud3;
  // spriteCloud1.setTexture(textureCloud);
  // spriteCloud2.setTexture(textureCloud);
  // spriteCloud3.setTexture(textureCloud);
  // position the clouds off screen
  // spriteCloud1.setPosition(0, 0);
  // spriteCloud2.setPosition(0, 250);
  // spriteCloud3.setPosition(0, 500);
  // are the clouds currently on screen?
  // bool cloud1Active = false;
  // bool cloud2Active = false;
  // bool cloud3Active = false;
  // how fast is each cloud?
  // float cloud1Speed = 0.0f;
  // float cloud2Speed = 0.0f;
  // float cloud3Speed = 0.0f;

  // variables to control time itself
  sf::Clock clock;

  // TIME BAR
  sf::RectangleShape timeBar;  // declare rectangle shape
  float timeBarStartWidth = 400;
  float timeBarHeight = 80;
  timeBar.setSize(sf::Vector2f(timeBarStartWidth, timeBarHeight));  // set size
  timeBar.setFillColor(sf::Color::Red);
  timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

  sf::Time gameTimeTotal;  // new time object
  float timeRemaining = 6.0f;
  float timeBarWidthPerSecond =
      timeBarStartWidth /
      timeRemaining;  // amount of pixels that need to shrink each second

  // track if game is running
  bool paused = true;

  // draw some text
  int score = 0;
  sf::Text messageText;
  sf::Text scoreText;

  // choose a font
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
  sf::FloatRect textRect =
      messageText.getLocalBounds();  // textRect holds a rectangle with floating
                                     // point coordinates
  messageText.setOrigin(
      textRect.left +  // getLocalBounds() initializes textRect with the cordn.
                       // of the rect. that wraps messageText
          textRect.width / 2.0f,
      textRect.top + textRect.height /
                         2.0f);  // this all sets origin at center of textRect
  messageText.setPosition(
      1920 / 2.0f,
      1080 / 2.0f);  // messageText is then posit. in the exact center of screen
  scoreText.setPosition(20, 20);  // position top left with a little padding

  // backgrounds for text
  sf::RectangleShape rect1;
  rect1.setFillColor(sf::Color(0, 0, 0, 150));
  rect1.setSize(sf::Vector2f(600, 105));
  rect1.setPosition(1150, 30);

  sf::RectangleShape rect2;
  rect2.setFillColor(sf::Color(0, 0, 0, 150));
  rect2.setSize(sf::Vector2f(1000, 105));
  rect2.setPosition(1150, 30);

  // prepare 6 branches
  sf::Texture textureBranch;
  textureBranch.loadFromFile("graphics/branch.png");
  // set texture, origin for each branch sprite
  for (int i = 0; i < NUM_BRANCHES; i++) {
    branches[i].setTexture(textureBranch);
    branches[i].setPosition(-2000, -2000);
    branches[i].setOrigin(220, 20);  // set sprite origin to dead center
  }

  // prepare player
  sf::Texture texturePlayer;
  texturePlayer.loadFromFile("graphics/lame-player.png");
  sf::Sprite spritePlayer;
  spritePlayer.setTexture(texturePlayer);
  spritePlayer.setPosition(580, 720);
  // start player on left side of tree
  side playerSide = side::LEFT;

  // prepare gravestone
  sf::Texture textureRip;
  textureRip.loadFromFile("graphics/rip.png");
  sf::Sprite spriteRip;
  spriteRip.setTexture(textureRip);
  spriteRip.setPosition(600, 860);

  // prepare axe
  sf::Texture textureAxe;
  textureAxe.loadFromFile("graphics/axe.png");
  sf::Sprite spriteAxe;
  spriteAxe.setTexture(textureAxe);
  spriteAxe.setPosition(700, 830);

  // line up axe with tree
  const float AXE_POSITION_LEFT = 700;
  const float AXE_POSITION_RIGHT = 1075;

  // prepare flying log
  sf::Texture textureLog;
  textureLog.loadFromFile("graphics/log.png");
  sf::Sprite spriteLog;
  spriteLog.setTexture(textureLog);
  spriteLog.setPosition(810, 720);

  // log variables
  bool logActive = false;
  float logSpeedX = 1000;
  float logSpeedY = -1500;

  // control player input
  bool acceptInput = false;

  // prepare sound
  sf::SoundBuffer chopBuffer;  // holds actual audio data
  chopBuffer.loadFromFile("sound/chop.wav");
  sf::Sound chop;
  chop.setBuffer(chopBuffer);  // associate with soundBuffer object

  sf::SoundBuffer deathBuffer;
  deathBuffer.loadFromFile("sound/death.wav");
  sf::Sound death;
  death.setBuffer(deathBuffer);

  // out of time
  sf::SoundBuffer outBuffer;
  outBuffer.loadFromFile("sound/out_of_time.wav");
  sf::Sound outOfTime;
  outOfTime.setBuffer(outBuffer);

  // control drawing of score
  int lastDrawn = 0;

  while (window.isOpen()) {
    // score++;
    sf::Event event;
    while (window.pollEvent(event))  // puts data into event object, which
                                     // describes an operating system event
    {
      if (event.type == sf::Event::KeyReleased && !paused) {
        // check for key presses again
        acceptInput = true;

        // hide axe
        spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
      }
    }

    /*
    *********************************************
    HANDLE PLAYER INPUT
    *********************************************
    */

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))  // exit game
    {
      window.close();
    }

    // start the game
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
      paused = false;

      // reset the time and score
      score = 0;  // on each game start
      timeRemaining = 5;

      // make branches disappear
      for (int i = 1; i < NUM_BRANCHES; i++) {
        branchPosition[i] = side::NONE;  // no branches already on player
      }

      // make sure gravestone is hidden
      spriteRip.setPosition(675, 2000);  // remove from screen

      // move player into position
      spritePlayer.setPosition(580, 720);
      acceptInput = true;  // ready to accept presses to chop tree
    }

    // wrap player controls to ensure accepting input
    if (acceptInput)  // when true
    {
      // handle right key
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        playerSide = side::RIGHT;  // ensure player is on right
        score++;

        // add to time remaining
        timeRemaining += (2 / score) + .15;  // bigger score, less time added

        spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

        spritePlayer.setPosition(1200, 720);

        // update branches
        updateBranches(score);  // move branches down 1, spawn random branch

        // send log flying to left
        spriteLog.setPosition(810, 720);  // starting position
        logSpeedX = -5000;
        logActive = true;  // animate log each frame

        acceptInput = false;  // no more chops possible

        // play chop fx
        chop.play();
      }
      // handle left key
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        playerSide = side::LEFT;  // ensure player is on left
        score++;

        // add to time remaining
        timeRemaining += (2 / score) + .15;

        spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);

        spritePlayer.setPosition(580, 720);

        // update branches
        updateBranches(score);

        // send log flying to left
        spriteLog.setPosition(810, 720);  // starting position
        logSpeedX = -5000;
        logActive = true;  // animate log each frame

        acceptInput = false;  // no more chops possible

        // play chop fx
        chop.play();
      }
    }

    /*
    *********************************************
    UPDATE SCENE
    *********************************************
    */

    // TIME HANDLING
    if (!paused)  // game won't update when paused
    {
      // measure time
      sf::Time deltaTime = clock.restart();

      // subtract from amount of time remaining
      timeRemaining -=
          deltaTime.asSeconds();  // substract left var from the right
      // size up the time bar
      timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond * timeRemaining,
                                   timeBarHeight));  // adjust timeBar

      if (timeRemaining <= 0.0f)  // time runs out
      {
        // pause game
        paused = true;

        // change the message shown to player
        messageText.setString("Out of time!");

        // re-position the text based on new size
        textRect = messageText.getLocalBounds();
        messageText.setOrigin(textRect.left + textRect.width / 2.0f,
                              textRect.top + textRect.height / 2.0f);
        messageText.setPosition(1920 / 2.0f,
                                1080 / 2.0f);  // re-calculate position

        // play out of time fx
        outOfTime.play();
      }

      // setup the bee
      if (!beeActive) {
        // how fast the bee is
        srand((int)time(0) * 10);
        beeSpeed = (rand() % 200) + 200;  // rand # between 199 and 399

        // how high the bee is
        srand((int)time(0) * 10);
        float height = (rand() % 500) + 500;  // rand # between 499 & 999
        spriteBee.setPosition(2000, height);
        beeActive = true;
      } else  // move bee when active
      {
        spriteBee.setPosition(spriteBee.getPosition().x -  // get x coordinates
                                  (beeSpeed * deltaTime.asSeconds()),
                              spriteBee.getPosition().y);

        // if bee reaches edge of screen
        if (spriteBee.getPosition().x < -100) {
          // set it up to be a new bee next frame
          beeActive = false;  // create new bee at random height/speed
        }
      }

      // manage clouds with arrays
      for (int i = 0; i < NUM_CLOUDS; i++) {
        if (!cloudsActive[i]) {
          // speed of cloud
          srand((int)time(0) * i);
          cloudSpeeds[i] = (rand() % 200);

          // height of cloud
          srand((int)time(0) * i);
          float height = (rand() % 150);
          clouds[i].setPosition(-200, height);
          cloudsActive[i] = true;
        } else {
          // set new position
          clouds[i].setPosition(clouds[i].getPosition().x +
                                    (cloudSpeeds[i] * deltaTime.asSeconds()),
                                clouds[i].getPosition().y);

          // if cloud reaches right hand edge of screen
          if (clouds[i].getPosition().x > 1920) {
            // set it up for a new cloud next frame
            cloudsActive[i] = false;
          }
        }
      }

      /*
      // manage clouds (OLD)
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
      */

      // draw score once every 100 frames
      lastDrawn++;
      if (lastDrawn == 100) {
        std::stringstream ss;  // ss adapts each frame
        ss << "Score = " << score;
        scoreText.setString(ss.str());

        lastDrawn = 0;
      }

      // update branch sprites
      for (int i = 0; i < NUM_BRANCHES; i++) {
        float height = i * 150;
        if (branchPosition[i] == side::LEFT) {
          branches[i].setPosition(610, height);  // move to left side
          branches[i].setOrigin(220, 40);
          branches[i].setRotation(180);  // flip, as hangs right by default
        } else if (branchPosition[i] == side::RIGHT) {
          branches[i].setPosition(1330, height);  // move to right side
          branches[i].setOrigin(220, 40);
          branches[i].setRotation(0);  // set rotation to normal
        } else {
          branches[i].setPosition(3000, height);  // hide branch offscreen
        }
      }

      // handle flying log
      if (logActive) {
        spriteLog.setPosition(
            spriteLog.getPosition().x + (logSpeedX * deltaTime.asSeconds()),
            spriteLog.getPosition().y + (logSpeedY * deltaTime.asSeconds()));

        // if log disappears from view
        if (spriteLog.getPosition().x < -100 ||
            spriteLog.getPosition().x > 2000) {
          // set it up for a new log next frame
          logActive = false;
          spriteLog.setPosition(810, 720);
        }
      }

      // if player is crushed by branch
      if (branchPosition[5] == playerSide) {
        // death
        paused = true;
        acceptInput = false;

        // draw gravestone
        spriteRip.setPosition(525, 760);

        // hide player
        spritePlayer.setPosition(2000, 660);

        // change text message
        messageText.setString("YOU'VE BEEN BRANCHED.");

        // center message
        textRect = messageText.getLocalBounds();

        messageText.setOrigin(textRect.left + textRect.width / 2.0f,
                              textRect.top + textRect.height / 2.0f);

        messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

        // play death fx
        death.play();
      }

    }  // end if(!paused)

    /*
    *********************************************
    DRAW SCENE
    *********************************************
    */

    // clear everything from last frame
    window.clear();

    // Draw game scene here
    window.draw(spriteBackground);

    // draw clouds
    for (int i = 0; i < NUM_CLOUDS; i++) {
      window.draw(clouds[i]);
    }

    /*
    window.draw(spriteCloud1);
    window.draw(spriteCloud2);
    window.draw(spriteCloud3);
    */

    // draw branches
    for (int i = 0; i < NUM_BRANCHES; i++) {
      window.draw(branches[i]);
    }

    // draw tree
    window.draw(spriteTree);

    // draw player
    window.draw(spritePlayer);

    // draw axe
    window.draw(spriteAxe);

    // draw flying log
    window.draw(spriteLog);

    // draw gravestone
    window.draw(spriteRip);

    // draw bee (in front of tree)
    window.draw(spriteBee);

    // draw score
    window.draw(scoreText);

    // draw timebar
    window.draw(timeBar);  // draw after tree

    if (paused) {
      // draw message
      window.draw(messageText);
    }

    // show everything just drawn
    window.display();
  }

  return 0;
}

// function definition
void updateBranches(int seed) {
  for (int j = NUM_BRANCHES - 1; j > 0; j--)  // move branches down one place
  {
    branchPosition[j] = branchPosition[j - 1];
  }
  // spawn new branch at position 0
  // LEFT, RIGHT, or NONE
  srand((int)time(0) + seed);  // random seed always different
  int r = (rand() % 5);
  switch (r) {
    case 0:  // if r == 0
      branchPosition[0] = side::LEFT;
      break;

    case 1:
      branchPosition[0] = side::RIGHT;
      break;

    default:  // if r == anything else
      branchPosition[0] = side::NONE;
      break;
  }
}
