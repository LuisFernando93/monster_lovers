#include<stdio.h>
#include<iostream>
#include<graphics.h>
#include<math.h>
using namespace std;

#define WIDTH 640
#define HEIGHT 248
#define SCALE 2 
#define ESC 27
#define ENTER 13
#define SPRITE_RES 128
#define GRAVITY 2

struct Floor {
	int x, y;
	int width, height;
};

struct Wall {
	int x, y;
	int width, height;
};

struct Player {
	int x, y;
	int width, height;
	int life, power, speed;
	bool attack, jump, move, moveLeft, moveRight, lookRight;
	bool damaged;
	int timerDamaged;
	int jumpHeight;
	double vspd;
};

struct Enemy {
	int x, y;
	int life, power;
	int width, height;
	int timer;
	bool damaged;
};

struct Position {
	int x,y;
};

struct Pellet {
	double x,y;
	double vx, vy;
	int width, height;
};

void update(Wall *walls, int nWall, Floor *floors, int nFloor, Position *positions);
void render(Wall *walls, int nWall, Floor *floors, int nFloor);
void updatePlayer(Wall *walls, int nWall, Floor *floors, int nFloor);
void playerCollision(Wall *walls, int nWall, Floor *floors, int nFloor);
void playerMove();
void playerJump(Floor *floors, int Floor);
void playerAttack();
void playerCheckLife();
void freeFall(Floor *floors, int nFloor);
void updateEnemy(Position *positions);
void enemyMove(Position *positions);
void enemyAttack(Position *positions);
void enemyCheckLife();
void updatePellets();
Pellet createPellet(int number, int position);
void addPellet(Pellet);
void removePellet(int i);
void clearPellets();

Player player;
Enemy enemy;

Pellet *pelletsGb;
int nPelletsGb = 0;

int gameStateGb = 1; //menu = 0; jogo = 1, creditos = 2, game over = 3

void *imgScene;

int main()  { 

	Wall wall1, wall2;
	Wall *walls;
	int nWall = 2;
	Floor floor1, floor2, floor3;
	Floor *floors;
	int nFloor = 3;
	Position pos0, pos1, pos2, pos3;
	Position *positions;
	Pellet *pellets;
	
	char Tecla;
	int pg = 1;
	long long unsigned gt1, gt2;
  	int fps = 60;
  	
  	bool restart = false;
  	
  	initwindow(WIDTH*SCALE, HEIGHT*SCALE);
  	
  	gt1 = GetTickCount();
  	gt2 = gt1 + 1200;
  	
  	int WImgScene, HImgScene, sizeImgScene;
  	
  	WImgScene = 1280;
  	HImgScene = 496;
  	
  	readimagefile(".\\res\\cenario.bmp", 0, 0, WImgScene - 1, HImgScene - 1);
  	sizeImgScene = imagesize(0, 0, WImgScene - 1, HImgScene - 1);
  	imgScene = malloc(sizeImgScene);
	getimage(0, 0, WImgScene-1, HImgScene-1, imgScene);
	
	walls = (Wall *)malloc(sizeof(Wall) * nWall);
	floors = (Floor *)malloc(sizeof(Floor) * nFloor);
	positions = (Position *)malloc(sizeof(Position) * 4);
	
	floor1.x = 0;
	floor1.y = HEIGHT*SCALE - 10;
	floor1.width = WIDTH*SCALE;
	floor1.height = 20;
	
	floor2.x = 0;
	floor2.y = (HEIGHT - 15 - 60)*SCALE;
	floor2.width = (WIDTH - 180)*SCALE/2;
	floor2.height = 20;
	
	floor3.x = (WIDTH/2 + 70)*SCALE;
	floor3.y = (HEIGHT - 15 - 60)*SCALE;
	floor3.width = (WIDTH - 180)*SCALE/2;
	floor3.height = 20;
	
	wall1.x = 0;
	wall1.y = 0;
	wall1.width = 35*SCALE;
	wall1.height = HEIGHT*SCALE;
	
	wall2.x = WIDTH*SCALE - 20;
	wall2.y = 0;
	wall2.width = 20;
	wall2.height = HEIGHT*SCALE;
	
	walls[0] = wall1;
	walls[1] = wall2;
	floors[0] = floor1;
	floors[1] = floor2;
	floors[2] = floor3;
	
	player.width = SPRITE_RES;
	player.height = SPRITE_RES;
	player.x = (WIDTH/2 - 180 )*SCALE;
	player.y = floor2.y - player.height;
	player.speed = 8;
	player.life = 1;
	player.power = 1;
	player.attack = false;
	player.jump = false;
	player.move = false;
	player.moveLeft = false;
	player.moveRight = false;
	player.lookRight = true;
	player.damaged = false;
	player.jumpHeight = 30;
	player.vspd = 0;
	
	enemy.width = SPRITE_RES;
	enemy.height = SPRITE_RES;
		
	pos0.x = 30*SCALE;
	pos0.y = floor1.y - enemy.height;
	
	pos1.x = 50*SCALE;
	pos1.y = floor2.y - enemy.height;
	
	pos2.x = (WIDTH - 50)*SCALE - enemy.width;
	pos2.y = floor3.y - enemy.height;
	
	pos3.x = (WIDTH - 30)*SCALE - enemy.width;
	pos3.y = floor1.y - enemy.height;
	
	positions[0] = pos0;
	positions[1] = pos1;
	positions[2] = pos2;
	positions[3] = pos3;
	
	int initialPosition = 2;

	enemy.x = positions[initialPosition].x;
	enemy.y = positions[initialPosition].y;
	enemy.life = 8;
	enemy.power = 1;
	enemy.timer = 0;
	enemy.damaged = false;
	
	while(Tecla != ESC) {
   			
		if (gt2 - gt1 > (1000/fps)) {
			
			gt1 = gt2;
			
			if (pg == 1) pg = 2; else pg = 1;
	  		setactivepage(pg);
			
			cleardevice();
			update(walls, nWall, floors, nFloor, positions);
			render(walls, nWall, floors, nFloor);
			setvisualpage(pg);
			
		}
		
		gt2 = GetTickCount();
		
		fflush(stdin);
		
		if(gameStateGb == 1) { //jogo
			
			if(GetKeyState('A')&0x80) { //esquerda
				player.move = true;
				player.moveLeft = true;
				player.moveRight = false;
				player.lookRight = false;
			} 
			
			if(GetKeyState('W')&0x80) { //cima
				player.jump = true;
			}
			
			if(GetKeyState('S')&0x80) { //baixo
				
			} 
			
			if(GetKeyState('D')&0x80) { //direita
				player.move = true;
				player.moveLeft = false;
				player.moveRight = true;
				player.lookRight = true;
			} 
			
			if(GetKeyState(VK_SPACE)&0x80) { //jump
				player.jump = true;
			}
			
			if(GetKeyState('R')&0x80) { //ataque
				player.attack = true;
			} 
			
		} else if(gameStateGb == 2) { //creditos
			
			if(GetKeyState(ENTER)&0x80) { //restart
				restart = true;
				printf("restart");
			}	

		} else if(gameStateGb == 3) { //game over
		
			if(GetKeyState(ENTER)&0x80) { //restart
				restart = true;
				printf("restart");
			}

		}
			
		if (kbhit()) {
	  		Tecla = getch();	
		} 

	}
	free(walls);
	free(floors);
	free(positions);
	free(pelletsGb);
	free(imgScene);
}

void update(Wall *walls, int nWall, Floor *floors, int nFloor, Position *positions) {
	
	if(gameStateGb == 1) { //jogo
		updatePlayer(walls, nWall, floors, nFloor);
		playerJump(floors, nFloor);
		freeFall(floors, nFloor);
		updateEnemy(positions);
		updatePellets();
	}
	
}

void render(Wall *walls, int nWall, Floor *floors, int nFloor) {
	
	if (gameStateGb == 1) {		
		
		putimage(0, 0, imgScene, COPY_PUT);
			
	//	setfillstyle(1, COLOR(255, 255, 0));
	//	for (int i = 0; i < nWall; i++) {
	//		bar(walls[i].x, walls[i].y, walls[i].x + walls[i].width, walls[i].y + walls[i].height);
	//	}
	//	for (int i = 0; i < nFloor; i++) {
	//		bar(floors[i].x, floors[i].y, floors[i].x + floors[i].width, floors[i].y + floors[i].height);
	//	}
		
		setfillstyle(1, COLOR(255, 0, 0));
		bar(enemy.x, enemy.y, enemy.x + enemy.width, enemy.y + enemy.height);
		
		setfillstyle(1, COLOR(0, 255, 0));
		bar(player.x, player.y, player.x + player.width, player.y + player.height);
		
		setfillstyle(1, COLOR(255, 255, 0));
		if (nPelletsGb > 0) {
			for (int i = 0; i < nPelletsGb; i++) {
				bar(pelletsGb[i].x, pelletsGb[i].y, pelletsGb[i].x + pelletsGb[i].width, pelletsGb[i].y + pelletsGb[i].height);
			}
		}
	} else if (gameStateGb == 2) { //creditos
		setfillstyle(1, COLOR(0, 255, 0));
		bar(0, 0, WIDTH*SCALE, HEIGHT*SCALE);
		printf("Vitoria ");
		
	} else if (gameStateGb == 3) { //game over
		setfillstyle(1, COLOR(255, 0, 0));
		bar(0, 0, WIDTH*SCALE, HEIGHT*SCALE);
		printf("Game Over ");
	}

}
void updatePlayer(Wall *walls, int nWall, Floor *floors, int nFloor) {
	
	playerCollision(walls, nWall, floors, nFloor);
	playerMove();
	playerAttack();
	playerCheckLife();
}

void playerCollision(Wall *walls, int nWall, Floor *floors, int nFloor) {
	
	for (int i = 0; i <= nWall - 1; i++) { //verifica colisao com parede
		if (!(player.y >= walls[i].y + walls[i].height) && !(player.y + player.height <= walls[i].y)) { //verifica se jogador esta acima ou abaixo da parede
			if (player.x + player.width/2 <= walls[i].x + walls[i].width/2) { //verifica se o jogador esta a esquerda da parede
				
				if ((player.x + player.width) > walls[i].x) {//verifica se o jogador esta entrando na parede
					player.x = walls[i].x - player.width;
				}
			} else { //jogador a direita da parede
				if (player.x < (walls[i].x + walls[i].width)) {//verifica se o jogador esta entrando na parede
					player.x = walls[i].x + walls[i].width;
				}
			}	
		}
	}
	
	for (int i = 0; i <= nFloor - 1; i++) { //verifica colisao com o chao
		if (!(player.x + player.width <= floors[i].x) && !(player.x >= floors[i].x + floors[i].width)) {// verifica se nao esta a direita ou esqueda do chao
			if (player.y + player.height > floors[i].y && !(player.y + player.height > floors[i].y + floors[i].height/2 )) {
				player.y = floors[i].y - player.height;
			}
		}
	}
}

void playerMove() {
	if (player.move) {
		if (player.moveLeft) {
			player.x -= player.speed;
			player.moveLeft = false;
		} else if (player.moveRight) {
			player.x += player.speed;
			player.moveRight = false;
		}
		player.move = false;
	}
}

void playerJump(Floor *floors, int nFloor) { //pulo do jogador
	
	if (player.jump) {
			for (int i = 0; i <= nFloor; i++) {
			if (!(player.x + player.width <= floors[i].x) && !(player.x >= floors[i].x + floors[i].width)) {
				if ((player.y+player.height >= floors[i].y) && (player.y+player.height <= floors[i].y + floors[i].height) && player.vspd == 0) {
					player.vspd = -(player.jumpHeight);
				}
			}
		}
	}
	player.jump = false;
}

void playerAttack() {
	if (player.attack) {
		double distance;
		if (player.lookRight) {
			distance = sqrt(pow(player.x + player.width - enemy.x, 2 ) + pow(player.y - enemy.y, 2));
		} else {
			distance = sqrt(pow(player.x - enemy.x - enemy.width, 2) + pow(player.y - enemy.y, 2));
		}
		printf("attack distance: %f ", distance);
		if (distance <= 100 && !enemy.damaged) {
			enemy.life -= player.power;
			enemy.damaged = true;
			enemy.timer = 0;
			printf("hit! enemy: %d ", enemy.life);
		}
		player.attack = false;
	}
}

void playerCheckLife() {
	
	if(player.life <= 0) { //player derrotado. game over
		gameStateGb = 3;
		return;
	}
	
	if(player.damaged) { //player machucado. frames de invencibilidade
		player.timerDamaged++;
		if(player.timerDamaged >= 40) {
			player.timerDamaged = 0;
			player.damaged = false;
		}
	}
}

void freeFall(Floor *floors, int nFloor) { //queda livre
	
	player.vspd += GRAVITY;
	if (player.vspd > 0) {
		for (int i = 0; i <= nFloor - 1; i++) { //verifica colisao com o chao
			if (!(player.x + player.width <= floors[i].x) && !(player.x >= floors[i].x + floors[i].width)) {
				if (player.y+player.height+player.vspd >= floors[i].y && player.y + player.height <= floors[i].y) {
					player.y = floors[i].y - player.height;
					player.vspd = 0;
					return;
				}
			}
		}
	}
	player.y += (int)player.vspd;
}

void updateEnemy(Position *positions) { //o inimigo utiliza o timer para fazer suas acoes. 
	enemyCheckLife();
	enemy.timer++;
	if (!enemy.damaged) {
		if (enemy.timer == 30) {
			enemyAttack(positions);
		} else if (enemy.timer == 90) {
			enemyAttack(positions);
		} else if (enemy.timer == 150) {
			enemyAttack(positions);
		} else if (enemy.timer == 240) {
			enemyMove(positions);
			enemy.timer = 0;
		}
	} else {
		if (enemy.timer == 40) {
			enemyMove(positions);
			enemy.damaged = false;
			enemy.timer = 0;
		}
	}
	
}

void enemyMove(Position *positions) {//movimento do inimigo
	
	int enemyPos, nextPos;
	
	//verifica a posicao atual do inimigo
	if (enemy.x == positions[0].x && enemy.y == positions[0].y) {
		enemyPos = 0;
	} else if (enemy.x == positions[1].x && enemy.y == positions[1].y) {
		enemyPos = 1;
	} else if (enemy.x == positions[2].x && enemy.y == positions[2].y) {
		enemyPos = 2;
	} else if (enemy.x == positions[3].x && enemy.y == positions[3].y) {
		enemyPos = 3;
	}
	
	nextPos = enemyPos;
	while (nextPos == enemyPos) { //escolhe uma nova posicao aleatoria, diferente da atual
		nextPos = rand()%4;
	}

	
	//reposiciona o inimigo
	enemy.x = positions[nextPos].x;
	enemy.y = positions[nextPos].y;
}

void enemyAttack(Position *positions) {
	
	int enemyPos;
	Pellet pellet1, pellet2;
	
	//verifica a posicao atual do inimigo
	if (enemy.x == positions[0].x && enemy.y == positions[0].y) { //posicao 0

		pellet1 = createPellet(1, 0);
		pellet2 = createPellet(2, 0);
		
	} else if (enemy.x == positions[1].x && enemy.y == positions[1].y) { //posicao 1

		pellet1 = createPellet(1, 1);
		pellet2 = createPellet(2, 1);
		
	} else if (enemy.x == positions[2].x && enemy.y == positions[2].y) { //posicao 2

		pellet1 = createPellet(1, 2);
		pellet2 = createPellet(2, 2);
		
		
	} else if (enemy.x == positions[3].x && enemy.y == positions[3].y) { //posicao 3

		pellet1 = createPellet(1, 3);
		pellet2 = createPellet(2, 3);
		
	} else {
		return;
	}
	
	addPellet(pellet1);
	addPellet(pellet2);
	
}

void enemyCheckLife() {
	if (enemy.life <= 0) { //inimigo derrotado. vitoria.
		gameStateGb = 2;
	}
}

void updatePellets() {
	if (nPelletsGb > 0) {
		for (int i = 0; i < nPelletsGb; i++) {
			pelletsGb[i].x += pelletsGb[i].vx;
			pelletsGb[i].y += pelletsGb[i].vy;
			
			if (pelletsGb[i].x <= (0*SCALE) || pelletsGb[i].x >= (WIDTH*SCALE) || pelletsGb[i].y <= (0*SCALE) || pelletsGb[i].y >= (HEIGHT*SCALE)) {
				removePellet(i);
			}
			
			if (pelletsGb[i].x >= player.x && pelletsGb[i].x - 1 <= player.x + player.width && pelletsGb[i].y >= player.y && pelletsGb[i].y - 1  <= player.y + player.height) { //acerta player
				if (!player.damaged) {
					removePellet(i);
					player.life -= enemy.power;
					player.damaged = true;
					printf("Damaged player: %d ", player.life);
				}
			}				
		}
	}
}

Pellet createPellet(int number, int position) {
	
	Pellet pellet;
	int speedX = 5;
	int speedY1 = 0;
	int speedY2 = 2;
	
	pellet.height = 8;
	pellet.width = 8;
	pellet.y = enemy.y + enemy.height/2 - pellet.height/2;
	
	if (number == 1) {
		
		if (position == 0) {
			
			pellet.x = enemy.x + enemy.width;
			pellet.vx = speedX;
			pellet.vy = speedY1;
			
		} else if(position == 1) {
			
			pellet.x = enemy.x + enemy.width;
			pellet.vx = speedX;
			pellet.vy = speedY1;
			
		} else if(position == 2) {
			
			pellet.x = enemy.x;
			pellet.vx = -speedX;
			pellet.vy = speedY1;
			
		} else if(position == 3) {
			
			pellet.x = enemy.x;
			pellet.vx = -speedX;
			pellet.vy = speedY1;
			
		}
		
	} else if (number == 2) {
		
		if (position == 0) {
			
			pellet.x = enemy.x + enemy.width;
			pellet.vx = speedX;
			pellet.vy = -speedY2;
			
		} else if(position == 1) {
			
			pellet.x = enemy.x + enemy.width;
			pellet.vx = speedX;
			pellet.vy = speedY2;
			
		} else if(position == 2) {
			
			pellet.x = enemy.x;
			pellet.vx = -speedX;
			pellet.vy = speedY2;
			
		} else if(position == 3) {
			
			pellet.x = enemy.x;
			pellet.vx = -speedX;
			pellet.vy = -speedY2;
			
		}
	}
	return pellet;
}

void addPellet(Pellet pellet) {
	nPelletsGb++;
	pelletsGb = (Pellet *)realloc(pelletsGb, sizeof(Pellet) * nPelletsGb);
	pelletsGb[nPelletsGb-1] = pellet;
}

void removePellet(int removeIndex) {
	if (nPelletsGb > 0) {
		if (removeIndex != nPelletsGb - 1) {
			for(int i = 0; i < (nPelletsGb-1); i++) {
				if (i >= removeIndex) {
					pelletsGb[i] = pelletsGb[i+1];
				}
			}
		}
		nPelletsGb--;
		pelletsGb = (Pellet *)realloc(pelletsGb, sizeof(Pellet) * nPelletsGb);
	}
}

void clearPellets() {
	free(pelletsGb);
	nPelletsGb = 0;
	pelletsGb = (Pellet *)malloc(sizeof(Pellet) * nPelletsGb);
}
