#include "Entities.hpp"

void Tower::tick(std::vector<Entity *> &entities) {
	bool enemyInLane = false;

	for (Entity *e : entities) {
		if (dynamic_cast<Enemy *>(e) != nullptr) {
			enemyInLane = true;
			break;
		}
	}

	if (!curCD) {
		if (enemyInLane) {
			curCD = attackCooldown;
			entities.push_back(new Projectile(projectile));
			entities[entities.size() - 1]->setPosition(this->getPosition());
		}
	}
	else
		curCD--;
}

bool Tower::take_damage(int damage) {
	health -= damage;
#ifdef DEBUG_MODE
	std::cout << "tower took damage\n";
#endif
	if (health <= 0)
		return true;
	return false;
}

void Enemy::tick(std::vector<Entity *> &entities) {
	int targetIndex = -1;
	for (int i = 0; i < entities.size(); i++) {
		Tower *tower = dynamic_cast<Tower *>(entities[i]);
		if (tower != nullptr && this->intersects(*tower)) {
			if (!curCD--) {
				curCD = attackCooldown;
				bool towerDied = tower->take_damage(damage);
				if (towerDied) {
					delete entities[i];
					entities.erase(entities.begin() + i--);
#ifdef DEBUG_MODE
					std::cout << "Tower died. number of entities: " << entities.size() << std::endl;
#endif
				}
			}
			return;
		}
	}
	curCD = 0;
	this->move(moveSpeed, 0.f);
}

bool Enemy::take_damage(int damage) {
	health -= damage; 
#ifdef DEBUG_MODE
		std::cout << "Enemy took damage\n";
#endif
	if (health <= 0)
		return true;
	return false;
}

Projectile::Projectile(const Projectile &copy): Entity(copy.spriteFileName) {
	damage = copy.damage;
	moveSpeed = copy.moveSpeed;
}

void Projectile::tick(std::vector<Entity *> &entities) {
	bool enemyHit = false;
	int thisIndex = -1;
	for (int i = 0; i < entities.size(); i++) {
		/*got all the information needed from loop*/
		if (thisIndex != -1 && enemyHit)
			break;

		/*found own index in entity list*/
		if (entities[i] == this)
			thisIndex = i;

		/*checking for collision with enemy*/
		if (!enemyHit) {
			Enemy *enemy = dynamic_cast<Enemy *>(entities[i]);
			if (enemy != nullptr && this->intersects(*enemy)) {
				enemyHit = true;
				bool enemyDied = enemy->take_damage(damage);
				if (enemyDied) {
					delete entities[i];
					entities.erase(entities.begin() + i--);
#ifdef DEBUG_MODE
					std::cout << "enemy died. number of entities: " << entities.size() << std::endl;
#endif
				}
			}
		}
	}

	/*destroy self upon hitting an enemy*/
	if (enemyHit) {
		delete this;
		entities.erase(entities.begin() + thisIndex);
#ifdef DEBUG_MODE
		std::cout << "projectile despawned. number of entities: " << entities.size() << std::endl;
#endif
		return;
	}
	else {
		/*if didn't hit an enemy, update position*/
		move(moveSpeed, 0.f);
	}
}
