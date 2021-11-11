#include<stdio.h>
#include<iostream>
#include<graphics.h>
using namespace std;

#define WIDTH 640
#define HEIGHT 248
#define SCALE 1
#define ESC 27
#define SPRITE_RES 64
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
	int speed;
	bool jump;
	int jumpHeight;
	double vspd;
};

struct Enemy {
	int x, y;
	int width, height;
	int timer;
};

struct Position {
	int x,y;
};

void update(Wall *walls, int nWall, Floor *floors, int nFloor, Position *positions);
void render(Wall *walls, int nWall, Floor *floors, int nFloor);
void playerCollision(Wall *walls, int nWall, Floor *floors, int nFloor);
void playerJump(Floor *floors, int Floor);
void playerAttack();
void freeFall(Floor *floors, int nFloor);
void enemyMove(Position *positions);
void enemyAttack();
void enemyAI(Position *positions);

Player player;
Enemy enemy;

void *imgCenario;

int main()  { 

	Wall wall1, wall2;
	Wall *walls;
	int nWall = 2;
	Floor floor1, floor2, floor3;
	Floor *floors;
	int nFloor = 3;
	Position pos0, pos1, pos2, pos3;
	Position *positions;
	
	char Tecla;
	int pg = 1;
	int gameState = 1;
	long long unsigned gt1, gt2;
  	int fps;
  	
  	initwindow(WIDTH*SCALE, HEIGHT*SCALE);
  	
  	gt1 = GetTickCount();
  	gt2 = gt1 + 1200;
  	fps = 60;
  	
  	int LImgCenario, AImgCenario, TamImgCenario;
  	
  	LImgCenario = 640;
  	AImgCenario = 248;
  	
  	readimagefile("cenario.bmp", 0, 0, 640 - 1, 248 - 1);
  	TamImgCenario = imagesize(0, 0, 640-1, 248-1);
  	imgCenario = malloc(TamImgCenario);
	getimage(0, 0, LImgCenario-1, AImgCenario-1, imgCenario);
	
	walls = (Wall *)malloc(sizeof(Wall) * nWall);
	floors = (Floor *)malloc(sizeof(Floor) * nFloor);
	positions = (Position *)malloc(sizeof(Position) * 4);
	
	floor1.x = 0;
	floor1.y = HEIGHT*SCALE -20;
	floor1.width = WIDTH*SCALE;
	floor1.height = 20;
	
	floor2.x = 0;
	floor2.y = (HEIGHT - 20 - 60)*SCALE;
	floor2.width = (WIDTH - 180)*SCALE/2;
	floor2.height = 20;
	
	floor3.x = (WIDTH/2 + 70)*SCALE;
	floor3.y = (HEIGHT - 20 - 60)*SCALE;
	floor3.width = (WIDTH - 180)*SCALE/2;
	floor3.height = 20;
	
	wall1.x = 0;
	wall1.y = 0;
	wall1.width = 20;
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
	
	player.width = SPRITE_RES*SCALE;
	player.height = SPRITE_RES*SCALE;
	player.x = 50*SCALE;
	player.y = floor2.y - player.height;
	player.speed = 5;
	player.jump = false;
	player.jumpHeight = 30;
	player.vspd = 0;
	
	enemy.width = SPRITE_RES*SCALE;
	enemy.height = SPRITE_RES*SCALE;
	enemy.x = (WIDTH - 50)*SCALE - enemy.width;
	enemy.y = floor3.y - enemy.height;
	enemy.timer = 0;
	
		
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
	
	while(Tecla != ESC) {
  		
  		if (gameState == 1) { //jogo
  			
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
				
				if (kbhit()) {
		  			Tecla = getch();
		  			if(GetKeyState('A')&0x80) { //esquerda
		  				player.x -= player.speed;
					} 
					
					if(GetKeyState('W')&0x80) { //cima
		  				player.jump = true;
					}
					
					if(GetKeyState('S')&0x80) { //baixo
		  				
					} 
					
		  			if(GetKeyState('D')&0x80) { //direita
		  				player.x += player.speed;
					} 
					
		  			if(GetKeyState(VK_SPACE)&0x80) { //jump
		  				player.jump = true;
					}
					
					if(GetKeyState('R')&0x80) { //ataque
		  				
					} 
						
				} 
  			
		} else if (gameState == 0) { //menu
		
			fflush(stdin);
			if (kbhit()) {
	  			
			} 
			
		}

	}
	free(walls);
	free(floors);
	free(positions);
	free(imgCenario);
}

void update(Wall *walls, int nWall, Floor *floors, int nFloor, Position *positions) {
	
	playerCollision(walls, nWall, floors, nFloor);
	playerJump(floors, nFloor);
	freeFall(floors, nFloor);
	enemyAI(positions);
	
}

void render(Wall *walls, int nWall, Floor *floors, int nFloor) {
	
	setcolor(COLOR(79, 0, 201));
	setfillstyle(1, COLOR(79, 0, 201));
	bar(20, 20, WIDTH*SCALE - 20, HEIGHT*SCALE - 20);
	
	putimage(0, 0, imgCenario, COPY_PUT);
		
	setfillstyle(1, COLOR(255, 255, 0));
	for (int i = 0; i <= nWall; i++) {
		bar(walls[i].x, walls[i].y, walls[i].x + walls[i].width, walls[i].y + walls[i].height);
	}
	for (int i = 0; i <= nFloor; i++) {
		bar(floors[i].x, floors[i].y, floors[i].x + floors[i].width, floors[i].y + floors[i].height);
	}
	
	setfillstyle(1, COLOR(255, 0, 0));
	bar(enemy.x, enemy.y, enemy.x + enemy.width, enemy.y + enemy.height);
	
	setfillstyle(1, COLOR(0, 255, 0));
	bar(player.x, player.y, player.x + player.width, player.y + player.height);

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

void playerJump(Floor *floors, int nFloor) { //pulo do jogador
	
	if (player.jump) {
			for (int i = 0; i <= nFloor; i++) {
			if (!(player.x + player.width <= floors[i].x) && !(player.x >= floors[i].x + floors[i].width)) {
				if ((player.y+player.height >= floors[i].y) && (player.y+player.height <= floors[i].y + floors[i].height)) {
					player.vspd = -(player.jumpHeight);
				}
			}
		}
	}
	player.jump = false;
}

void playerAttack() {
	
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

void enemyAttack() {
	
}

void enemyAI(Position *positions) { //o inimigo utiliza o timer para fazer suas acoes. ao executar, retorna o novo valor do timer
	enemy.timer++;
	if (enemy.timer >= 60) {
		enemyMove(positions);
		enemy.timer = 0;
	}
}
