#include "DxLib.h"
#include "sound.h"

int titleBGM, playingBGM, gameOverBGM, startSE, pauseSE, enemyHitSE, moveSE, shootSE;

// ƒTƒEƒ“ƒh‚Ì“Ç‚İ‚İ
void LoadSounds() {
    titleBGM = LoadSoundMem("assets/bgm/title.mp3");
    playingBGM = LoadSoundMem("assets/bgm/playing.mp3");
    gameOverBGM = LoadSoundMem("assets/bgm/gameover.mp3");

    startSE = LoadSoundMem("assets/sound/start.wav");
    pauseSE = LoadSoundMem("assets/sound/start.wav");
    enemyHitSE = LoadSoundMem("assets/sound/hit.wav");
    moveSE = LoadSoundMem("assets/sound/move.wav");
    shootSE = LoadSoundMem("assets/sound/shoot.wav");

    ChangeVolumeSoundMem(200, titleBGM);
}

// Œø‰Ê‰¹‚ğÄ¶
void PlaySE(int sound) {
    PlaySoundMem(sound, DX_PLAYTYPE_BACK);
}