## Maze Solving Robot

The Robot used in this project is the LEGO MINDSTORMS EV3, Here is a picture of what the robot looks like:
![lego-ev3-img_0994_wjUbPs5d2Y](https://user-images.githubusercontent.com/86879362/212527739-8380c3ea-7086-47e0-a744-e7f21ab84361.jpg)

### Challenge Objective:
The robot has the line maze information (e.g., dimensions, paths, dead ends, etc.) saved in 
memory. Lines represent paths the robot can use as a guide (e.g., line tracking) to follow while 
solving the maze. Given a starting point, and a target point, the robot calculates the shortest path 
in memory, then physically follows the shortest path to the target. While the robot is calculating 
the shortest path, and moving towards the target, the robot screen should show the status of the 
program graphically on the screen


### What can this robot do?

This robot can solve any line maze or possibly a wall maze by knowing the maze layout and design in its memory, then solving it on its screen in memory and then actually solving the maze by following the shortest path to reach the end coordinate. 


Other comments:

- the bot will solve the maze in memory first and then actually solve it using the shortest path physically.
- After it reaches the target point, it will showcase the respective on its ev3 screen.
- It tracks the maze using encoder values and so it can go out of bounds if the maze is uneven or unequal.

### Here is a video demonstrating the Robot solving a maze on its display and then solving it in real time:
https://user-images.githubusercontent.com/86879362/230726288-f139e19e-5e93-4c4c-b810-387191d925d3.mp4



