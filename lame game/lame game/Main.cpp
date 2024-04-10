#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

int main() {
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

    return 0;
}