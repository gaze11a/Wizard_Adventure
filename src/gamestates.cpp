#include "DxLib.h"
#include "game.h"
#include "gamestates.h"
#include "input.h"
#include "sound.h"
#include "score.h"

void HandleMenu(Game& game) {
    if (CheckSoundMem(titleBGM) == 0) {
        PlaySoundMem(titleBGM, DX_PLAYTYPE_LOOP);
    }

    DrawRotaGraph(game.x1, 240, 0.625, 0.0, back[game.i % 3], TRUE);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
    DrawBox(0, 0, 640, 480, GetColor(100, 100, 100), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    DrawExtendGraph(0, -35, 640, 480, titlelogo, TRUE); // �^�C�g��

    // �ō��X�R�A�̕\��
    int text_x = 320 - GetDrawFormatStringWidth("�ō��X�R�A: %d (%dm)", game.high_score, game.L_max) / 2;
    DrawFormatString(text_x, 420, GetColor(75, 0, 130), "�ō��X�R�A: %d (%dm)", game.high_score, game.L_max);

    // SPACE�L�[�̃e�L�X�g
    text_x = 320 - GetDrawFormatStringWidth("SPACE�������ăX�^�[�g") / 2;
    DrawFormatString(text_x, 450, GetColor(0, 0, 0), "SPACE�������ăX�^�[�g");

    DrawFormatString(500, 20, GetColor(0, 0, 0), "�ݒ�: S");

    if (Key[KEY_INPUT_SPACE] == 1) {
        StopSoundMem(titleBGM);

        PlaySE(switchSE);
        WaitTimer(1000);
        PlaySoundMem(playingBGM, DX_PLAYTYPE_LOOP);
        game.state = PLAYING;
    }

    if (Key[KEY_INPUT_S] == 1) {
        PlaySE(switchSE);
        game.state = MENU_SETTINGS;
    }
}

void HandlePlaying(Game& game) {
    if (CheckSoundMem(titleBGM) == 1) {
        StopSoundMem(titleBGM);
    }
    if (CheckSoundMem(playingBGM) == 0) {
        PlaySoundMem(playingBGM, DX_PLAYTYPE_LOOP);
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
    if ((game.L - game.shot) == 10000 && game.bulletnum < 5) game.bulletnum++; // �e�������Ă���10000m���Ƃɒe�ǉ�


    DrawFormatString(125, 10, GetColor(195, 144, 22), "Score:%d", game.score);
    DrawFormatString(400, 10, GetColor(255, 0, 0), "Press D: attack %d/5", game.bulletnum);

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
        PlaySE(shootSE);
        FireBullet(game);
    }
    UpdateBullets(game);
    UpdateEnemies(game);
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

    DrawFormatString(500, 20, GetColor(255, 255, 255), "�ݒ�: S");
    DrawFormatString(230, 230, GetColor(255, 255, 255), "Space�������čĊJ");

    // �ݒ�
	if (Key[KEY_INPUT_S] == 1) {
		game.state = PAUSE_SETTINGS;
        PlaySE(switchSE);
	}

    // �Q�[���ĊJ
    if (Key[KEY_INPUT_SPACE] == 1) {
        game.state = PLAYING;
		PlaySE(switchSE);
    }
}

void HandleGameOver(Game& game) {
    if (CheckSoundMem(titleBGM) == 1) {
        StopSoundMem(titleBGM);
    }
    if (CheckSoundMem(playingBGM) == 1) {
        StopSoundMem(playingBGM);
    }

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
    DrawFormatString(text_x, 350, GetColor(255, 255, 255), "Space�������ă��X�^�[�g");

    DrawFormatString(500, 20, GetColor(255, 255, 255), "�ݒ�: S");

    if (Key[KEY_INPUT_S] == 1) {
        game.state = GAMEOVER_SETTINGS;
        PlaySE(switchSE);
    }

    if (Key[KEY_INPUT_SPACE] == 1) {
        InitGame(game);
        WaitTimer(1000);
        game.state = PLAYING;
    }
}

void HandleSettings(Game& game) {
    // �w�i��`��
    DrawRotaGraph(game.x1, 240, 0.625, 0.0, back[game.i % 3], TRUE);
    DrawRotaGraph(game.x2, 240, 0.625, 0.0, back[game.i % 3], TRUE);

    // ���������C���[���d�˂�
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
    DrawBox(0, 0, 640, 480, GetColor(100, 100, 100), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // �^�C�g��
    int title_x = 320 - GetDrawFormatStringWidth("�ݒ�") / 2;
    DrawFormatString(title_x, 50, GetColor(255, 255, 255), "�ݒ�");

    // �X���C�_�[�p�̕ϐ�
    static int volumeBGM = 200;
    static int volumeSE = 200;
    static int selected = 0;  // 0: BGM, 1: SE

    // BGM���ʂ̕\��
    int bgm_x = 320 - GetDrawFormatStringWidth("BGM ����: %d") / 2;
    DrawFormatString(bgm_x, 150, GetColor(255, 255, 255), "BGM ����: %d", volumeBGM);

    // SE���ʂ̕\��
    int se_x = 320 - GetDrawFormatStringWidth("SE ����: %d") / 2;
    DrawFormatString(se_x, 250, GetColor(255, 255, 255), "SE ����: %d", volumeSE);

    // �X���C�_�[�̕`��
    int sliderWidth = 300;
    int sliderStartX = 320 - (sliderWidth / 2);

    // BGM�X���C�_�[
    DrawBox(sliderStartX, 180, sliderStartX + sliderWidth, 200, GetColor(255, 255, 255), FALSE);
    DrawBox(sliderStartX + volumeBGM, 175, sliderStartX + volumeBGM + 20, 205, GetColor(255, 255, 255), TRUE);

    // SE�X���C�_�[
    DrawBox(sliderStartX, 280, sliderStartX + sliderWidth, 300, GetColor(255, 255, 255), FALSE);
    DrawBox(sliderStartX + volumeSE, 275, sliderStartX + volumeSE + 20, 305, GetColor(255, 255, 255), TRUE);

    // �߂�{�^��
    int back_x = 320 - GetDrawFormatStringWidth("�߂�: SPACE") / 2;
    DrawFormatString(back_x, 400, GetColor(255, 255, 255), "�߂�: SPACE");

    // ���L�[�őI��ύX
    if (Key[KEY_INPUT_UP] == 1 || Key[KEY_INPUT_DOWN] == 1) {
        selected = 1 - selected;
    }

    // ���E�L�[�ŉ��ʕύX
    if (selected == 0) {
        if (Key[KEY_INPUT_LEFT] == 1 && volumeBGM > 0) volumeBGM -= 10;
        if (Key[KEY_INPUT_RIGHT] == 1 && volumeBGM < 255) volumeBGM += 10;
        ChangeVolumeSoundMem(volumeBGM, titleBGM);
        ChangeVolumeSoundMem(volumeBGM, playingBGM);
    }
    else {
        if (Key[KEY_INPUT_LEFT] == 1 && volumeSE > 0) volumeSE -= 10;
        if (Key[KEY_INPUT_RIGHT] == 1 && volumeSE < 255) volumeSE += 10;
        ChangeVolumeSoundMem(volumeSE, switchSE);
        ChangeVolumeSoundMem(volumeSE, hitSE);
        ChangeVolumeSoundMem(volumeSE, shootSE);
    }

    // �I�𒆂̃X���C�_�[�������\��
    if (selected == 0) {
        DrawBox(sliderStartX - 10, 140, sliderStartX + sliderWidth + 10, 210, GetColor(255, 255, 0), FALSE);
    }
    else {
        DrawBox(sliderStartX - 10, 240, sliderStartX + sliderWidth + 10, 310, GetColor(255, 255, 0), FALSE);
    }

    // �ݒ��ʂ����
    if (Key[KEY_INPUT_SPACE] == 1) {
        PlaySE(switchSE);
        switch (game.state)
        {
		case MENU_SETTINGS:
			game.state = MENU;
			break;
		case PAUSE_SETTINGS:
			game.state = PAUSED;
			break;
		case GAMEOVER_SETTINGS:
			game.state = GAME_OVER;
			break;
        }
    }
}

