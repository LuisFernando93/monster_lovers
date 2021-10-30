#include<stdio.h>
#include<iostream>
#include<graphics.h>
using namespace std;

#define WIDTH 800
#define HEIGHT 600
#define ESC 27
#define SPRITE_RES 64

struct Chao {
	int x, y;
	int width, height;
};

struct Parede {
	int x, y;
	int width, height;
};

struct Player {
	int x, y;
	int width, height;
	int speed;
};

void update(Parede *paredes, int nParede, Chao *pisos, int nChao);
void render();
void playerCollision(Parede *paredes, int nParede, Chao *pisos, int nChao);

Player player;

int main()  { 

	Parede parede1, parede2;
	Parede *paredes;
	int nParede = 2;
	Chao chao1, chao2;
	Chao *pisos;
	int nChao = 2;
	
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
	
	paredes = (Parede *)malloc(sizeof(Parede) * nParede);
	pisos = (Chao *)malloc(sizeof(Chao) * nChao);
	
	chao1.x = 0;
	chao1.y = 580;
	chao1.width = 800;
	chao1.height = 20;
	
	chao2.x = 200;
	chao2.y = 300;
	chao2.width = 400;
	chao2.height = 20;
	
	parede1.x = 0;
	parede1.y = 0;
	parede1.width = 20;
	parede1.height = 600;
	
	parede2.x = 780;
	parede2.y = 0;
	parede2.width = 20;
	parede2.height = 600;
	
	paredes[0] = parede1;
	paredes[1] = parede2;
	pisos[0] = chao1;
	pisos[1] = chao2;
	
	while(Tecla != ESC) {
  		
  		if (gameState == 1) { //jogo
  			
  			if (gt2 - gt1 > (1000/fps)) {
  				
  				gt1 = gt2;
  				
  				if (pg == 1) pg = 2; else pg = 1;
		  		setactivepage(pg);
				
				cleardevice();
				update(paredes, nParede, pisos, nChao);
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
	
	free(paredes);
	free(pisos);
}

void update(Parede *paredes, int nParede, Chao *pisos, int nChao) {
	
	playerCollision(paredes, nParede, pisos, nChao);
}

void render() {
	setcolor(COLOR(79, 0, 201));
	setfillstyle(1, COLOR(79, 0, 201));
	bar(20, 20, WIDTH - 20, HEIGHT - 20);
	
	setfillstyle(1, 0);
	bar(200, 300, 600, 320);
	
	setfillstyle(1, COLOR(0, 255, 0));
	bar(player.x, player.y, player.x + player.width, player.y + player.height);
	
//	setfillstyle(1, COLOR(255, 0, 0));
//	for (int i = 0; i <= 1; i++) {
//		bar(pisos[i].x, pisos[i].y, pisos[i].x + pisos[i].width, pisos[i].y + pisos[i].height);
//		bar(paredes[i].x, paredes[i].y, paredes[i].x + paredes[i].width, paredes[i].y + paredes[i].height);
//	}
}

void playerCollision(Parede *paredes, int nParede, Chao *pisos, int nChao) {
	
	for (int i = 0; i <= nParede - 1; i++) { //verifica colisao com parede
		if (!(player.y >= paredes[i].y + paredes[i].height) && !(player.y + player.height <= paredes[i].y)) { //verifica se jogador esta acima ou abaixo da parede
			if (player.x + player.width/2 <= paredes[i].x + paredes[i].width/2) { //verifica se o jogador esta a esquerda da parede
				
				if ((player.x + player.width) > paredes[i].x) {//verifica se o jogador esta entrando na parede
					player.x = paredes[i].x - player.width;
				}
			} else { //jogador a direita da parede
				if (player.x < (paredes[i].x + paredes[i].width)) {//verifica se o jogador esta entrando na parede
					player.x = paredes[i].x + paredes[i].width;
				}
			}	
		}
	}
	
	for (int i = 0; i <= nChao - 1; i++) { //verifica colisao com o chao
		if (!(player.x + player.width <= pisos[i].x) && !(player.x >= pisos[i].x + pisos[i].width)) {// verifica se nao esta a direita ou esqueda do chao
			if (player.y + player.height > pisos[i].y && !(player.y + player.height > pisos[i].y + pisos[i].height/2 )) {
				player.y = pisos[i].y - player.height;
			}
		}
	}
}

