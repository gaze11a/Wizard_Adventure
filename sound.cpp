#include "DxLib.h"
#include "sound.h"

int titleBGM, playingBGM, gameOverBGM, startSE, enemyHitSE, moveSE, shootSE;

// �T�E���h�̓ǂݍ���
void LoadSounds() {
    titleBGM = LoadSoundMem("bgm/title.mp3");
    playingBGM = LoadSoundMem("bgm/playing.wav");
    gameOverBGM = LoadSoundMem("bgm/gameover.wav");

    startSE = LoadSoundMem("sound/start.wav");
    enemyHitSE = LoadSoundMem("sound/hit.wav");
    moveSE = LoadSoundMem("sound/move.wav");
    shootSE = LoadSoundMem("sound/shoot.wav");

    ChangeVolumeSoundMem(200, titleBGM);
}

// ���ʉ����Đ�
void PlaySE(int sound) {
    PlaySoundMem(sound, DX_PLAYTYPE_BACK);
}