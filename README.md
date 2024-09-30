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
1. Right click `MultiThreadedMain` and select 'Select as Startup Project'
7. Right click `MultiThreadedMain` again and select Build
8. Then use VS to run in non-debugger mode (hollow green arrow, not solid)

Default Game Controls:

move left: left arrow

move right: right arrow

jump: space bar

quit: q

proportional toggle: s

pause/unpause: p

speed up / slow down: + / -
