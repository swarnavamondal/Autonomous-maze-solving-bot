This bot solves any maze in given 2 attempts.  The first attempt it will solve the maze but by taking multiple wrong turns and and U-turns. In the second attempt it learns all the errors and solves them to execute an optimized path which will solve the maze seamlessly without any errors
In the first attempt it solves the maze using left wall rule. 
### How It Works
1. **Place your left hand on a wall** when you enter the maze.
2. **Keep walking forward**, making sure your left hand stays touching the wall at all times.
3. Follow the wall until you find the exit.
### Why It Works
The left wall rule works because if you keep your hand on the wall, you will eventually traverse the entire maze and find the exit **as long as the maze is connected (all walls touch)**.

In the second attempt the bot stores its pervious path and learns it. It looks at all its mistakes and the unnecessary turns it has taken and elinimates them.
for example: LBL (left-back-left) is nothing but the bot moving straight. so the bot will elinimate such turns and optimize its path to find the shortest way out of the maze.
