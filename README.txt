Instructions:
This readme gives a brief set of instructions to setup the project and run your code. Also, refer to User Manual and documentation.

Requirements:
Visual Studio 2012 (http://go.microsoft.com/?linkid=9816768)

Setting up the project:
***************************************************************

Setting up the environment:
...............................................................
Setting up the Project to build in release mode:
1)Click on Build in menu bar and select Configuration Manager from drop-down list.
2)Select Release under the heading Active solution configuration.

Setting up for Simulator:
...............................................................
1)Open the project by opening the strategy.sln file present in Strategy folder.
2)Click on Project and select Strategy Properties from the drop-down list.
3)Go to Configuration Properties -> Build Events -> Post-Build Events
4)Add (without qoutes) "START run.bat" to command line field.

To change your Team colour:
...............................................................
To change your team colour, write your team colour against "Team_colour" field in game.hpp.
Make sure to choose the corresponding team colour in the debugger and simulator as well.

Building the Project:
...............................................................
Press F7 or Build -> Build Solution


...............................................................

To check your code, you need to start the simulator.

Starting the Simulator and running the project:
...............................................................
1)Open the "Debugger.exe" present in installation directory.
2)Click on "Start Strategy" button. 
3)Now after the simulator loads, from the right column select "STRATEGIES" in the simulator window.
4)Against team you want to run, click on "LINGO". It changes to C++.(Refer to Note: Info on competing with an opponent)
5)Now click on Send and then Start button and you are ready to see your code work.

Note:
1)Competing against human opponent:
First of all, it is important that you both select different team colours and build before proceeding.
Data(Team1.dll or Team2.dll file) of your teams is present in C:/Strategy/(blue/yellow) folder. One of you just have to copy this file to other computer's corresponding folder and run the "Debugger.exe". Then click on STRATEGIES and click LINGO -> C++ for both teams. now you can click Start.

2)Debugger will help you to see common parameters like position,velocity and angle of your bots and other parameters that you would like investigate.

Happy Code-O-Soccer Guys
Cheers
Team KRSSG
