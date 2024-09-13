# Assignment 2: OpenMP Concurrent Server Programming
Due February 14, 2024 11:59 PM
Instructions
Write a program for a concurrent server using OpenMP that manages Tennis courts.
There are 4 Tennis courts
A client represents a player, and they arrive randomly.
Player/ Game Characteristics
Gender: M or F
Game Preference: S, D, b, B
S -> Singles (2 players),
D -> Doubles (4 players),
b -> first preference S and second preference D
B -> first preference D and second preference S
Each game lasts for 'x' minutes where x is:
Singles Game: Male-10 min ,Female-5 min
Doubles Game: Male-15 min, Female: 10 min
If the Game consists of both M and F players, take the time of the M player.
The server should return a response to the client only after the match is over.
The maximum waiting time for the client to get allotted a match is 30 minutes, after which the client should receive a response indicating the timeout.
Each line of the input is an independent client instance.
Input Format: (Same as the previous assignment)
Each line corresponds to one player/client and their attributes.
Player-ID Arrival-time Gender Preference
1 1 M S
2 2 F S
3 3 M D

Output Format:
Player-ID Game-start-time Game-end-time Court-Number Game-type
(OR)
Player-ID -1 timeout-response-time

1 2 12 1 S 
2 2 12 1 S
3 -1 33  
