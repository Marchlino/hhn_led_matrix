# LED Matrix

<img width="250" height="250" alt="render" src="https://github.com/user-attachments/assets/0f78cc20-a732-4961-bb83-921dee39d86f" />

The goal of the project is to use a LED display to display things.

Currently we have:

- A base project structure with:
  - Some kind of rendering pipeline
  - A game object
  - A main menu to manage each games
- What we need is:
  1. **Add display driver for our LED display**
  2. Implement network management/player management with a Network object and [CrowCpp](https://crowcpp.org/master/) (in include)
  3. Make an actual game and add it to the main menu
  - *Multi-threaded draw calls ?? Basically anything to speed up the rendering process which is fairly slow as we run on 1 CPU core, calling the SDL renderer twice for each pixel might also be a big bottle-neck.*
  - ~~Make the triangle primitive (https://gfxcourses.stanford.edu/cs248a/winter24content/media/drawing/02_drawtriangle.pdf) (needed if we're going to make the circular pong, could even be use for 3D)~~
  - ~~It would be cool to have a Util folder with convenience functions/Macros like RGB() or HSL() to be able to write RGB/HSL color code in the program and directly convert them to uint8_t (or maybe uint16_t in the future)~~
  - ~~Implement font rendering with the lib. [Schrift](https://tomolt.github.io/libschrift/v0.10.2.schrift.3.html) (in include)~~
- What we can do right now is:
  - Start the Main menu
  - Use the main menu to render something on the Image object
  - Display the image object with a display driver
 
Quick note here: The internal image pixels can be accessed by view units, this mean, displaying pixels on a LED display of various density/size become trivial. We can display a 24x86 image on a 546x963 screen the same way we would display this 24x86 on a 24x86 screen, and we could for instance easily display the same image on four screens of 32x64, 98x67, 5420x6320 and 9x10.
  
```mermaid
classDiagram
class Game {

        virtual bool addPlayer(Player* ) = 0;
        virtual bool removePlayer(Player* ) = 0;
        virtual bool updatePlayer(Player* , std::vector<int> ) = 0;
        virtual bool isRunning() = 0;
        virtual bool render(Image* i) = 0;
        virtual bool update() = 0;
        virtual bool reset() = 0;

}

class MainMenu {

        std::vector<Player*> players;
        std::vector<int> next_game_vote;
        std::vector<Game*> games;
        std::vector<int> voted_for;
        int current_game;
        float iterations;
        int getPlayerIndex(Player* );
        int mostVotedGame();
        MainMenu(std::vector<Game*> games);
        bool addPlayer(Player* ) override;
        bool removePlayer(Player* ) override;
        bool updatePlayer(Player* , std::vector<int> ) override;
        bool isRunning() override;
        bool render(Image* i) override;
        bool update() override;
        bool reset() override;

}

class Player {

        std::string name;
        std::string id;
        Player(std::string name, std::string id);
        std::string getID();

}

class Pong {

    std::vector<Player*> players;
    std::vector<int> scores;
    int getPlayerIndex(Player* );
    Pong() = default;
    bool addPlayer(Player* ) override;
    bool removePlayer(Player* )override;
    bool updatePlayer(Player* , std::vector<int> ) override;
    bool isRunning() override;
    bool render(Image* i) override;
    bool update() override;
    bool reset() override;

}

class Image {

        int size_x;
        int size_y;
        std::vector<uint8_t> pixels;
        uint8_t drawing_color;
        float view_unit_x;
        float view_unit_y;
        FT_Library  library;
        FT_Face     face;
        Image(int size_x, int size_y);
        bool loadFont(std::string path);
        uint8_t getPixelAt(int x, int y);
        uint8_t getPixelInView(float vw, float vh);
        bool setPixelAt(int x, int y);
        bool setDrawingColor(uint8_t color);
        bool clear();
        float weigthUnit();
        float heigthUnit();
        bool drawGlyph(std::string glyph, int x, int y, int size_x, int size_y);
        bool drawText(std::string text, float vw, float vh, float weight, float height);
        bool drawRect(float vw, float vh, float weight, float height);
        bool drawCircle(float vw, float vh, float radius);
        bool drawTriangle(float vw1, float vh1, float vw2, float vh2, float vw3, float vh3);

}

class Display {

        virtual bool display(Image* ) = 0;
        virtual ~Display() = default;

}

class Sdl {

        int size_x;
        int size_y;
        SDL_Event event;
        SDL_Renderer* renderer;
        SDL_Window* window;
        Sdl(int x, int y);
        ~Sdl();
        bool display(Image* ) override;

}

class Stdout {

        ~Stdout() = default;
        bool display(Image* ) override;

}

MainMenu --|> Game
MainMenu "*" *-- "*"  Player
Pong --|> Game
Stdout --|> Display
Sdl --|> Display

Game .. Image
Display .. Image
Game "*" *-- "1" MainMenu
```

## Pong Game

Pong is a simple tennis game, but we want a potentially infinite number of players. Our solution to fix this issue is the following (for 8 players here):

<img width="1268" height="1192" alt="geogebra-export" src="https://github.com/user-attachments/assets/f57e2acf-36c1-499d-aa4c-a608a0720f71" /> (Made with Geogebra)

The players would be place following the shape of a circle. To do so, we place N equidistant point for N players on the circle surface, the players as then allowed to move on the tangent of this circle at this point until the border of the screen or the first intersection with an other tangent.

More simply, this shape can be summarized by a polygon, like an hexagon, where each segments are equal in length and angle.
