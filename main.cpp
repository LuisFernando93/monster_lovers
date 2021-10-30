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

void update();
void render(Player player);

int main()  { 
	
	Player player;
	
	char Tecla;
	int pg = 1;
	int gameState = 1;
	long long unsigned gt1, gt2;
  	int fps;
  	
  	gt1 = GetTickCount();
  	gt2 = gt1 + 1200;
  	fps = 30;

	
	initwindow(WIDTH, HEIGHT);
	
	player.x = 50;
	player.y = 500;
	player.width = SPRITE_RES;
	player.height = SPRITE_RES;
	player.speed = 5;
	
	while(Tecla != ESC) {
  		
  		if (gameState == 1) { //jogo
  			
  			if (gt2 - gt1 > (1000/fps)) {
  				
  				gt1 = gt2;
  				
  				if (pg == 1) pg = 2; else pg = 1;
		  		setactivepage(pg);
				
				cleardevice();
				//update
				render(player);
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
}

void render(Player player) {
	setcolor(COLOR(79, 0, 201));
	setfillstyle(1, COLOR(79, 0, 201));
	bar(20, 20, WIDTH - 20, HEIGHT - 20);
	
	setfillstyle(1, 0);
	bar(200, 300, 600, 320);
	
	setfillstyle(1, COLOR(0, 255, 0));
	bar(player.x, player.y, player.x + player.width, player.y + player.height);
}

