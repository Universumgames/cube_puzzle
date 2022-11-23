# 2022/11/23
- Neue Zeichnung zu Cube Transition erstellt ([`readme_images/Cube Transitions.jpg`](readme_images/Cube%20Transitions.jpg)) und analysiert
- Cube Transition Logik zusammengefasst ("Uebergangsfunktionen von Seite zu Seite.txt") und im Code gefixt ([`src/GameObjects/CubeMap_playerMovement.cpp`](src/GameObjects/CubeMap_playerMovement.cpp))
- Player darf ab sofort nur noch von einer Cube Side auf eine andere Cube Side gehen, wenn das neue Feld, auf dem er landen würde, auch betretbar ist ([`src/GameObjects/CubeMap_playerMovement.cpp`](src/GameObjects/CubeMap_playerMovement.cpp))
- bugfixing Cube Transisiton bug ([`src/data/DiceData.cpp`](src/data/DiceData.cpp))
- add green Sprite Border in Debug View (SDL_RenderDrawRect), main draw method in [`src/GameObjects/GameObject.cpp`](src/GameObjects/GameObject.cpp) in `drawSpriteBorder`