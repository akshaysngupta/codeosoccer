Important!!!!!!!

You have to use the new 'Strategy.h' file to create correct DLL file for Robot Soccer v1.5a
What you have to do is just replace your old 'Strategy.h' file to new 'Strategy.h' file which is in this folder.



---------------------------------------------------------------------------------------------
The MyStrategy project is based on the Win32 DLL wizard option in VC++.

It contains a sample strategy and can be compiled down and placed in the Director folder to be used.

Developers can use this project as a starting point for developing a DLL-based strategy. 

The project creates a MyStrategy.dll file, however, any file name should be acceptable as long as the Director program allows users to select their own strategy DLLs.

The Strategy.h file contains the function prototypes which will be exported from the DLL: Create, MyStrategy, and Destroy. And also the Environment structure.

The Environment structure is passed into the MyStrategy function and is used to access information about the robots, ball and field. It can also be used to set the velocities of the robots.

Global data is not possible in the DLL as both robot teams may wish to use the same DLL file. So a user structure can be allocated and assigned to the userData field in Environment.h.

Because user data only needs to be allocated once it can be done in the Create function which is called once when the Xtra is loaded.

The user data can be freed in the Destroy function which is called when the Xtra is unloaded.