#include "Player.h"

void move(Player& player, int x, int y) {

	player.x += player.speed * x;
	player.y += player.speed * y;
}