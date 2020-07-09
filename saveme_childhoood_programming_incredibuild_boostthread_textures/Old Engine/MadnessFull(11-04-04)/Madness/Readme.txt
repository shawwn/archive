Madness Engine

- Press enter to print to the console.
- Tab toggles mouselook.

====================
= Console commands = 
====================
When specifying a mapname, you do not need to include a folder path or 
extention.  All maps are loaded at coordinates (0,0,0).

map (mapname) - Loads a Quake3 BSP from the Data/Maps/ folder.
	The four included maps are ghettomap, ghettobuilding, 
	newghettobuilding and concrete_jungle.
hmap (mapname) - Loads a heightmap from the /Data/Heightmaps/ folder.
	The three included test heightmaps are test, test2, and test3.
hmap (mapname) (scale) (height) - Scale allows you to specify the 
	number of units per pixel of the heightmap, and height is the 
	number of units for the height of the heightmap.  I'd reccomend 
	using hmap (mapname) 1 3 for viewing purposes.
lightmaps - Toggles viewing the lightmaps on a BSP.
deallocate - Deletes everything currently allocated.
resolution (width) (height) - Sets the current resolution to this width and height.
resolution (width) (height) (fullscreen 1 or 0) - passing 1 for fullscreen will result in fullscreen, 0 for windowed.
