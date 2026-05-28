# Donkey Kong
## Video Game Design and Development 1st Year Project.
Based on Donkey Kong (1981) by Nintendo






# LINK TO VIDEO






A recreation made by the SlopSquad Team.  
https://github.com/xdHyfa/Project-I-DonkeyKong  

Visit our website!
https://xdhyfa.github.io/Project-I-DonkeyKong/




Marcel Ribot Llorens: https://github.com/JackP-14  
Lux Duart Pérez: https://github.com/Ballo0x  
Daniel Martínez Martínez: https://github.com/Dunge0nraider  
Nil Fernández Muniente: https://github.com/xdHyfa  






## Built With




- **C++** — Core language used for all game logic and systems
- **raylib** — Graphics and input library used for rendering, audio, and window management






Donkey Kong (1981), developed by Nintendo, is an early arcade platform game that introduced  
the character later known as Mario, then called “Jumpman.” The gameplay involves navigating a  
multi-level construction site while avoiding obstacles such as rolling barrels and climbing  
ladders to reach the top. The primary objective is to rescue a character named Pauline from  
a captive gorilla, establishing core mechanics that would influence the platform genre.  






## HOW TO RUN
This project is optimized for 1920 × 1080 resolution. A minimum display resolution of 1280 × 960 is required.
To launch the game, double-click DonkeyKong.exe.




## Controls
Objective: Save Pauline while traversing the three main levels.  
Level 1: Reach Pauline who is located on the top platform while dodging both barrels and fire spirits.  
Level 2: Reach the star dodging the goombas and the Bullet Bills.
Level 3: Press all the buttons found around the level in order to throw Donkey Kong down.






↑ ↑ ↓ ↓ ← → ← → B A: Open shop
Arrows: Traverse through the shop
Enter: Accept
6 7: Give 676767 points


### Player 1


| Action | Keyboard |
|---|---|
| Move Left | `←` |
| Move Right | `→` |
| Climb Up | `↑` |
| Climb Down | `↓` |
| Jump | `Space` |


### Player 2


| Action | Keyboard | Gamepad |
|---|---|---|
| Move Left | `A` | D-Pad ◀ |
| Move Right | `D` | D-Pad ▶ |
| Climb Up | `W` | D-Pad ▲ |
| Climb Down | `S` | D-Pad ▼ |
| Jump | `Shift` | `A` Button |




*Debug Tools:*
R: Upwards Dash
Q: Downwards Dash
1: Switch to First Level
2: Switch to Second Level
H: Show Hitboxes






## Features
Jumpman movement — Responsive controls with the classic Donkey Kong-style jump inertia.
Ramp collision — Full ramp detection and physics response.
Fall Damage — Inertia based on the vertical component that kills you when reaching certain velocity.
Ladder climbing — Vertical traversal with ladder grab and release mechanics.
Barrel system — Barrel spawning, rolling movement and collision.
Fire enemies — Fire spawning and autonomous enemy behavior.
Donkey Kong & Lady — Fully implemented character entities.
Two playable levels — Each with an independent map, entity, and collision logic.
Title & initial screen — Game entry flow with branded screens.
Cutscene system — Narrative sequences between game states.
Win screen — End condition with HowHigh score display.
UI system — In-game interface rendering.
In-Game Score — Real-time score tracking with dynamic updates during gameplay.
Pickable Items — Collectible objects scattered across levels that award points on pickup.
3 Lives — Player starts with three lives, losing one on death and triggering game over when all lives are exhausted.
Shop — Persistent in-game shop where players spend earned points on the following upgrades:
Two Player Mode — Local co-op support with a second simultaneous Jumpman. Player 2 supports both keyboard and gamepad input via the raylib controller API, with full axis and button mapping for movement, jump, and ladder interaction.
Custom Music — Replaces the original soundtrack with an alternate music pack.
God Mode — Grants invincibility and exposes a suite of debug tools for level inspection.
Hammer Start — Every new level begins with the hammer already equipped.
Third Level — A new stage with original level design, introducing two new enemy types and escalating hazards.
Goombas — New autonomous ground enemies that patrol platforms and damage the player on contact.
Bullet Bills — New projectile obstacles launched horizontally across the stage, requiring precise timing to avoid.
Star — New collectible pickup that grants temporary invincibility and a speed boost on collection.








This project is a fan-made recreation for educational purposes only. Donkey Kong is a trademark of Nintendo.




LICENSE

All game assets (sprites, sounds) are property of Nintendo Co., Ltd. and are not covered by this license. This project is a fan recreation for educational purposes only.


MIT License


Copyright (c) 2026 SlopSquad.


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.