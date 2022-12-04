# Für Vorlesung 2022/12/07
- implement "hit wall" sound
- player animation for moving left an right

# Für Vorlesung 2022/11/30
- update [`src/States/Level.cpp`](src/States/Level.cpp) rendering Method to use frame-buffers (render ui und game into separate texures und render buffered frames then to the window)
- update rendering structure to make access to frame buffers and frame layers accessible
- add Reference to "current" ComplexGameState to every GameObject
- fix minimap animation, when moving from cubeFields to cubeFields fast enough, the arrow changes, but the fade would not reset, this is fixed now
- implement basic cubeFields transition animation
- slow down player movement to one move every 500 ms (adjustable via define in [`src/GameObjects/Player.cpp`](src/GameObjects/Player.cpp))
- created class "CubeObject" ([`src/GameObjects/CubeObject.hpp`](src/GameObjects/CubeObject.hpp), [`src/GameObjects/CubeObject.cpp`](src/GameObjects/CubeObject.cpp))
- some objects that were CubeFields before are now CubeObjects: GravityObjects (FallingStone and RestingStone) and Slider.
- every CubeField now has a <code>Vector<CubeObject *></code>, where all the objects currently positioned on that field can be stored. The constructor of CubeField has been overloaded to accept a <code>Vector<CubeObject *>&</code>.
- the CubeObjects available as of now are: Slider, RestingMagnet, MovingMagnet (which is intended to moves with the player), RestingStone, FallingStone, Flag.
- the CubeFields available as of now are: EmptyField, Wall_1, Wall_2, Grass, PressurePlate.
- implement inter game state routing for finishing levels and loading next
- implement level selector

# Für Vorlesung 2022/11/23
- Neue Zeichnung zu Cube Transition erstellt ([`readme_images/Cube Transitions.jpg`](readme_images/Cube%20Transitions.jpg)) und analysiert
- Cube Transition Logik zusammengefasst ([`Uebergangsfunktionen von Seite zu Seite.txt`](Uebergangsfunktionen von Seite zu Seite.txt)) und im Code gefixt ([`src/GameObjects/CubeMap_playerMovement.cpp`](src/GameObjects/CubeMap_playerMovement.cpp))
- Player darf ab sofort nur noch von einer Cube Side auf eine andere Cube Side gehen, wenn das neue Feld, auf dem er landen würde, auch betretbar ist ([`src/GameObjects/CubeMap_playerMovement.cpp`](src/GameObjects/CubeMap_playerMovement.cpp))
- bugfixing Cube Transisiton bug ([`src/data/DiceData.cpp`](src/data/DiceData.cpp))
- add green Sprite Border in Debug View (SDL_RenderDrawRect), main draw method in [`src/GameObjects/GameObject.cpp`](src/GameObjects/GameObject.cpp) in `drawSpriteBorder`