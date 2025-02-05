#pragma warning(disable : 4996) // fopen�G���[����

#include "DxLib.h"
#include <stdio.h>
#include <math.h>

#define MAX_BULLETS 5
#define MAX_ENEMIES 3

// �������@�\
// 
// �G�̃X�|�[�������X�ɑ��߂�A�h���S���̒ǉ�
// ���C�t�@�\�i�ŏ�3�A�΂ɓ���������1�}�C�i�X�A�h���S���͑����j
// ���ʉ��̒ǉ�
// �ݒ�i���ʉ����ʒ��߁A


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

    Bullet bullets[MAX_BULLETS];
    int bulletnum;
    int shot;

    Enemy enemies[MAX_ENEMIES];
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

int bulletSprite;
int enemySprite;
int titlelogo;

void InitGame(Game& game) {
    game.y = 0;
    game.L = 0;
    game.score = 0;
    SetFontSize(20);

    titlelogo = LoadGraph("fig/title2.webp");

    bulletSprite = LoadGraph("fig/bullet00.png");
    for (int i = 0; i < MAX_BULLETS; i++) {
        game.bullets[i] = { 60, false }; // x���W, y���W
    }
    game.bulletnum = 5;
    game.shot = 0;

    enemySprite = LoadGraph("fig/fire.png");

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
            game.bullets[i].x += 5; // �������ړ�

            // �e���őO�ʂɕ`�� & �X�P�[���𒲐�
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            DrawRotaGraph(game.bullets[i].x, game.bullets[i].y, 1.0, 0.0, bulletSprite, TRUE);
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
    int minGap = 150; // �G���m�̍ŏ��Ԋu
    int maxGap = 300; // �G���m�̍ő�Ԋu

    for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy& enemy = game.enemies[i];

        if (enemy.spawn > 0) {
            enemy.spawn--;
            continue;
        }

        enemy.x -= 2; // �G�̈ړ����x������ɒx������

        if (enemy.x < -60) {
            int prevX = (i == 0) ? 640 : game.enemies[i - 1].x;
            enemy.x = prevX + GetRand(maxGap - minGap) + minGap; // ���O�̓G�Ƃ̊Ԋu���m��
            enemy.y = 120 + i * 100;  // Y�ʒu�����Ԋu�ňێ�
            enemy.spawn = GetRand(300) + 600; // ���̃X�|�[��������ɒx������
        }

        DrawRotaGraph(enemy.x, enemy.y, 0.12, 0.0, enemySprite, TRUE);

        for (int j = 0; j < MAX_BULLETS; j++) {
            if (game.bullets[j].active && CheckCollision(game.bullets[j], enemy)) {
                int prevX = (i == 0) ? 640 : game.enemies[i - 1].x;
                enemy.x = prevX + GetRand(maxGap - minGap) + minGap; // ���O�̓G�Ƃ̊Ԋu���m��
                enemy.y = 120 + i * 100;
                enemy.spawn = GetRand(300) + 600; // �|���ꂽ��̍ďo��������ɒx�߂�
                game.bullets[j].active = false;
                game.score += 100; // �G��|���ƃ|�C���g���Z
            }
        }

        if (enemy.x < 147 && enemy.x > 22 &&
            300 - game.y - 26 < enemy.y + 29 && 300 - game.y + 26 > enemy.y - 29) {
            game.state = GAME_OVER;
        }
    }
}


int titleBGM, playingBGM, gameOverBGM, startSE, enemyHitSE, moveSE, shootSE;

// Menu��ʂ�bgm1�����s�[�g
// PLAYING�ł�bgm2�����s�[�g
// GAMEOVER�ɂȂ�����bgm3�����s�[�g

void LoadSounds() {
    titleBGM = LoadSoundMem("bgm/title.wav");
    playingBGM = LoadSoundMem("bgm/playing.wav");
    gameOverBGM = LoadSoundMem("bgm/gameover.wav");

    startSE = LoadSoundMem("sound/start.wav");
    enemyHitSE = LoadSoundMem("sound/hit.wav");
    moveSE = LoadSoundMem("sound/move.wav");
    shootSE = LoadSoundMem("sound/shoot.wav");

    // BGM�����[�v�Đ�
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

    int charactor1 = LoadGraph("fig/charactor01.png");
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
            // �w�i��`�悵�ăQ�[����ʂ��������猩����
            DrawRotaGraph(x1, 240, 0.625, 0.0, back[i % 3], TRUE);
            DrawRotaGraph(x2, 240, 0.625, 0.0, back[i % 3], TRUE);
            DrawRotaGraph(60, 300 - game.y, 0.1, 0.0, charactor1, TRUE);

            // ���������C���[���d�˂�
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
            DrawBox(0, 0, 640, 480, GetColor(100, 100, 100), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

            // �|�[�Y���b�Z�[�W
            DrawFormatString(230, 230, GetColor(255, 255, 255), "Space�������čĊJ");

            // �Q�[���J�n
            if (Key[KEY_INPUT_SPACE] == 1) {
                game.state = PLAYING;
            }
        }
        else if (game.state == MENU) {
            DrawRotaGraph(x1, 240, 0.625, 0.0, back[i % 3], TRUE);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
            DrawBox(0, 0, 640, 480, GetColor(100, 100, 100), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

            DrawExtendGraph(0, -35, 640, 480, titlelogo, TRUE); // �^�C�g��

            // �ō��X�R�A�̕\�������S�̉��ɔz�u
            int text_x = 320 - GetDrawFormatStringWidth("�ō��X�R�A: %d (%dm)", game.high_score, game.L_max) / 2;
            DrawFormatString(text_x, 420, GetColor(75, 0, 130), "�ō��X�R�A: %d (%dm)", game.high_score, game.L_max);

            // SPACE�L�[�̃e�L�X�g������ɉ��ɔz�u
            text_x = 320 - GetDrawFormatStringWidth("SPACE�������ăX�^�[�g") / 2;
            DrawFormatString(text_x, 450, GetColor(0, 0, 0), "SPACE�������ăX�^�[�g");

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

            // 10000m���Ƃɔw�i�؂�ւ�
            if (game.L % 5000 == 0) {
                i = (i + 1) % 3;
            }

            DrawRotaGraph(x1, 240, 0.625, 0.0, back[i % 3], TRUE);
            DrawRotaGraph(x2, 240, 0.625, 0.0, back[i % 3], TRUE);

            game.L += 2;
            DrawFormatString(20, 10, GetColor(0, 255, 0), "%dm", game.L);

            if (game.L % 1000 == 0) game.score += 300; // 1000m���ƂɃ|�C���g���Z
            if ((game.L - game.shot) == 5000 && game.bulletnum < 5) game.bulletnum++; // �e�������Ă���5000m���Ƃɒe�ǉ�
            
            DrawFormatString(100, 10, GetColor(195, 144, 22), "Score:%d", game.score);
            DrawFormatString(400, 10, GetColor(255, 0, 0), "Press A: attack %d/5", game.bulletnum);

            if (Key[KEY_INPUT_UP] >= 1 && game.y < 220) game.y += 6;
            if (Key[KEY_INPUT_DOWN] >= 1 && game.y > 0) game.y -= 6;
            DrawRotaGraph(60, 300 - game.y, 0.1, 0.0, charactor1, TRUE);

            if (Key[KEY_INPUT_A] == 1 && game.bulletnum > 0) {
                FireBullet(game);
                PlaySE(shootSE);
            }
            UpdateBullets(game);
            UpdateEnemies(game);
        }
        else if (game.state == GAME_OVER) {
            // �w�i��`�悵�ăQ�[����ʂ��������猩����
            DrawRotaGraph(x1, 240, 0.625, 0.0, back[i % 3], TRUE);
            DrawRotaGraph(x2, 240, 0.625, 0.0, back[i % 3], TRUE);
            DrawRotaGraph(60, 300 - game.y, 0.1, 0.0, charactor1, TRUE);

            // ���������C���[���d�˂�
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
            DrawBox(0, 0, 640, 480, GetColor(100, 100, 100), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

            // �ō��L�^�̍X�V
            SaveHighScore(game);

            // GAME OVER ���b�Z�[�W
            SetFontSize(48); // �t�H���g�T�C�Y��傫������

            int text_x = 320 - GetDrawFormatStringWidth("GAME OVER") / 2;
            DrawFormatString(text_x, 120, GetColor(255, 0, 0), "GAME OVER");
            SetFontSize(20); // ���̃e�L�X�g�͒ʏ�T�C�Y

            text_x = 320 - GetDrawFormatStringWidth("�X�R�A: %d (%dm)", game.score, game.L) / 2;
            DrawFormatString(text_x, 230, GetColor(255, 255, 255), "�X�R�A: %d (%dm)", game.score, game.L);

            text_x = 320 - GetDrawFormatStringWidth("�ō��X�R�A: %d (%dm)", game.high_score, game.L_max) / 2;
            DrawFormatString(text_x, 290, GetColor(255, 255, 0), "�ō��X�R�A: %d (%dm)", game.high_score, game.L_max);

            text_x = 320 - GetDrawFormatStringWidth("Space�������ă��X�^�[�g") / 2;
            DrawFormatString(text_x, 330, GetColor(255, 255, 255), "Space�������ă��X�^�[�g");

            if (Key[KEY_INPUT_SPACE] == 1) {
                InitGame(game);
                game.state = PLAYING;
            }
        }
    }
    DxLib_End();
    return 0;
}
