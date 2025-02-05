#include "game.h"
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

    titlelogo = LoadGraph("fig/title.webp");
	charactor = LoadGraph("fig/charactor.png");
    back[0] = LoadGraph("fig/back01.jpg");
    back[1] = LoadGraph("fig/back02.jpg");
    back[2] = LoadGraph("fig/back03.jpg");

    bulletIMG = LoadGraph("fig/bullet.png");
    for (int i = 0; i < MAX_BULLETS; i++) {
        game.bullets[i] = { 60, false }; // x���W, y���W
    }
    game.bulletnum = 3;
    game.shot = 0;

    enemyIMG = LoadGraph("fig/fire.png");
    dragonIMG = LoadGraph("fig/dragon.png");
    heartIMG = LoadGraph("fig/heart.png");

    int baseX = 640; // ��ƂȂ�X���W
    int minGap = 150; // �ŏ��Ԋu
    int maxGap = 300; // �ő�Ԋu

    for (int i = 0; i < MAX_ENEMIES; i++) {
        game.enemies[i].x = baseX + GetRand(maxGap - minGap) + minGap; // ���Ԋu���m�ۂ������_���ɔz�u
        baseX = game.enemies[i].x; // ���̓G�̊�ʒu���X�V
        game.enemies[i].y = 120 + i * 100; // ��A���A���̏��ɔz�u
        game.enemies[i].spawn = GetRand(300) + 600; // ����̃X�|�[���Ԋu������ɍL����
    }
}

void LoadHighScore(Game& game) {
    FILE* fp;
    fopen_s(&fp, "save.dat", "rb");
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
    fopen_s(&fp, "save.dat", "wb");
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
            game.bullets[i].x += 5; // �������ړ�

            // �e���őO�ʂɕ`�� & �X�P�[���𒲐�
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            DrawRotaGraph(game.bullets[i].x, game.bullets[i].y, 1.0, 0.0, bulletIMG, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (CheckCollision(game.bullets[i], game.enemies[j])) {
                    game.bullets[i].active = false; // �e������
                    game.enemies[j].x = 640 + GetRand(200); // �G�����X�|�[��
                    game.score += 100; // �|�C���g���Z
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
            game.bullets[i].x = 60; // �L�����N�^�[�̎�̈ʒu
            game.bullets[i].y = 300 - game.y;
            game.bullets[i].active = true;
            break;
        }
    }
    game.bulletnum--;
    game.shot = game.L; // �Ō�Ɍ������ʒu���L�^
}


void UpdateEnemies(Game& game) {
    // 5000m ���Ƃɑ��x�𑝉� (�ő�6px/frame)
    int enemySpeed = min(3 + (game.L / 5000), 7);

    int minGap = 150; // �G���m�̍ŏ��Ԋu
    int maxGap = 300; // �G���m�̍ő�Ԋu

    for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy& enemy = game.enemies[i];

        if (enemy.spawn > 0) {
            enemy.spawn--;
            continue;
        }

        enemy.x -= enemySpeed;

        if (enemy.x < -60) {
            int prevX = (i == 0) ? 640 : game.enemies[i - 1].x;
            enemy.x = prevX + GetRand(maxGap - minGap) + minGap; // ���O�̓G�Ƃ̊Ԋu���m��
            enemy.y = 120 + i * 100;  // Y�ʒu�����Ԋu�ňێ�
            enemy.spawn = GetRand(300) + 600; // ���̃X�|�[����x������

            if (game.L >= 10000) {
                int dragonChance = min(10 + (game.L / 5000) * 5, 50); // �ő�50%�̊m��
                if (GetRand(100) < dragonChance) {
                    enemy.isDragon = true;
                }
                else {
                    enemy.isDragon = false;
                }
            }
            else {
                enemy.isDragon = false;
            }
        }

        if (enemy.isDragon) {
            DrawRotaGraph(enemy.x, enemy.y, 0.3, 0.0, dragonIMG, TRUE);
        }
        else {
            DrawRotaGraph(enemy.x, enemy.y, 0.12, 0.0, enemyIMG, TRUE);
        }

        for (int j = 0; j < MAX_BULLETS; j++) {
            if (game.bullets[j].active && CheckCollision(game.bullets[j], enemy)) {
                game.bullets[j].active = false;

                // �|�C���g���Z
                if (enemy.isDragon) {
                    game.score += 500;
                }
                else {
                    game.score += 100;
                }

                int prevX = (i == 0) ? 640 : game.enemies[i - 1].x;
                enemy.x = prevX + GetRand(maxGap - minGap) + minGap; // �G�����X�|�[��
                enemy.y = 120 + i * 100;
                enemy.spawn = GetRand(300) + 600;
            }
        }

        // �L�����N�^�[�ƓG���Ԃ������Ƃ�
        if (enemy.x < 147 && enemy.x > 22 &&
            300 - game.y - 26 < enemy.y + 29 && 300 - game.y + 26 > enemy.y - 29) {
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
            // �G�����X�|�[��
            enemy.x = 640 + GetRand(200);
            enemy.spawn = GetRand(300) + 600;
        }
    }
}
