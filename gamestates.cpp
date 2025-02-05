#include "DxLib.h"
#include "game.h"
#include "gamestates.h"
#include "input.h"
#include "sound.h"

void HandleMenu(Game& game) {
    if (CheckSoundMem(titleBGM) == 0) {
        PlaySoundMem(titleBGM, DX_PLAYTYPE_LOOP);
    }

    DrawRotaGraph(game.x1, 240, 0.625, 0.0, back[game.i % 3], TRUE);
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
        StopSoundMem(titleBGM);
        PlaySE(startSE);
    }
}

void HandlePause(Game& game) {
    if (CheckSoundMem(titleBGM) == 1) {
        StopSoundMem(titleBGM);
    }
    // �w�i��`�悵�ăQ�[����ʂ��������猩����
    DrawRotaGraph(game.x1, 240, 0.625, 0.0, back[game.i % 3], TRUE);
    DrawRotaGraph(game.x2, 240, 0.625, 0.0, back[game.i % 3], TRUE);
    DrawRotaGraph(60, 300 - game.y, 0.1, 0.0, charactor, TRUE);

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

void HandlePlaying(Game& game) {
    if (CheckSoundMem(titleBGM) == 1) {
        StopSoundMem(titleBGM);
    }

    game.x1 -= 2;
    if (game.x1 <= 0) game.x2 -= 2;
    if (game.x2 == 640) {
        game.x1 = game.x2;
        game.x2 = 1280;
    }

    // �w�i
    DrawRotaGraph(game.x1, 240, 0.625, 0.0, back[game.i % 3], TRUE);
    DrawRotaGraph(game.x2, 240, 0.625, 0.0, back[game.i % 3], TRUE);

    game.L += 2;
    //DrawFormatString(20, 10, GetColor(0, 255, 0), "%dm", game.L);
    if (game.L % 10000 == 0) game.i = (game.i + 1) % 3; // 10000m���Ƃɔw�i�؂�ւ�

    if (game.L % 1000 == 0) game.score += 300; // 1000m���ƂɃ|�C���g���Z
    if ((game.L - game.shot) == 10000 && game.bulletnum < 3) game.bulletnum++; // �e�������Ă���10000m���Ƃɒe�ǉ�


    DrawFormatString(125, 10, GetColor(195, 144, 22), "Score:%d", game.score);
    DrawFormatString(400, 10, GetColor(255, 0, 0), "Press D: attack %d/3", game.bulletnum);

    for (int i = 0; i < game.life; i++) {
        DrawRotaGraph(35 + i * 30, 20, 0.1, 0.0, heartIMG, TRUE);
    }

    // �L�����N�^�[�̈ړ�
    if (Key[KEY_INPUT_UP] >= 1 && game.y < 220) game.y += 6;
    if (Key[KEY_INPUT_DOWN] >= 1 && game.y > 0) game.y -= 6;

    // �L�����N�^�[�̕`��
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

void HandleGameOver(Game& game) {
    // �w�i��`�悵�ăQ�[����ʂ��������猩����
    DrawRotaGraph(game.x1, 240, 0.625, 0.0, back[game.i % 3], TRUE);
    DrawRotaGraph(game.x2, 240, 0.625, 0.0, back[game.i % 3], TRUE);

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