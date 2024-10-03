# csc581-engine
Group game engine for CSC 581

# How to Run the Submissoin
Must have a Windows 10/11 environment with Visual Studio 2022 for C++ projects. SDL2's dll and libzmq's dll should be added to the environment PATH variable.

Everything is in one Source.zip file.

To build and run the project,

1. Unzip the folder
2. Extract all files to a location of your choice
3. In Visual Studio, choose open a local folder and select the folder you just extracted
4. The default view of a local folder should be folder view
5. In Solution Exporer (right or left side), select "Switch between solutions and available views"
6. There will be a `game-engine.sln` item and a folder item. Double click the `.sln` item.
1. Double check that VS is configured for Debug x64
7. Right click `GameEngineLib` and select Build
1. Right click `Server` and select Build
1. Right click `Client` and select Build
1. Right click `Server` and select 'Select as Startup Project'
1. Run in Non-Debugger Mode, this starts the server
7. Right click `Client` and select 'Select as Startup Project'
8. Run in non-debugger mode 3 times to add 3 clients. (Do not spam run, server needs time to establish connection with each client)

Default Game Controls:

move left: left arrow

move right: right arrow

jump: space bar

quit: q

proportional toggle: s

pause/unpause: p

speed up / slow down: + / -
