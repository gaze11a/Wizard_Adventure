#ifndef GAME_H
#define GAME_H

#include "DxLib.h"

#define MAX_BULLETS 3
#define MAX_ENEMIES 3

struct Bullet {
    int x;
    int y;
    bool active;
};

struct Enemy {
    int x, y, spawn, isDragon;
};


enum GameState {
    MENU = 0,
    PAUSED = 1,
    PLAYING = 2,
	GAME_OVER = 3,
	MENU_SETTINGS = 4,
	PAUSE_SETTINGS = 5,
	GAMEOVER_SETTINGS = 6
};

// ゲームのグローバルデータ
struct Game {
    int y;
    GameState state;
    int L;
    int L_max;
    int score;
    int high_score;
    int life;

    Bullet bullets[MAX_BULLETS];
    int bulletnum;
    int shot;

    Enemy enemies[MAX_ENEMIES];
    int hitTimer;

	int x1, x2, i;
};

// 画像
extern int charactor;
extern int back[3];
extern int bulletIMG;
extern int enemyIMG;
extern int dragonIMG;
extern int heartIMG;
extern int titlelogo;

extern Game game;

// ゲーム関連の関数
void InitGame(Game& game);
// void LoadHighScore(Game& game);
// void SaveHighScore(Game& game);

bool CheckCollisionWithBullet(Bullet bullet, Enemy enemy);
bool CheckCollisionWithPlayer(Game& game, Enemy enemy);

void UpdateBullets(Game& game);
void FireBullet(Game& game);

void DebugDrawHitbox(Enemy enemy);
void UpdateEnemies(Game& game);

#endif
