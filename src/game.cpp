#include "game.h"
#include "sound.h"
#include "DxLib.h"
#include <stdio.h>

int charactor;
int back[3];
int bulletIMG;
int enemyIMG;
int dragonIMG;
int heartIMG;
int titlelogo;

void InitGame(Game& game) {
    game.y = 0;
    game.L = 0;
    game.score = 0;
    game.life = 3;

    game.x1 = 640;
    game.x2 = 1280;
    game.i = 0;

    SetFontSize(20);

    titlelogo = LoadGraph("assets/fig/title.webp");
	charactor = LoadGraph("assets/fig/charactor.png");
    back[0] = LoadGraph("assets/fig/back01.jpg");
    back[1] = LoadGraph("assets/fig/back02.jpg");
    back[2] = LoadGraph("assets/fig/back03.jpg");

    bulletIMG = LoadGraph("assets/fig/bullet.png");
    for (int i = 0; i < MAX_BULLETS; i++) {
        game.bullets[i] = { 60, false }; // x座標, y座標
    }
    game.bulletnum = 5;
    game.shot = 0;

    enemyIMG = LoadGraph("assets/fig/fire.png");
    dragonIMG = LoadGraph("assets/fig/dragon.png");
    heartIMG = LoadGraph("assets/fig/heart.png");

    int baseX = 640; // 基準となるX座標
    int minGap = 150; // 最小間隔
    int maxGap = 300; // 最大間隔

    for (int i = 0; i < MAX_ENEMIES; i++) {
        game.enemies[i].x = baseX + GetRand(maxGap - minGap) + minGap; // 一定間隔を確保しつつランダムに配置
        baseX = game.enemies[i].x; // 次の敵の基準位置を更新
        game.enemies[i].y = 120 + i * 100; // 上、中、下の順に配置
        game.enemies[i].spawn = GetRand(300) + 600; // 初回のスポーン間隔をさらに広げる
    }
}

void LoadHighScore(Game& game) {
    FILE* fp;
    fopen_s(&fp, "assets/save.dat", "rb");
    if (fp) {
        fread(&game.L_max, sizeof(int), 1, fp);
        fread(&game.high_score, sizeof(int), 1, fp);
        fclose(fp);
    }
}

void SaveHighScore(Game& game) {
    if (game.L > game.L_max) {
        game.L_max = game.L;
    }
    if (game.score > game.high_score) {
        game.high_score = game.score;
    }

    FILE* fp;
    fopen_s(&fp, "assets/save.dat", "wb");
    if (fp) {
        fwrite(&game.L_max, sizeof(int), 1, fp);
        fwrite(&game.high_score, sizeof(int), 1, fp);
        fclose(fp);
    }
}

bool CheckCollisionWithBullet(Bullet bullet, Enemy enemy) {
	int hitboxExpansion = 20; // 当たり判定を広げる

    return bullet.x >= enemy.x - 20 && bullet.x <= enemy.x + 20 && 
        bullet.y >= enemy.y - (29 + hitboxExpansion) && // y方向の下限
		bullet.y <= enemy.y + (29 + hitboxExpansion); // y方向の上限
}

bool CheckCollisionWithPlayer(Game& game, Enemy enemy) {
    int hitboxWidth, hitboxHeight = 29;  // 縦の当たり判定は火の玉と同じ

    if (enemy.isDragon) {
        // ドラゴンの画像サイズを取得
        int originalWidth, originalHeight;
        GetGraphSize(dragonIMG, &originalWidth, &originalHeight);

        // スケールを適用（0.3 倍）
        hitboxWidth = (int)(originalWidth * 0.3f);
    }
    else {
        // 火の玉の当たり判定は変更なし
        hitboxWidth = 29;
    }

    // 当たり判定（中央基準）
    return (enemy.x < 147 + hitboxWidth / 2 && enemy.x > 22 - hitboxWidth / 2 &&
        300 - game.y - (26 + hitboxHeight / 2) < enemy.y + hitboxHeight / 2 &&
        300 - game.y + (26 + hitboxHeight / 2) > enemy.y - hitboxHeight / 2);
}



void UpdateBullets(Game& game) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (game.bullets[i].active) {
            game.bullets[i].x += 5; // ゆっくり移動

            // 弾を最前面に描画 & スケールを調整
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            DrawRotaGraph(game.bullets[i].x, game.bullets[i].y, 1.0, 0.0, bulletIMG, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (CheckCollisionWithBullet(game.bullets[i], game.enemies[j])) {
                    game.bullets[i].active = false; // 弾を消す
                    game.enemies[j].x = 640 + GetRand(200); // 敵をリスポーン
                    game.score += 100; // ポイント加算
                }
            }

            if (game.bullets[i].x > 640) {
                game.bullets[i].active = false;
            }
        }
    }
}

void FireBullet(Game& game) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!game.bullets[i].active) {
            game.bullets[i].x = 60; // キャラクターの手の位置
            game.bullets[i].y = 300 - game.y;
            game.bullets[i].active = true;
            break;
        }
    }
    game.bulletnum--;
    game.shot = game.L; // 最後に撃った位置を記録
}

void DebugDrawHitbox(Enemy enemy) {
    int hitboxWidth, hitboxHeight = 29;  // 縦の当たり判定は火の玉と同じ

    if (enemy.isDragon) {
        int originalWidth, originalHeight;
        GetGraphSize(dragonIMG, &originalWidth, &originalHeight);
        hitboxWidth = (int)(originalWidth * 0.2f); // ドラゴンはスケール 0.2
    }
    else {
        hitboxWidth = 29;  // 火の玉の当たり判定は変更なし
    }

    // 赤い枠で当たり判定を可視化
    DrawBox(enemy.x - hitboxWidth / 2, enemy.y - hitboxHeight / 2,
        enemy.x + hitboxWidth / 2, enemy.y + hitboxHeight / 2,
        GetColor(255, 0, 0), FALSE);
}


void UpdateEnemies(Game& game) {
	// DebugDrawHitbox(game.enemies[0]);
    // DebugDrawHitbox(game.enemies[1]);
    // DebugDrawHitbox(game.enemies[2]);

    // 5000m ごとに速度を増加 (最大6px/frame)
    int enemySpeed = min(3 + (game.L / 5000), 7);

    int minGap = 150; // 敵同士の最小間隔
    int maxGap = 300; // 敵同士の最大間隔

    for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy& enemy = game.enemies[i];

        if (enemy.spawn > 0) {
            enemy.spawn--;
            continue;
        }

        enemy.x -= enemySpeed;

        if (enemy.x < -60) {
            int prevX = (i == 0) ? 640 : game.enemies[i - 1].x;
            enemy.x = prevX + GetRand(maxGap - minGap) + minGap; // 直前の敵との間隔を確保
            enemy.y = 120 + i * 100;  // Y位置を一定間隔で維持
            enemy.spawn = GetRand(300) + 600; // 次のスポーンを遅くする

            int dragonChance = min(10 + (game.L / 5000) * 5, 50); // 最大50%の確率
            if (GetRand(100) < dragonChance) {
                enemy.isDragon = true;
            }
            else {
                enemy.isDragon = false;
            }
        }

        if (enemy.isDragon) {
            DrawRotaGraph(enemy.x, enemy.y, 0.2, 0.0, dragonIMG, TRUE);
        }
        else {
            DrawRotaGraph(enemy.x, enemy.y, 0.12, 0.0, enemyIMG, TRUE);
        }

        for (int j = 0; j < MAX_BULLETS; j++) {
            if (game.bullets[j].active && CheckCollisionWithBullet(game.bullets[j], enemy)) {
                game.bullets[j].active = false;

                // ポイント加算
                if (enemy.isDragon) {
                    game.score += 500;
                }
                else {
                    game.score += 100;
                }

                int prevX = (i == 0) ? 640 : game.enemies[i - 1].x;
                enemy.x = prevX + GetRand(maxGap - minGap) + minGap; // 敵をリスポーン
                enemy.y = 120 + i * 100;
                enemy.spawn = GetRand(300) + 600;
            }
        }

        // キャラクターと敵がぶつかったとき
        if (CheckCollisionWithPlayer(game, enemy)) {
			PlaySE(hitSE);

            if (enemy.isDragon) {
                game.state = GAME_OVER;
            }
            else {
                if (game.life == 1) {
                    game.life = 0;
                    game.state = GAME_OVER;
                }
                else if (game.life > 1) {
                    game.life--;
                    game.hitTimer = 60;
                }
            }
            // 敵をリスポーン
            enemy.x = 640 + GetRand(200);
            enemy.spawn = GetRand(300) + 600;
        }
    }
}
