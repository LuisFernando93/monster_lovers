#include<stdio.h>
#include<iostream>
#include<graphics.h>
using namespace std;

#define WIDTH 800
#define HEIGHT 600
#define ESC 27
#define SPRITE_RES 64

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
};

void update(Wall *walls, int nWall, Floor *floors, int nFloor);
void render();
void playerCollision(Wall *walls, int nWall, Floor *floors, int nFloor);
void freeFall(Floor *floors, int nFloor);

Player player;

int main()  { 

	Wall wall1, wall2;
	Wall *walls;
	int nWall = 2;
	Floor floor1, floor2, floor3;
	Floor *floors;
	int nFloor = 3;
	
	char Tecla;
	int pg = 1;
	int gameState = 1;
	long long unsigned gt1, gt2;
  	int fps;
  	
  	gt1 = GetTickCount();
  	gt2 = gt1 + 1200;
  	fps = 60;
	
	initwindow(WIDTH, HEIGHT);
	
	player.x = 50;
	player.y = 500;
	player.width = SPRITE_RES;
	player.height = SPRITE_RES;
	player.speed = 5;
	
	walls = (Wall *)malloc(sizeof(Wall) * nWall);
	floors = (Floor *)malloc(sizeof(Floor) * nFloor);
	
	floor1.x = 0;
	floor1.y = 580;
	floor1.width = 800;
	floor1.height = 20;
	
	floor2.x = 0;
	floor2.y = 380;
	floor2.width = 300;
	floor2.height = 20;
	
	floor3.x = 500;
	floor3.y = 380;
	floor3.width = 300;
	floor3.height = 20;
	
	wall1.x = 0;
	wall1.y = 0;
	wall1.width = 20;
	wall1.height = 600;
	
	wall2.x = 780;
	wall2.y = 0;
	wall2.width = 20;
	wall2.height = 600;
	
	walls[0] = wall1;
	walls[1] = wall2;
	floors[0] = floor1;
	floors[1] = floor2;
	
	while(Tecla != ESC) {
  		
  		if (gameState == 1) { //jogo
  			
  			if (gt2 - gt1 > (1000/fps)) {
  				
  				gt1 = gt2;
  				
  				if (pg == 1) pg = 2; else pg = 1;
		  		setactivepage(pg);
				
				cleardevice();
				update(walls, nWall, floors, nFloor);
				render();
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
		  				player.y -= player.speed;
					}
					
					if(GetKeyState('S')&0x80) { //baixo
		  				player.y += player.speed;
					} 
					
		  			if(GetKeyState('D')&0x80) { //direita
		  				player.x += player.speed;
					} 
					
		  			if(GetKeyState(VK_SPACE)&0x80) { //jump
		  				
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
}

void update(Wall *walls, int nWall, Floor *floors, int nFloor) {
	
	playerCollision(walls, nWall, floors, nFloor);
}

void render() {
	setcolor(COLOR(79, 0, 201));
	setfillstyle(1, COLOR(79, 0, 201));
	bar(20, 20, WIDTH - 20, HEIGHT - 20);
	
	setfillstyle(1, 0);
	bar(0, 380, 300, 400);
	bar(500, 380, 800, 400);
	
	setfillstyle(1, COLOR(0, 255, 0));
	bar(player.x, player.y, player.x + player.width, player.y + player.height);
	
//	setfillstyle(1, COLOR(255, 0, 0));
//	for (int i = 0; i <= 1; i++) {
//		bar(pisos[i].x, pisos[i].y, pisos[i].x + pisos[i].width, pisos[i].y + pisos[i].height);
//		bar(paredes[i].x, paredes[i].y, paredes[i].x + paredes[i].width, paredes[i].y + paredes[i].height);
//	}
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

void freeFall(Floor *floors, int nFloor) {
	
}

