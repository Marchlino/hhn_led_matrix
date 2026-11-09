# How to add a game

Most of what games do is automated by the display part (rendering) and the network part (player managment/request). The main menu handle everything related to managing the games themselves by loading/unloading them when needed.

All that make creating and adding a new game a somewhat easy process:

## Add a Game object

You first need to create a new daughter class of the mother class Game. Override all the abstract functions.

- `addPlayer` is called when a player join your game.
- `removePlayer` is called when a player quit your game.
- `updatePlayer` is called by a player to update its in-game values, for instance position. The values you receive depend of the controller you use.
- `isRunning` is used to check if the game is still running, for instance, your game has a `end` state which completly end the game. (not that useful)
- `render` if called every frame with a canva you can use to draw on.
- `update` is called every frame to update the game (for instance physique or whatever)
- `reset` reset the game to the state it was at the creation of the object (delete all the players, etc)
- `getControllerType` allow you to select the type of controller you want. 1 is a slider, 2 is a D-pad.
- `getName` is for the name of your game. It will be displayed on the UI.

Things you might need:

- `utils/geometry.hpp` contain fonction and structures to use vectors, rotate them and translate them from 3D space to 2D space. If you plan to use translate2D(), you will NEED to initialize a camera object with a Image and a angle to pre-calculate the focal length for x and y axis.
- `utils/colors.hpp` contain a function to convert RGB code to uint32_t and a RGB type to abstract the uint32_t type.
- `display/Image.hpp` represent the canva you will have to draw on, the class already has some base drawing function like drawRectangle, drawCircle, drawTriangle, etc. This in combination of geometry.cpp allow you to do simple 3D and 2D graphics. You can also drawText, but avoid drawGlyph.
- `updatePlayer` give you a list of Values which number and type depend of the controller type. A slider will only give you 1 Value of type int, a D-pad 4, etc. You always have one more Value at the start of the list which is the command code. If you don't need it, ignore it.

## Integrate the Game object in the program

To register your game, include it in `main.cpp` and add it to the `std::vector<Game*> games` list, simple as that. 

Then, you can add an illustration for your game to display on the UI, to do so, place a an image in png format in the folder `/www/static/games` named as the name you gave to your game in lower case, with special character such as â, Ä all in ascii and space replaced with underscores. For instance, `Thïs âmazing GAME` illustration image would be `this_amazing_game.png`.

Everythin else is already being handled for u.
