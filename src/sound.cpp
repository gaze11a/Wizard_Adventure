#include "DxLib.h"
#include "sound.h"

// startSEはPause時にも使う
// 音が消えてから（1s待ってから）ゲームスタート

int titleBGM, playingBGM;
int startSE, hitSE, shootSE;

// サウンドの読み込み
void LoadSounds() {
    titleBGM = LoadSoundMem("assets/bgm/title.mp3");
    playingBGM = LoadSoundMem("assets/bgm/playing.mp3");

    startSE = LoadSoundMem("assets/sound/start.mp3");
    hitSE = LoadSoundMem("assets/sound/hit.mp3");
    shootSE = LoadSoundMem("assets/sound/shoot.mp3");

    ChangeVolumeSoundMem(200, titleBGM);
    ChangeVolumeSoundMem(200, playingBGM);
}

// 効果音を再生
void PlaySE(int sound) {
    PlaySoundMem(sound, DX_PLAYTYPE_BACK);
}