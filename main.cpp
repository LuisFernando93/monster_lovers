#include<stdio.h>
#include<iostream>
#include<graphics.h>
using namespace std;

#define WIDTH 800
#define HEIGHT 600
#define ESC 27

int main()  { 
	
	char Tecla;
	int pg = 1;
	int gameState = 1;
	long long unsigned gt1, gt2;
  	int fps;
  	int frame;
  	
  	gt1 = GetTickCount();
  	gt2 = gt1 + 1200;
  	fps = 30;

	
	initwindow(WIDTH, HEIGHT);
	
	while(Tecla != ESC) {
  		
  		if (gameState == 1) { //jogo
  			
  			if (gt2 - gt1 > (1000/fps)) {
  				
  				frame = (gt2 - gt1);
  				printf("%d\n", frame);
  				gt1 = gt2;
  				
  				if (pg == 1) pg = 2; else pg = 1;
		  		setactivepage(pg);
				
				cleardevice();
				//update
				//render
				setvisualpage(pg);
				
				frame++;
			}
			
			gt2 = GetTickCount();
  			
  			fflush(stdin);
				
				if (kbhit()) {
		  			Tecla = getch();
		  			if(GetKeyState('A')&0x80) { //esquerda
		  				
					} 
					
					if(GetKeyState('W')&0x80) { //cima
		  				
					}
					
					if(GetKeyState('S')&0x80) { //baixo
		  				
					} 
					
		  			if(GetKeyState('D')&0x80) { //direita
		  				
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

