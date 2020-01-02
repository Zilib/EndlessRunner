MIT License

Copyright (c) 2020 Zilib

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

# Endless Runner by Adrian Bielec
## A bit about project.
Endless runner is a game where the hero only run, and the map will never end. The game has two assumptions, run and try to survive as much as you can.

The project is only for develop myself, and improve my C++ and Unreal Engine 4 skill. I would like to use math and my knowledge in practice. 
## Game assumptions, what game will include?
* Collecting system,
* With time, game will be harder,
* One level, but never ending and each time level is different,
* Various kind of corridors,
* Obstacles.

# In details about game systems
## Collecting system
It is an additional score system, player will receive two awards. One scores for stay alive which are independent of player, and collected items. Collected item will have a condition, if you meet it you will receive a reward which will have an impact for a game.
## With time, game will be harder
Oke here is a place where should i tell you something more about map generator. Game level is generating automatically after begin play. The way to make game harder is a use a human response time. 
## Map is generating automatically using script
Script is created using a physical formulas, 