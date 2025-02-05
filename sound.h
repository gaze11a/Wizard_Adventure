#ifndef SOUND_H
#define SOUND_H

#include "DxLib.h"

extern int titleBGM, playingBGM, gameOverBGM, startSE, enemyHitSE, moveSE, shootSE;

void LoadSounds();
void PlaySE(int sound);

#endif
