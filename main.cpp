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
	
	while(Tecla != ESC) {
  		
  		if (gameState == 1) { //jogo
  		
			if (pg == 1) pg = 2; else pg = 1;
	  		setactivepage(pg);
			cleardevice();
	  		
			//update
			//render
			
			setvisualpage(pg);
			
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
		} else if (gameState == 0) { //game over
		
			fflush(stdin);
			if (kbhit()) {
	  			
			} 
			
		}

	}
}

