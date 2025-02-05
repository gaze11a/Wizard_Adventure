#include "DxLib.h"
#include "sound.h"

// startSE��Pause���ɂ��g��
// ���������Ă���i1s�҂��Ă���j�Q�[���X�^�[�g

int titleBGM, playingBGM;
int startSE, hitSE, shootSE;

// �T�E���h�̓ǂݍ���
void LoadSounds() {
    titleBGM = LoadSoundMem("assets/bgm/title.mp3");
    playingBGM = LoadSoundMem("assets/bgm/playing.mp3");

    startSE = LoadSoundMem("assets/sound/start.mp3");
    hitSE = LoadSoundMem("assets/sound/hit.mp3");
    shootSE = LoadSoundMem("assets/sound/shoot.mp3");

    ChangeVolumeSoundMem(200, titleBGM);
    ChangeVolumeSoundMem(200, playingBGM);
}

// ���ʉ����Đ�
void PlaySE(int sound) {
    PlaySoundMem(sound, DX_PLAYTYPE_BACK);
}