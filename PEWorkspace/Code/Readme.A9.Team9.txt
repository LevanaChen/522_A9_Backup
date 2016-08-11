Team 9: Car Racing Demo
Team mmembers: Chicheng Ren, Liquan Chen

Project Proposal: Multiplayer Racing Game with advanced physics

Details:
1. We add the collision system into the game. Now the car will react properly if it runs into another car.
2. Implementation of the server side time sync system. Now the simulation on the other client will make use of this 
sync time to calculate the simulation of the other car which brings more precision. Along with this sync time, the client
is also able to get the ping of the connection.
3. Implementation of the record system. Now the record system will automatically record your best laps and 
store the three best one (Shown on the screen). (In A8, it only shows the last lap)
4. Implementation of the network game, now the two players are able to play together on the network, the movement
of the car is synced.
5. Implementation of the MapMesh. The MapMesh is printed on the screen to show where the car is on the whole map.
This MapMesh would change according to the change of the car position.
6. Platforms: PC, Xbox

Game Controller:

Key Input on PC:
Start Game:	P
Car Controller:
	W - Move Forward
	S - Move Backward
	Left Arrow - Turn Left
	Right Arrow - Turn Right
Debug Controller:
	K: Reload Car Position
	L: Stop the timer	

Key Input on XBox:
Start Game: Start Button
Car Controller:
	Left Stick: Control all the movement

Debug Controller:
	B: Reload Car Position
	A: Stop the timer