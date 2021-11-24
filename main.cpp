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
#define MAX_SPRITES 220
#define MAX_PLAYER_MOVE_INDEX 13
#define MAX_PLAYER_IDLE_INDEX 8
#define MAX_PLAYER_ATTACK_INDEX 13
#define MAX_PLAYER_DAMAGED_INDEX 5
#define MAX_PLAYER_DEATH_INDEX 14
#define MAX_ENEMY_TELEPORT_INDEX 11
#define MAX_ENEMY_IDLE_INDEX 5
#define MAX_ENEMY_ATTACK_INDEX 11
#define MAX_ENEMY_DAMAGED_INDEX 9
#define MAX_ENEMY_DEATH_INDEX 14

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
	bool attack, attackDamage, jump, move, moveLeft, moveRight, lookRight, falling;
	bool damaged, invincible, dead;
	int timerDamaged;
	int jumpHeight;
	double vspd;
	int idleIndex, moveIndex, attackIndex, damagedIndex, deathIndex;
};

struct Enemy {
	int x, y;
	int life, power;
	int width, height;
	int timer;
	bool attack, teleport, move, lookRight;
	bool damaged, invincible, dead;
	int idleIndex, teleportIndex, attackIndex, damagedIndex, deathIndex;
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
void renderPlayer();
void playerCollision(Wall *walls, int nWall, Floor *floors, int nFloor);
void playerMove();
void resetPlayerMove();
void playerJump(Floor *floors, int nFloor);
void playerAttack();
void cancelPlayerAttack();
void playerCheckLife();
void freeFall(Floor *floors, int nFloor);
void updateEnemy(Position *positions);
void renderEnemy();
void enemyMove(Position *positions);
void enemyAttack(Position *positions);
void enemyCheckLife();
void updatePellets();
Pellet createPellet(int number, int position);
void addPellet(Pellet);
void removePellet(int i);
void clearPellets();
void ui();
void newGame(Floor *floors, int nFloor, Position *positions, int initPos);
void changeGameState(int newState);

Player player;
Enemy enemy;

Pellet *pelletsGb;
int nPelletsGb = 0;

int gameStateGb = -1; //menu = 0; jogo = 1, creditos = 2, game over = 3

void *imgScene, *imgCredits, *imgGameover, *imgMenu, *imgControls, *imgOptions;
void *sprites[MAX_SPRITES], *spriteMasks[MAX_SPRITES];
void *imgUI[8];

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
	long long unsigned gt1, gt2;
  	int fps = 60;
  	
  	bool restart = false;
  	
  	mciSendString("open .\\res\\audio\\Uberpunch(game).mp3 type MPEGVideo alias game", NULL, 0, 0); 
  	mciSendString("open .\\res\\audio\\OneDestination-TwoJourneys(menu).mp3 type MPEGVideo alias menu", NULL, 0, 0);  
	mciSendString("open .\\res\\audio\\DreamsOfVain(gameover).mp3 type MPEGVideo alias gameover", NULL, 0, 0);  
  	mciSendString("open .\\res\\audio\\AfterTheEnd(credits).mp3 type MPEGVideo alias credits", NULL, 0, 0); 
  	
  	waveOutSetVolume(0,0x44444444);
  	
  	gt1 = GetTickCount();
  	gt2 = gt1 + 1200;
  	
	initwindow(1280, 3072);
  	
	int WImgSpritesheet, HImgSpritesheet, sizeSprite;
	
	WImgSpritesheet = 1280;
	HImgSpritesheet = 3072;
	
	sizeSprite = imagesize(0, 0, SPRITE_RES-1, SPRITE_RES-1);
	for(int i = 0; i < MAX_SPRITES; i++) {
		sprites[i] = malloc(sizeSprite);
		spriteMasks[i] = malloc(sizeSprite);
	}
	
	readimagefile(".\\res\\image\\spritesheet.bmp", 0, 0, WImgSpritesheet - 1, HImgSpritesheet - 1);
	for(int j = 0; j < 20; j++) {
		for(int i = 0; i < 10; i++) {
			getimage(SPRITE_RES*i, SPRITE_RES*j, SPRITE_RES*i + SPRITE_RES-1, SPRITE_RES*j + SPRITE_RES-1, sprites[i + j*10]);
		}
	}
	
	
	readimagefile(".\\res\\image\\mask.bmp", 0, 0, WImgSpritesheet - 1, HImgSpritesheet - 1);
	for(int j = 0; j < 20; j++) {
		for(int i = 0; i < 10; i++) {
			getimage(SPRITE_RES*i, SPRITE_RES*j, SPRITE_RES*i + SPRITE_RES-1, SPRITE_RES*j + SPRITE_RES-1, spriteMasks[i + j*10]);
		}
	}
	
	closegraph();
	
	initwindow(WIDTH*SCALE, HEIGHT*SCALE);
  	
  	int WImgScene, HImgScene, sizeImgScene;
  	
  	WImgScene = 1280;
  	HImgScene = 496;
  	
  	readimagefile(".\\res\\image\\cenario.bmp", 0, 0, WImgScene - 1, HImgScene - 1);
  	sizeImgScene = imagesize(0, 0, WImgScene - 1, HImgScene - 1);
  	imgScene = malloc(sizeImgScene);
	getimage(0, 0, WImgScene-1, HImgScene-1, imgScene);
	
	int WImgCredits, HImgCredits, sizeImgCredits;
	
	WImgCredits = 800;
	HImgCredits = 496;
	
	readimagefile(".\\res\\image\\creditos.bmp", 0, 0, WImgCredits - 1, HImgCredits - 1);
	sizeImgCredits = imagesize(0, 0, WImgCredits - 1, HImgCredits - 1);
	imgCredits = malloc(sizeImgCredits);
	getimage(0, 0, WImgCredits - 1, HImgCredits - 1, imgCredits);
	
	int WImgGameover, HImgGameover, sizeImgGameover;
	
	WImgGameover = 800;
	HImgGameover = 496;
	
	readimagefile(".\\res\\image\\gameover.bmp", 0, 0, WImgGameover - 1, HImgGameover - 1);
	sizeImgGameover = imagesize(0, 0, WImgGameover - 1, HImgGameover - 1);
	imgGameover = malloc(sizeImgGameover);
	getimage(0, 0, WImgGameover - 1, HImgGameover - 1, imgGameover);
	
	int WImgMenu, HImgMenu, sizeImgMenu;
	
	WImgMenu = 800;
	HImgMenu = 496;
	
	readimagefile(".\\res\\image\\menu.bmp", 0, 0, WImgMenu - 1, HImgMenu - 1);
	sizeImgMenu = imagesize(0, 0, WImgMenu - 1, HImgMenu - 1);
	imgMenu = malloc(sizeImgMenu);
	getimage(0, 0, WImgMenu - 1, HImgMenu - 1, imgMenu);
	
	int WImgControls, HImgControls, sizeImgControls;
	
	WImgControls = 800;
	HImgControls = 496;
	
	readimagefile(".\\res\\image\\controles.bmp", 0, 0, WImgControls - 1, HImgControls - 1);
	sizeImgControls = imagesize(0, 0, WImgControls - 1, HImgControls - 1);
	imgControls = malloc(sizeImgControls);
	getimage(0, 0, WImgControls - 1, HImgControls - 1, imgControls);
	
	int WImgOptions, HImgOptions, sizeImgOptions;
	
	WImgOptions = 800;
	HImgOptions = 496;
	
	readimagefile(".\\res\\image\\opcoes.bmp", 0, 0, WImgOptions - 1, HImgOptions - 1);
	sizeImgOptions = imagesize(0, 0, WImgOptions - 1, HImgOptions - 1);
	imgOptions = malloc(sizeImgOptions);
	getimage(0, 0, WImgOptions - 1, HImgOptions - 1, imgOptions);
	
	int WImgUI, HImgUI, sizeImgUI, WProjetil, HProjetil, sizeImgProjetil;
	
	WImgUI = 46;
	HImgUI = 64;
	sizeImgUI = imagesize(0, 0, WImgUI - 1, HImgUI - 1);
	
	for(int i = 0; i < 4; i++) {
		imgUI[i] = malloc(sizeImgUI);
		imgUI[i+4] = malloc(sizeImgUI);
	}
	
	readimagefile(".\\res\\image\\UIcor.bmp", 0, 0, 172 - 1, HImgUI - 1);
	for (int i = 0; i < 2; i++) {
		getimage(WImgUI*i, 0, WImgUI*i + WImgUI - 1, HImgUI - 1, imgUI[i]);
		getimage(WImgUI*2 + 40*i, 0, WImgUI*2 + 40*(i+1) - 1, HImgUI - 1, imgUI[i+2]);
	}
	
	readimagefile(".\\res\\image\\UImask.bmp", 0, 0, 172 - 1, HImgUI - 1);
	for (int i = 4; i < 6; i++) {
		getimage(WImgUI*(i-4), 0, WImgUI*(i-4) + WImgUI - 1, HImgUI - 1, imgUI[i]);
		getimage(WImgUI*2 + 40*(i-4), 0, WImgUI*2 + 40*(i-3) - 1, HImgUI - 1, imgUI[i+2]);
	}
	
	
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
	
	player.width = 60;
	player.height = SPRITE_RES;
	
	enemy.width = 60;
	enemy.height = SPRITE_RES;
		
	pos0.x = 45*SCALE;
	pos0.y = floor1.y - enemy.height;
	
	pos1.x = 54*SCALE;
	pos1.y = floor2.y - enemy.height;
	
	pos2.x = (WIDTH - 54)*SCALE - enemy.width;
	pos2.y = floor3.y - enemy.height;
	
	pos3.x = (WIDTH - 45)*SCALE - enemy.width;
	pos3.y = floor1.y - enemy.height;
	
	positions[0] = pos0;
	positions[1] = pos1;
	positions[2] = pos2;
	positions[3] = pos3;
	
	int initPos = 2;
	
	newGame(floors, nFloor, positions, initPos);
	
	changeGameState(1);
	
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
		
		if (restart == true) {
			newGame(floors, nFloor, positions, initPos);
			changeGameState(1);
			restart = false;
		}
		
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
			}	

		} else if(gameStateGb == 3) { //game over
		
			if(GetKeyState(ENTER)&0x80) { //restart
				restart = true;
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
	free(imgCredits);
	for (int i = 0; i < MAX_SPRITES; i++){
		free(sprites[i]);
		free(spriteMasks[i]);
	}
	for (int i = 0; i < 4; i++) {
		free(imgUI[i]);
		free(imgUI[i+4]);
	} 
	
	mciSendString("close menu", NULL, 0, 0);
	mciSendString("close game", NULL, 0, 0);
	mciSendString("close gameover", NULL, 0, 0);
	mciSendString("close credits", NULL, 0, 0);
	
}

void update(Wall *walls, int nWall, Floor *floors, int nFloor, Position *positions) {
	
	if(gameStateGb == 1) { //jogo
		updatePlayer(walls, nWall, floors, nFloor);
		freeFall(floors, nFloor);
		updateEnemy(positions);
		updatePellets();
	}
	
}

void render(Wall *walls, int nWall, Floor *floors, int nFloor) {
	
	if (gameStateGb == 1) {
		
		putimage(0, 0, imgScene, COPY_PUT);
		
		ui();
			
	//	setfillstyle(1, COLOR(255, 255, 0));
	//	for (int i = 0; i < nWall; i++) bar(walls[i].x, walls[i].y, walls[i].x + walls[i].width, walls[i].y + walls[i].height);
	//	for (int i = 0; i < nFloor; i++) bar(floors[i].x, floors[i].y, floors[i].x + floors[i].width, floors[i].y + floors[i].height);
		
		if (!enemy.damaged) setfillstyle(1, COLOR(255, 0, 0)); else setfillstyle(1, COLOR(0, 0, 255));
		bar(enemy.x, enemy.y, enemy.x + enemy.width, enemy.y + enemy.height);
		
		renderEnemy();
		renderPlayer();
		
		//if (!player.damaged) setfillstyle(1, COLOR(0, 255, 0)); else setfillstyle(1, COLOR(255, 255, 255));
		//bar(player.x, player.y, player.x + player.width, player.y + player.height);
		
		setfillstyle(1, COLOR(255, 255, 0));
		if (nPelletsGb > 0) {
			for (int i = 0; i < nPelletsGb; i++) {
				bar(pelletsGb[i].x, pelletsGb[i].y, pelletsGb[i].x + pelletsGb[i].width, pelletsGb[i].y + pelletsGb[i].height);
			}
		}
	} else if (gameStateGb == 2) { //creditos
		setfillstyle(1, COLOR(0, 0, 0));
		bar(0, 0, WIDTH*SCALE, HEIGHT*SCALE);
		putimage(240, 0, imgCredits, COPY_PUT);
		
	} else if (gameStateGb == 3) { //game over
		setfillstyle(1, COLOR(0, 0, 0));
		bar(0, 0, WIDTH*SCALE, HEIGHT*SCALE);
		putimage(240, 0, imgGameover, COPY_PUT);
	}

}
void updatePlayer(Wall *walls, int nWall, Floor *floors, int nFloor) {
	
	playerCollision(walls, nWall, floors, nFloor);
	if (!player.dead) {
		playerMove();
		playerJump(floors, nFloor);
		playerAttack();
		playerCheckLife();
	}
	
	
}

void renderPlayer() {
	
	if (player.dead) { //verifica se player esta morto
		if (player.lookRight) { //se player olha para direita
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, spriteMasks[34 + player.deathIndex], AND_PUT);
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, sprites[34 + player.deathIndex], OR_PUT);
		} else { //player olha para a esquerda
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, spriteMasks[90 + player.deathIndex], AND_PUT);
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, sprites[90 + player.deathIndex], OR_PUT);
		}
		player.deathIndex++;
		if (player.deathIndex >= MAX_PLAYER_DEATH_INDEX) {
			delay(20);
			changeGameState(3);
		}	
	} else if (player.damaged) {
		if (player.lookRight) { //se player olha para direita
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, spriteMasks[50 + player.damagedIndex], AND_PUT);
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, sprites[50 + player.damagedIndex], OR_PUT);
		} else { //player olha para a esquerda
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, spriteMasks[107 + player.damagedIndex], AND_PUT);
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, sprites[107 + player.damagedIndex], OR_PUT);
		}
		player.damagedIndex++;
		if (player.damagedIndex >= MAX_PLAYER_DAMAGED_INDEX) {
			player.damagedIndex = 0;
			player.damaged = false;
		}
	} else if (player.attack) { //verifica se ataca
		if (player.lookRight) { //se player olha para direita
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, spriteMasks[21 + player.attackIndex], AND_PUT);
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, sprites[21 + player.attackIndex], OR_PUT);
		} else { //player olha para a esquerda
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, spriteMasks[77 + player.attackIndex], AND_PUT);
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, sprites[77 + player.attackIndex], OR_PUT);
		}
		player.attackIndex++;
			if(player.attackIndex >= 8) {
				player.attackDamage = true;
			}
		if(player.attackIndex >= MAX_PLAYER_ATTACK_INDEX) {
			player.attackIndex = 0;
			player.attack = false;
			player.attackDamage = false;
		}
	} else if(player.falling) { // player no ar
		if (player.lookRight) { //se player olha para direita
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, spriteMasks[48], AND_PUT);
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, sprites[48], OR_PUT);
		} else { //player olha para a esquerda
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, spriteMasks[104], AND_PUT);
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, sprites[104], OR_PUT);
		}
	} else if(player.move) { // se player esta se movendo
		if(player.moveRight) { //se player se move para a direita
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, spriteMasks[0 + player.moveIndex], AND_PUT);
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, sprites[0 + player.moveIndex], OR_PUT);

		} else if(player.moveLeft) { //player se move para a esquerda
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, spriteMasks[56 + player.moveIndex], AND_PUT);
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, sprites[56 + player.moveIndex], OR_PUT);
			
		}
		player.moveIndex++;
		if(player.moveIndex >= MAX_PLAYER_MOVE_INDEX) {
			player.moveIndex = 0;
		}
		
	} else { //player esta parado
		if(player.lookRight) { //se player olha para direita
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, spriteMasks[13 + player.idleIndex], AND_PUT);
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, sprites[13 + player.idleIndex], OR_PUT);	
		} else { //player olha para a esquerda
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, spriteMasks[69 + player.idleIndex], AND_PUT);
			putimage(player.x - (SPRITE_RES-player.width)/2, player.y, sprites[69 + player.idleIndex], OR_PUT);
		}
		player.idleIndex++;
		if(player.idleIndex >= MAX_PLAYER_IDLE_INDEX) {
			player.idleIndex = 0;
		}
	}
	resetPlayerMove();
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
		} else if (player.moveRight) {
			player.x += player.speed;
		}
	}
}

void resetPlayerMove() {
	player.moveRight = false;
	player.move = false;
	player.moveLeft = false;
}

void playerJump(Floor *floors, int nFloor) { //pulo do jogador
	
	if (player.jump) {
			for (int i = 0; i <= nFloor; i++) {
			if (!(player.x + player.width <= floors[i].x) && !(player.x >= floors[i].x + floors[i].width)) {
				if ((player.y+player.height >= floors[i].y) && (player.y+player.height <= floors[i].y + floors[i].height) && player.vspd == 0) {
					player.vspd = -(player.jumpHeight);
					player.falling = true;
				}
			}
		}
	}
	player.jump = false;
}

void playerAttack() {
	if (player.attackDamage) {
		double distance;
		if (player.lookRight) {
			distance = sqrt(pow(player.x + player.width - enemy.x, 2 ) + pow(player.y - enemy.y, 2));
		} else {
			distance = sqrt(pow(player.x - enemy.x - enemy.width, 2) + pow(player.y - enemy.y, 2));
		}
		//printf("attack distance: %f ", distance);
		if (distance <= 55 && !enemy.damaged) { //ataque acertou
			sndPlaySound(".\\res\\audio\\hitEnemy.wav", SND_ASYNC);
			enemy.life -= player.power;
			enemy.damaged = true;
			enemy.timer = 0;
			printf("hit! enemy: %d ", enemy.life);
		}
	}
}

void cancelPlayerAttack() {
	player.attack = false;
	player.attackDamage = false;
	player.attackIndex = 0;
}

void playerCheckLife() {
	
	if(player.life <= 0) { //player derrotado. game over
		player.dead = true;
		return;
	}
	
	if(player.invincible) { //player machucado. frames de invencibilidade
		player.timerDamaged++;
		if(player.timerDamaged >= 30) {
			player.timerDamaged = 0;
			player.invincible = false;
		}
	}
}

void freeFall(Floor *floors, int nFloor) { //queda livre
	
	player.vspd += GRAVITY;
	if (player.vspd > 0) {
		player.falling = true;
		for (int i = 0; i <= nFloor - 1; i++) { //verifica colisao com o chao
			if (!(player.x + player.width <= floors[i].x) && !(player.x >= floors[i].x + floors[i].width)) {
				if (player.y+player.height+player.vspd >= floors[i].y && player.y + player.height <= floors[i].y) {
					player.y = floors[i].y - player.height;
					player.vspd = 0;
					player.falling = false;
					return;
				}
			}
		}
	}
	player.y += (int)player.vspd;
}

void updateEnemy(Position *positions) { //o inimigo utiliza o timer para fazer suas acoes. 
	
	enemyCheckLife();
	if (!player.dead){
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
	
}

void renderEnemy() {
	if (enemy.dead) {
		if(enemy.lookRight) {
			putimage(enemy.x - (SPRITE_RES-enemy.width)/2, enemy.y, spriteMasks[142 + enemy.deathIndex], AND_PUT);
			putimage(enemy.x - (SPRITE_RES-enemy.width)/2, enemy.y, sprites[142 + enemy.deathIndex], OR_PUT);
		} else {
			putimage(enemy.x - (SPRITE_RES-enemy.width)/2, enemy.y, spriteMasks[181 + enemy.deathIndex], AND_PUT);
			putimage(enemy.x - (SPRITE_RES-enemy.width)/2, enemy.y, sprites[181 + enemy.deathIndex], OR_PUT);
		}
		enemy.deathIndex++;
		if (enemy.deathIndex >= MAX_ENEMY_DEATH_INDEX) {
			delay(20);
			changeGameState(2);
		}
	} else if(enemy.damaged) {
		if(enemy.lookRight) {
			
		} else {
			
		}
	} else {
		if(enemy.lookRight) {
			//putimage(enemy.x - (SPRITE_RES-enemy.width)/2, enemy.y, spriteMasks[135 + enemy.idleIndex], AND_PUT);
			//putimage(enemy.x - (SPRITE_RES-enemy.width)/2, enemy.y, sprites[135 + enemy.idleIndex], OR_PUT);
		} else {
			//putimage(enemy.x - (SPRITE_RES-enemy.width)/2, enemy.y, spriteMasks[184 + enemy.idleIndex], AND_PUT);
			//putimage(enemy.x - (SPRITE_RES-enemy.width)/2, enemy.y, sprites[184 + enemy.idleIndex], OR_PUT);
		}
		enemy.idleIndex++;
		if (enemy.deathIndex >= MAX_ENEMY_DEATH_INDEX) {
			enemy.idleIndex;
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
	
	if (nextPos == 0 || nextPos == 1) enemy.lookRight = true; else if (nextPos == 2 || nextPos == 3) enemy.lookRight = false;
}

void enemyAttack(Position *positions) {
	
	int enemyPos;
	Pellet pellet1, pellet2;

	
	//verifica a posicao atual do inimigo
	if (enemy.x == positions[0].x && enemy.y == positions[0].y) { //posicao 0

		pellet1 = createPellet(1, 0);
		pellet2 = createPellet(2, 0);
		addPellet(pellet1);
		addPellet(pellet2);
		
	} else if (enemy.x == positions[1].x && enemy.y == positions[1].y) { //posicao 1

		pellet1 = createPellet(1, 1);
		pellet2 = createPellet(2, 1);
		addPellet(pellet1);
		addPellet(pellet2);
		
	} else if (enemy.x == positions[2].x && enemy.y == positions[2].y) { //posicao 2
		
		pellet1 = createPellet(1, 2);
		pellet2 = createPellet(2, 2);
		addPellet(pellet1);
		addPellet(pellet2);
		
	} else if (enemy.x == positions[3].x && enemy.y == positions[3].y) { //posicao 3

		pellet1 = createPellet(1, 3);
		pellet2 = createPellet(2, 3);
		addPellet(pellet1);
		addPellet(pellet2);
		
	} else {
		return;
	}
	
	
}

void enemyCheckLife() {
	if (enemy.life <= 0) { //inimigo derrotado. vitoria.
		enemy.dead = true;
		sndPlaySound(".\\res\\audio\\deathEnemy.wav", SND_ASYNC);
	}
}

void updatePellets() {
	if (nPelletsGb > 0) {
		for (int i = 0; i < nPelletsGb; i++) {
			pelletsGb[i].x += pelletsGb[i].vx;
			pelletsGb[i].y += pelletsGb[i].vy;
			
			if (pelletsGb[i].x <= (0*SCALE) || pelletsGb[i].x >= (WIDTH*SCALE) || pelletsGb[i].y <= (0*SCALE) || pelletsGb[i].y >= (HEIGHT*SCALE)) {
				removePellet(i);
				if(nPelletsGb == 0) {
					return;
				}
			}	
			
			if (pelletsGb[i].x >= player.x && pelletsGb[i].x - 1 <= player.x + player.width && pelletsGb[i].y >= player.y && pelletsGb[i].y - 1  <= player.y + player.height) { //acerta player
				if (!player.invincible) {
					removePellet(i);
					cancelPlayerAttack();
					player.life -= enemy.power;
					if (player.life > 0) sndPlaySound(".\\res\\audio\\hitPlayer.wav", SND_ASYNC); else sndPlaySound(".\\res\\audio\\deathPlayer.wav", SND_ASYNC);
					player.damaged = true;
					player.invincible = true;
					printf("Damaged player: %d ", player.life);
				}
			}	
			
					
		}
	}
}

Pellet createPellet(int number, int position) {
	
	Pellet pellet;
	int speedX = 8;
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

void ui() {
	
	for (int i = 0; i < 8; i++) {
		putimage(40 + i*56, 30, imgUI[5], AND_PUT);
		putimage(40 + i*56, 30, imgUI[1], OR_PUT);
	
		putimage(WIDTH*SCALE - 40 - 40 - i*56, 30, imgUI[7], AND_PUT);
		putimage(WIDTH*SCALE - 40 - 40 - i*56, 30, imgUI[3], OR_PUT);
	}
	
	for (int i = 0; i<player.life; i++) {
		putimage(40 + i*56, 30, imgUI[4], AND_PUT);
		putimage(40 + i*56, 30, imgUI[0], OR_PUT);
	}
	
	for (int i = 0; i<enemy.life; i++) {
		putimage(WIDTH*SCALE - 40 - 40 - i*56, 30, imgUI[6], AND_PUT);
		putimage(WIDTH*SCALE - 40 - 40 - i*56, 30, imgUI[2], OR_PUT);
	}
	
}

void newGame(Floor *floors, int Floor, Position *positions, int initPos) {
	
	player.x = (WIDTH/2 - 180 )*SCALE;
	player.y = floors[1].y - player.height;
	player.speed = 8;
	player.life = 8;
	player.power = 1;
	player.attack = false;
	player.attackDamage = false;
	player.jump = false;
	player.move = false;
	player.moveLeft = false;
	player.moveRight = false;
	player.lookRight = true;
	player.falling = false;
	player.damaged = false;
	player.invincible = false;
	player.dead = false;
	player.jumpHeight = 26;
	player.vspd = 0;
	player.idleIndex = 0;
	player.moveIndex = 0;
	player.attackIndex = 0;
	player.damagedIndex = 0;
	player.deathIndex = 0;
	
	enemy.x = positions[initPos].x;
	enemy.y = positions[initPos].y;
	enemy.life = 8;
	enemy.power = 1;
	enemy.timer = 0;
	enemy.attack = false;
	enemy.move = false;
	enemy.teleport = false;
	enemy.lookRight = false;
	enemy.damaged = false;
	enemy.invincible = false;
	enemy.dead = false;
	enemy.idleIndex = 0;
	enemy.teleportIndex = 0;
	enemy.attackIndex = 0;
	enemy.damagedIndex = 0;
	enemy.deathIndex = 0;
	
	clearPellets();
}

void changeGameState(int newState) {
	
	if(gameStateGb == 1) {
		mciSendString("stop game", NULL, 0, 0);
	} else if(gameStateGb == 2) {
		mciSendString("stop credits", NULL, 0, 0);
	} else if(gameStateGb == 3) {
		mciSendString("stop gameover", NULL, 0, 0);
	} else if(gameStateGb == 0) {
		mciSendString("stop menu", NULL, 0, 0);
	}
	
	if(newState == 1) {
		mciSendString("seek game to start", NULL, 0, 0);
		mciSendString("play game repeat", NULL, 0, 0);
	} else if (newState == 2) {
		mciSendString("seek credits to start", NULL, 0, 0);
		mciSendString("play credits repeat", NULL, 0, 0);
	} else if (newState == 3) {
		mciSendString("seek gameover to start", NULL, 0, 0);
		mciSendString("play gameover repeat", NULL, 0, 0);
	} else if (newState == 0) {
		mciSendString("seek menu to start", NULL, 0, 0);
		mciSendString("play menu repeat", NULL, 0, 0);
	}
	
	gameStateGb = newState;
}
