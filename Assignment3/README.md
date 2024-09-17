 # Tennis Court Manager: A Multi-Client Tennis Scheduling and Game Simulation System
## Overview
This project implements a concurrent server-client system to manage the scheduling of tennis matches on four courts. The program schedules games based on player preferences (singles or doubles), manages timeouts, and handles court allocation. After the match, players exchange congratulatory messages using MPI (Message Passing Interface).

The project evolves in three phases:

#### Phase 1: Court scheduling system based on player preferences and court availability.
#### Phase 2: Multi-client concurrent server using OpenMP to handle random player arrivals and manage waiting times.
#### Phase 3: Post-game interaction using MPI, where losing players send congratulatory messages to winners.
## Features
### Game Types: 
 Supports Singles and Doubles matches for male (M) and female (F) players.
### Player Preferences:
 Players may prefer Singles or Doubles, or indicate flexible preferences with "b" or "B" for backup options.
### Court Allocation:
 Manages up to 4 courts with dynamic scheduling based on real-time player arrivals.
### Timeout Handling:
 Players waiting for more than 30 minutes are notified with a timeout response.
### Post-Game Messaging:
Winners and losers exchange messages using MPI after the game is over.

### Game Duration
Singles: 10 minutes for Male players, 5 minutes for Female players.
Doubles: 15 minutes for Male players, 10 minutes for Female players.
Mixed Gender Matches: Game duration is determined by the Male player’s time.
Input Format
Each line in the input file represents a player's attributes:

Player-ID Arrival-time Gender Preference
1 1 M S
2 2 F S
3 3 M D
4 4 F b

## Output Format
The output includes the game details and timeout information. For the post-game phase, message exchanges between players are also shown.

### Sample Output (Phase 2: Concurrent Server)
Player-ID Game-start-time Game-end-time Court-Number Game-type
1 2 12 1 S
2 2 12 1 S
3 -1 33

### Sample Output (Phase 3: MPI Messaging)
Src-Process-Rank, Src-Player-Id To Dst-Process-Rank, Dst-Player-Id: <Message>
1, 3 To 0, 1: Congratulations!
0, 1 To 1, 3: Thank you!

## Setup and Usage
### Requirements:
OpenMP
MPI
C/C++ compiler (GCC recommended)

### Compilation
#### OpenMP
 g++ -fopenmp -o ser ser.cpp
 
 run = ./ser

 #### MPI 
 mpic++ cl1.cpp -o cl
##### run
 mpirun -np 20 ./cl                     //20 is number of clients


