# 🎩 Wizard Adventure 🧙‍♂️✨

![ゲームのロゴ](https://github.com/gaze11a/Wizard_Adventure/blob/master/assets/fig/title.webp?raw=true)

## 🕹️ ゲーム概要
魔法使いが飛び続ける横スクロールアクションゲーム！  
火の玉やドラゴンを避けながら、魔法を放って敵を倒そう！

## 📸 ゲーム画面
![プレイ画面](https://github.com/gaze11a/Wizard_Adventure/blob/master/assets/screenshots/gameplay.png?raw=true)

---

## 🎮 操作方法
| キー | 操作内容 |
|------|------------------------|
| ↑ (上キー) | キャラクターを上に移動 |
| ↓ (下キー) | キャラクターを下に移動 |
| D | 魔法を発射 |
| SPACE | タイトル画面でスタート / ポーズ解除 |
| ESC | 一時停止 (PAUSE) |

---

## ⚖️ ゲーム仕様
### ゲームフロー
1. タイトル画面で **SPACE キー** を押してスタート
2. 魔法を使って迫りくる敵を倒せ！
3. 徐々に速くなる敵を避けながら、高得点を目指そう！

### プレイ内容
- **敵**
  - 🔥 **火の玉**: ぶつかるとライフ -1
  - 🐉 **ドラゴン**: ぶつかると即死
- **スコア**
  - 敵を倒す: 火の玉 (+100点), ドラゴン (+500点)
  - 1000m進むごとに +300点
- **ライフ**
  - 初期ライフ 3
  - 火の玉に当たると -1
  - ライフが 0 になるとゲームオーバー
- **敵のスピード**
  - 5000m進むごとに速度アップ
  - 100000mで最大速度

---

## 📄 ファイル構成
```
Wizard_Adventure/
├── assets/            # 画像・BGM・効果音
│   ├── fig/           # ゲーム内画像
│   ├── bgm/           # BGM
│   ├── sound/         # 効果音
│   └── screenshots/   # スクリーンショット
│
├── src/               # ソースコード
│   ├── main.cpp       # エントリーポイント
│   ├── gamestates.cpp # ゲームの各ステート処理
│   ├── enemy.cpp      # 敵の管理
│   ├── bullet.cpp     # 弾の管理
│   ├── score.cpp      # スコア管理
│   ├── sound.cpp      # 音楽・SE管理
│
└── README.md          # このファイル
```
