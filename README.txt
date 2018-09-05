Names: Dustin Hengel, Madison Williams, Bradley Worthen
Team: 10
Class: CS3050
Assignment: Final Project

HOW TO RUN OUR PROGRAM:
======================
Our program's command line requires the executable name as well as the input file name in the following format: <executable> <input file>. 

HOW TO INTERPRET OUR OUTPUT: 
===========================
Our program gives the user the option of seeing the Robots move around the room in a flipbook format or simply will show the paths taken by the robots. 

The program will run and print out the room with paths taken by each robot, and then ask the user if they would like to see the flipbook format of the robots traveling from startpoint to endpoint. It will also give the user an estimation of how long it will take the flipbook to run. 

If the user chooses to view the flipbook the room will be printed with each iteration of the DFS, and screen clears each time creating a flipbook like appearance that shows the robots moving around the room. The robots will also leave a trail behind them so the user can see which spaces were visited by which robot. 


The following symbols will be used in our output: 

'#'- represents a wall or obstacle. Robots cannot move through the walls or obstacles.
' '- represents a space in the room that does not currently hold an endpoint or a robot, and did not hold a robot at any point during runtime.
'S'- represents the position of Robot1 in the room during runtime.
'E'- represents the exit position of Robot1. The letter E will not move during runtime but the letter S will eventaully take place of the E when Robot1 reaches its exit position. 
'F'- represents the position of Robot2 in the room during runtime.
'L'- represents the exit position of Robot2. The letter L will not move during runtime but the letter F will eventaully take place of the L when Robot1 reaches its exit position. 
'-'- represents the path taken by Robot1. This space was only visited by Robot1 during runtime. 
'~'- represents the path taken by Robot2. This space was only visited by Robot2 during runtime. 
'='- represents a space in the path of both Robot 1 and Robot2. This space was visited by both robots during runtime. 
