#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

const int SCREEN_WIDTH = sf::VideoMode::getDesktopMode().width;
const int SCREEN_HEIGHT = sf::VideoMode::getDesktopMode().height;

void screensaver() {
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    sf::Image image;
    image.loadFromFile("face.png");
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    bool movingUp = false, movingRight = true;
    int x = 0, y = 0;
    int xLowBound = 0, xHighBound = 2360,
        yLowBound = 0, yHighBound = 1400;

    window.setFramerateLimit(120);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        sprite.rotate(1.0f);
        // sprite.scale(sf::Vector2f(2.0f, 2.0f));
        sprite.setPosition(100, 100);
        sprite.setColor(sf::Color::Green);
        window.draw(sprite);


        //update window position
        if (movingUp)
            y -= 10;
        else
            y += 10;
        if (movingRight)
            x += 10;
        else
            x -= 10;



        //check bounds
        if (x <= xLowBound)
            movingRight = true;
        else if (x >= xHighBound)
            movingRight = false;

        if (y <= yLowBound)
            movingUp = false;
        else if (y >= yHighBound)
            movingUp = true;

        if ((y <= yLowBound || y >= yHighBound)
            && (x <= xLowBound || x >= xHighBound))
            window.close();

        window.setPosition(sf::Vector2i(x, y));
        window.display();
    }
}

sf::Vector2f normalize(sf::Vector2f v) {
    float magnitude = sqrtf(v.x * v.x + v.y * v.y);
    return sf::Vector2f(v.x / magnitude, v.y / magnitude);
}

int main() {
    /*const float heightScale = 1000 / SCREEN_HEIGHT;
    const float widthScale = 1000 / SCREEN_WIDTH;*/
    sf::Image image;
    image.loadFromFile("face.png");
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite ball;
    ball.setTexture(texture);

    const float paddleHeight = 200.f, paddleWidth = 20.f,
        ballDiameter =  ball.getGlobalBounds().getSize().x;
    unsigned int ballCooldown = 60;
    sf::Vector2f ballSpawnPoint(450.f, 450.f);
    

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "game");
    //sf::CircleShape ball(ballRadius);
    ball.setPosition(ballSpawnPoint);
    sf::Vector2f ballDirection(-1.f, 0.f);
    float ballSpeed = 10.f;
    //ball.setFillColor(sf::Color::Green);
    sf::RectangleShape p1(sf::Vector2f(paddleWidth, paddleHeight)), p2(sf::Vector2f(paddleWidth, paddleHeight));
    p1.setFillColor(sf::Color::White);
    p2.setFillColor(sf::Color::White);
    p1.setPosition(sf::Vector2f(0.f, 500.f - (paddleHeight / 2)));
    p2.setPosition(sf::Vector2f(1000.f - paddleWidth, 500.f - (paddleHeight / 2)));
    window.setFramerateLimit(60);
    
    int p1Score = 0, p2Score = 0;

    sf::Text p1ScoreText,
        p2ScoreText;
    sf::Font font;
    font.loadFromFile("arial.ttf");
    p1ScoreText.setCharacterSize(50);
    p1ScoreText.setString("0");
    p1ScoreText.setFillColor(sf::Color::Cyan);
    p1ScoreText.setFont(font);
    p2ScoreText.setCharacterSize(50);
    p2ScoreText.setString("0");
    p2ScoreText.setFillColor(sf::Color::Cyan);
    p2ScoreText.setFont(font);
    p2ScoreText.setPosition(sf::Vector2f(973.f, 0.f));

    sf::Text winMessage;
    winMessage.setCharacterSize(100);
    winMessage.setString("");
    winMessage.setFillColor(sf::Color::Magenta);
    winMessage.setFont(font);
    winMessage.setPosition(sf::Vector2f(300.f, 150.f));

    while (window.isOpen()) {
        /*sf::Event::KeyEvent event;
        while(event.)*/
        sf::Vector2f p1Pos = p1.getPosition(),
            p2Pos = p2.getPosition(),
            ballPos = ball.getPosition();

        if (!ballCooldown)
            ballPos += normalize(ballDirection) * ballSpeed;
        else
            ballCooldown--;

        //std::cout << ballCooldown << std::endl;
        
        //get keyboard inputs and update position
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            p2Pos.y -= 10.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            p2Pos.y += 10.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            p1Pos.y -= 10.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            p1Pos.y += 10.f;

        //keep shit in bounds
        if (p1Pos.y < 0.f)
            p1Pos.y = 0.f;
        else if (p1Pos.y > 1000.f - paddleHeight)
            p1Pos.y = 1000.f - paddleHeight;
        if (p2Pos.y < 0.f)
            p2Pos.y = 0.f;
        else if (p2Pos.y > 1000.f - paddleHeight)
            p2Pos.y = 1000.f - paddleHeight;

        if (ballPos.y >= (1000.f - ballDiameter)) {
            ballPos.y = 1000.f - ballDiameter;
            ballDirection.y = -ballDirection.y;
        }
        else if (ballPos.y <= 0.f) {
            ballPos.y = 0.f;
            ballDirection.y = -ballDirection.y;
        }
        if (ballPos.x <= paddleWidth) {
            //check for colision
            if (ballPos.y + ballDiameter >= p1Pos.y && ballPos.y <= p1Pos.y + paddleHeight) {
                ballSpeed++;
                ballPos.x = paddleWidth;
                ballDirection.x = -ballDirection.x;
                ballDirection.y = (((ballPos.y + ballDiameter - p1Pos.y) / (ballPos.y + ballDiameter - (p1Pos.y / 2)) - .4f)) * 2.f;
            }
            else if (ballPos.x <= 0.f) {
                p2ScoreText.setString(std::to_string(++p2Score));
                ballPos = ballSpawnPoint;
                if (p2Score < 5) {
                    ballSpeed /= 2;
                    ballCooldown = 60;
                }
                else {
                    ballCooldown = UINT_MAX;
                    winMessage.setString("P2 Wins");
                }
            }
        }
        else if (ballPos.x + ballDiameter >= p2Pos.x) {
            //check for colision
            if (ballPos.y + ballDiameter >= p2Pos.y && ballPos.y <= p2Pos.y + paddleHeight) {
                ballSpeed++;
                ballPos.x = p2Pos.x - ballDiameter;
                ballDirection.x = -ballDirection.x;
                ballDirection.y = (((ballPos.y + ballDiameter - p2Pos.y) / (ballPos.y + ballDiameter - (p2Pos.y / 2)) - .3f)) * 2.f;
            }
            else if (ballPos.x >= 1000.f - ballDiameter) {
                p1ScoreText.setString(std::to_string(++p1Score));
                ballPos = ballSpawnPoint;
                if (p1Score < 5) {
                    ballSpeed /= 2;
                    ballCooldown = 60;
                }
                else {
                    ballCooldown = UINT_MAX;
                    winMessage.setString("P1 Wins");
                }
            }
        }

        //update screen
        window.clear();
        p1.setPosition(p1Pos);
        p2.setPosition(p2Pos);
        ball.setPosition(ballPos);
        window.draw(ball);
        window.draw(p1);
        window.draw(p2);
        window.draw(p1ScoreText);
        window.draw(p2ScoreText);
        window.draw(winMessage);
        window.display();
    }

    return 0;
}