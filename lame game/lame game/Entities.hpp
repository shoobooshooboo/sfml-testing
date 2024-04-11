#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#define DEBUG_MODE

class Entity : public sf::Sprite {
protected:
    std::string spriteFileName;
    sf::Image image;
    sf::Texture texture;
public:
    Entity(std::string fileName) : spriteFileName(fileName){
        image.loadFromFile(fileName);
        texture.loadFromImage(image);
        this->setTexture(texture);
    }
    ~Entity() {}

    bool intersects(const Entity &copy) { return this->getGlobalBounds().intersects(copy.getGlobalBounds()); }

    virtual void tick(std::vector<Entity *> &entities) = 0;
};

class Projectile : public Entity {
private:
    int damage;
    float moveSpeed;

public:
    Projectile(std::string spriteFile, int d, float ms) : Entity(spriteFile), damage(d), moveSpeed(ms){}
    Projectile(const Projectile &copy);

    /*projectile moves forward and attempts to hit an enemy, dealing damage*/
    void tick(std::vector<Entity *> &entities);
};

class Enemy : public Entity {
private:
    int attackCooldown;
    int curCD;
    int damage;
    float moveSpeed;
    int health;

public:
    Enemy(std::string spriteFile, float ms, int h, int acd, int d) : Entity(spriteFile), moveSpeed(-ms), health(h), attackCooldown(acd), curCD(acd), damage(d){}

    /*enemy moves forward and attacks a tower if intersecting with tower hitbox*/
    void tick(std::vector<Entity *> &entities);
    /*deals damage. returns true if enemy died*/
    bool take_damage(int damage);
};

class Tower : public Entity {
private:
    int attackCooldown;
    int curCD;
    int health;
    Projectile projectile;

public:
    Tower(std::string spriteFile, int acd, int h, Projectile p) : Entity(spriteFile), attackCooldown(acd), health(h), projectile(p), curCD(acd) { }

    /*decrements attack cooldown. if cooldown reaches 0, spawn projectile and reset*/
    void tick(std::vector<Entity *> &entities);
    /*deals damage. returns true if tower died*/
    bool take_damage(int damage);
};