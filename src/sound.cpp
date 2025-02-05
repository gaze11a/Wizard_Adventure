#include "DxLib.h"
#include "sound.h"

// startSE��Pause���ɂ��g��

int titleBGM, playingBGM;
int switchSE, hitSE, shootSE;

// �T�E���h�̓ǂݍ���
void LoadSounds() {
    titleBGM = LoadSoundMem("assets/bgm/title.mp3");
    playingBGM = LoadSoundMem("assets/bgm/playing.mp3");

    switchSE = LoadSoundMem("assets/sound/switch.mp3");
    hitSE = LoadSoundMem("assets/sound/hit.mp3");
    shootSE = LoadSoundMem("assets/sound/shoot.mp3");

    ChangeVolumeSoundMem(200, titleBGM);
    ChangeVolumeSoundMem(200, playingBGM);
}

// ���ʉ����Đ�
void PlaySE(int sound) {
    PlaySoundMem(sound, DX_PLAYTYPE_BACK);
}