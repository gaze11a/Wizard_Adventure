#pragma warning(disable : 4996) // fopenエラー解除

#include "DxLib.h"
#include <stdio.h>
#include <math.h>

#define MAX_BULLETS 5
#define MAX_ENEMIES 3

// 未実装機能
// 
// 即死ドラゴンの追加
// 宝箱の追加
// 効果音の追加


struct Bullet {
    int x;
    int y;
    bool active;
};

struct Enemy {
    int x, y, spawn;
};

enum GameState {
    MENU = 0,
    PAUSED = 1,
    PLAYING = 2,
    GAME_OVER = 3
};

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
};

Game game;
int Key[256];

int gpUpdateKey() {
    char tmpKey[256];
    GetHitKeyStateAll(tmpKey);
    for (int i = 0; i < 256; i++) {
        if (tmpKey[i] != 0)
            Key[i]++;
        else
            Key[i] = 0;
    }
    return 0;
}

// 画像
int bulletIMG;
int enemyIMG;
int heartIMG;
int titlelogo;

void InitGame(Game& game) {
    game.y = 0;
    game.L = 0;
    game.score = 0;
    game.life = 3;

    SetFontSize(20);

    titlelogo = LoadGraph("fig/title.webp");

    bulletIMG = LoadGraph("fig/bullet.png");
    for (int i = 0; i < MAX_BULLETS; i++) {
        game.bullets[i] = { 60, false }; // x座標, y座標
    }
    game.bulletnum = 3;
    game.shot = 0;

    enemyIMG = LoadGraph("fig/fire.png");
	heartIMG = LoadGraph("fig/heart.png");

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
    FILE* fp = fopen("save.dat", "rb");
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

    FILE* fp = fopen("save.dat", "wb");
    if (fp) {
        fwrite(&game.L_max, sizeof(int), 1, fp);
        fwrite(&game.high_score, sizeof(int), 1, fp);
        fclose(fp);
    }
}

bool CheckCollision(Bullet bullet, Enemy enemy) {
    return bullet.x >= enemy.x &&
        300 - game.y > enemy.y - 29 &&
        300 - game.y < enemy.y + 29;
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
                if (CheckCollision(game.bullets[i], game.enemies[j])) {
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


void UpdateEnemies(Game& game) {
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
        }

        DrawRotaGraph(enemy.x, enemy.y, 0.12, 0.0, enemyIMG, TRUE);

        for (int j = 0; j < MAX_BULLETS; j++) {
            if (game.bullets[j].active && CheckCollision(game.bullets[j], enemy)) {
                int prevX = (i == 0) ? 640 : game.enemies[i - 1].x;
                enemy.x = prevX + GetRand(maxGap - minGap) + minGap; // 敵をリスポーン
                enemy.y = 120 + i * 100;
                enemy.spawn = GetRand(300) + 600;
                game.bullets[j].active = false;
                game.score += 100; // ポイント加算
            }
        }

        // キャラクターと敵がぶつかったとき
        if (enemy.x < 147 && enemy.x > 22 &&
            300 - game.y - 26 < enemy.y + 29 && 300 - game.y + 26 > enemy.y - 29) {

            if (game.life == 1) {
                game.life = 0;
                game.state = GAME_OVER;
            }
            else if (game.life > 1) {
                game.life--;
                game.hitTimer = 60;
            }

            // 敵をリスポーン
            enemy.x = 640 + GetRand(200);
            enemy.spawn = GetRand(300) + 600;
        }
    }
}



int titleBGM, playingBGM, gameOverBGM, startSE, enemyHitSE, moveSE, shootSE;

// Menu画面でbgm1をリピート
// PLAYINGでもbgm2をリピート
// GAMEOVERになったらbgm3をリピート

void LoadSounds() {
    titleBGM = LoadSoundMem("bgm/title.wav");
    playingBGM = LoadSoundMem("bgm/playing.wav");
    gameOverBGM = LoadSoundMem("bgm/gameover.wav");

    startSE = LoadSoundMem("sound/start.wav");
    enemyHitSE = LoadSoundMem("sound/hit.wav");
    moveSE = LoadSoundMem("sound/move.wav");
    shootSE = LoadSoundMem("sound/shoot.wav");

    // BGMをループ再生
    PlaySoundMem(titleBGM, DX_PLAYTYPE_LOOP); 
    PlaySoundMem(playingBGM, DX_PLAYTYPE_LOOP);
    PlaySoundMem(gameOverBGM, DX_PLAYTYPE_LOOP);
}

void PlaySE(int sound) {
    PlaySoundMem(sound, DX_PLAYTYPE_BACK);
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    ChangeWindowMode(TRUE);
    DxLib_Init();
    SetDrawScreen(DX_SCREEN_BACK);

    int charactor = LoadGraph("fig/charactor.png");
    int back[3] = { LoadGraph("fig/back01.jpg"), LoadGraph("fig/back02.jpg"), LoadGraph("fig/back03.jpg") };

    game.state = MENU;
    InitGame(game);
    LoadHighScore(game);

    int x1 = 640, x2 = 1280, i = 0;

    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && gpUpdateKey() == 0) {
        if (Key[KEY_INPUT_ESCAPE] == 1) {
            if (game.state == PLAYING) game.state = PAUSED;
            else if (game.state == PAUSED) game.state = PLAYING;
        }

        if (game.state == PAUSED) {
            // 背景を描画してゲーム画面をうっすら見せる
            DrawRotaGraph(x1, 240, 0.625, 0.0, back[i % 3], TRUE);
            DrawRotaGraph(x2, 240, 0.625, 0.0, back[i % 3], TRUE);
            DrawRotaGraph(60, 300 - game.y, 0.1, 0.0, charactor, TRUE);

            // 半透明レイヤーを重ねる
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
            DrawBox(0, 0, 640, 480, GetColor(100, 100, 100), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

            // ポーズメッセージ
            DrawFormatString(230, 230, GetColor(255, 255, 255), "Spaceを押して再開");

            // ゲーム開始
            if (Key[KEY_INPUT_SPACE] == 1) {
                game.state = PLAYING;
            }
        }
        else if (game.state == MENU) {
            DrawRotaGraph(x1, 240, 0.625, 0.0, back[i % 3], TRUE);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
            DrawBox(0, 0, 640, 480, GetColor(100, 100, 100), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

            DrawExtendGraph(0, -35, 640, 480, titlelogo, TRUE); // タイトル

            // 最高スコアの表示をロゴの下に配置
            int text_x = 320 - GetDrawFormatStringWidth("最高スコア: %d (%dm)", game.high_score, game.L_max) / 2;
            DrawFormatString(text_x, 420, GetColor(75, 0, 130), "最高スコア: %d (%dm)", game.high_score, game.L_max);

            // SPACEキーのテキストもさらに下に配置
            text_x = 320 - GetDrawFormatStringWidth("SPACEを押してスタート") / 2;
            DrawFormatString(text_x, 450, GetColor(0, 0, 0), "SPACEを押してスタート");

            if (Key[KEY_INPUT_SPACE] == 1) {
                game.state = PLAYING;
                PlaySE(startSE);
            }
        }
        else if (game.state == PLAYING) {
            x1 -= 2;
            if (x1 <= 0) x2 -= 2;
            if (x2 == 640) {
                x1 = x2;
                x2 = 1280;
            }

            // 背景
            DrawRotaGraph(x1, 240, 0.625, 0.0, back[i % 3], TRUE);
            DrawRotaGraph(x2, 240, 0.625, 0.0, back[i % 3], TRUE);

            game.L += 2;
            //DrawFormatString(20, 10, GetColor(0, 255, 0), "%dm", game.L);
            if (game.L % 10000 == 0) i = (i + 1) % 3; // 10000mごとに背景切り替え

            if (game.L % 1000 == 0) game.score += 300; // 1000mごとにポイント加算
            if ((game.L - game.shot) == 10000 && game.bulletnum < 3) game.bulletnum++; // 弾を撃ってから10000mごとに弾追加
            

            DrawFormatString(125, 10, GetColor(195, 144, 22), "Score:%d", game.score);
            DrawFormatString(400, 10, GetColor(255, 0, 0), "Press D: attack %d/3", game.bulletnum);

            for (int i = 0; i < game.life; i++) {
                DrawRotaGraph(35 + i * 30, 20, 0.1, 0.0, heartIMG, TRUE);
            }

			// キャラクターの移動
            if (Key[KEY_INPUT_UP] >= 1 && game.y < 220) game.y += 6;
            if (Key[KEY_INPUT_DOWN] >= 1 && game.y > 0) game.y -= 6;
            
			// キャラクターの描画
            if (game.life > 0) {
                if (game.hitTimer > 0) {
                    game.hitTimer--;
                    if (game.hitTimer % 10 < 5) {
                        DrawRotaGraph(60, 300 - game.y, 0.1, 0.0, charactor, TRUE);
                    }
                }
                else {
                    DrawRotaGraph(60, 300 - game.y, 0.1, 0.0, charactor, TRUE);
                }
            }

            if (Key[KEY_INPUT_D] == 1 && game.bulletnum > 0) {
                FireBullet(game);
                PlaySE(shootSE);
            }
            UpdateBullets(game);
            UpdateEnemies(game);
        }
        else if (game.state == GAME_OVER) {
            // 背景を描画してゲーム画面をうっすら見せる
            DrawRotaGraph(x1, 240, 0.625, 0.0, back[i % 3], TRUE);
            DrawRotaGraph(x2, 240, 0.625, 0.0, back[i % 3], TRUE);

            // 半透明レイヤーを重ねる
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
            DrawBox(0, 0, 640, 480, GetColor(100, 100, 100), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

            // 最高記録の更新
            SaveHighScore(game);

            // GAME OVER メッセージ
            SetFontSize(48); // フォントサイズを大きくする

            int text_x = 320 - GetDrawFormatStringWidth("GAME OVER") / 2;
            DrawFormatString(text_x, 120, GetColor(255, 0, 0), "GAME OVER");
            SetFontSize(20); // 他のテキストは通常サイズ

            text_x = 320 - GetDrawFormatStringWidth("スコア: %d (%dm)", game.score, game.L) / 2;
            DrawFormatString(text_x, 230, GetColor(255, 255, 255), "スコア: %d (%dm)", game.score, game.L);

            text_x = 320 - GetDrawFormatStringWidth("最高スコア: %d (%dm)", game.high_score, game.L_max) / 2;
            DrawFormatString(text_x, 290, GetColor(255, 255, 0), "最高スコア: %d (%dm)", game.high_score, game.L_max);

            text_x = 320 - GetDrawFormatStringWidth("Spaceを押してリスタート") / 2;
            DrawFormatString(text_x, 330, GetColor(255, 255, 255), "Spaceを押してリスタート");

            if (Key[KEY_INPUT_SPACE] == 1) {
                InitGame(game);
                game.state = PLAYING;
            }
        }
    }
    DxLib_End();
    return 0;
}
