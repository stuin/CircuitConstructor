# Quick key to the level creation system

## Tiles
Each character in a level .txt file represents one tile, or the starting point of a block.
Dirt/grass blocks can be swapped out for snow variants.

| Char | Tile       		| Collision |
|------|--------------------|-----------|
| #    | dirt block 		| FULL      |
| /    | grass slope		| SLOPERIGHT|
| -    | grass flat     	| FULL      |
| \    | grass slope		| SLOPELEFT |
| '    | dirt platform  	| PLATFORM  |
| []   | grass edge     	| FULL      |
| {}   | ice upper edge  	| FULL 		|
| =    | ice upper middle  	| FULL 		|
| ()   | ice lower edge  	| PLATFORM 	|
| u    | ice lower middle  	| PLATFORM 	|
| ""   | wood platform  	| PLATFORM  |
| T    | supported platform | PLATFORM  |
| N    | wood supports 		| EMPTY  	|
| H    | ladder 			| PLATFORM  |
| _    | box button 		| EMPTY  	|
| ,    | player button 		| EMPTY  	|
| f    | flag 				| EMPTY  	|
| <    | sign 				| EMPTY  	|
| >    | sign 				| EMPTY  	|
| b    | bones				| EMPTY  	|
| ^    | icicles			| EMPTY  	|
| t    | tent  				| EMPTY 	|
| E    | yeti  				| EMPTY 	|
| ?    | invisible barrier  | FULL 		|
| ~    | hidden platform  	| PLATFORM? |
| %    | hidden door  		| FULL? 	|
| P    | player spawn  		| EMPTY 	|
| w    | wood block  		| EMPTY 	|
| i    | ice block  		| EMPTY 	|
| g    | gold block  		| EMPTY 	|
| m    | large block  		| EMPTY 	|

## world.json
{
	"id": "Section number, must be in list order",
	"file": "File name of tilemap .txt",
	"tile_offset": "0 for grass, 100 for snow",
	"up": "Section id to place above",
	"right": "Section id to place to the right",
	"down": "Section id to place below",
	"left": "Section id to place to the left",
	"x_offset": "Number of tiles to shift this section horizontally, + to right, - to left",
	"y_offset": "Number of tiles to shift this section vertically, + for down, - for up",
	"grab_camera": "Center camera on this section when player is inside",
	"zoom": "Zoom factor when player is inside, 1.5 = default, + to zoom out, - to zoom in",
	"sign_text": "Text to display above flags and signs",
	"invert_trigger": "Start with hidden platforms/doors shown"
}


