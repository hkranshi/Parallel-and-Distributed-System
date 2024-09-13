# Assignment 1: Concurrent Programming
Due January 31, 2024 11:59 PM
Instructions
There are 4 Tennis courts and players arrive randomly to play. 

Characteristics of the player:
Gender: M or F
Game Preference: S, D, b, B 
S -> Singles (2 players),
D -> Doubles (4 players),
b -> first preference S and second preference D
B -> first preference D and second preference S
Each game lasts for 'x' minutes where x is:
Singles Game
Male: 10 min
Female: 5 min
Doubles Game
Male: 15 min
Female: 10 min
Write a program that implements this scenario.


Update:
A game can consist of both M and F players. In such a case, take game time to be that of a M player.

The input for the program must be taken from a file (.txt or .csv). Attach a sample input file along with your program.

Sample input:
Each line corresponds to one player and their attributes.
Player-ID Arrival-time Gender Preference
1 1 M S
2 1 F S
3 2 M S
4 3 M b

Sample Output:
Each line corresponds to a game played.
Game-start-time Game-end-time Court-Number List-of-player-ids
1 11 1 1 2
3 13 2 3 4 

In the first output line, the game started at time 1 at court 1 with players with ID 1 and 2.

If you have any queries, feel free to contact me.

Update 2:
I have updated the output format above to include Game-end-time. Please make a note of it and update your assignment accordingly.

