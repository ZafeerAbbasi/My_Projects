## Maze Solving Robot

What can this robot do?

This robot can solve any line maze or possibly a wall maze by knowing the maze layout and design in its memory, then solving it on its screen in memory and then actually solving the maze by following the shortest path to reach the end coordinate. 

What can’t this robot do?

It can not solve any kind of maze that includes loops or islands in it as it follows the RHF rule. 

Other comments:

// the bot will solve the maze in memory first and then actually solve it using the shortest path physically.
// After it reaches the target point, it will showcase the respective on its ev3 screen.
// It tracks the maze using encoder values and so it can go out of bounds if the maze is uneven or unequal.
