# LED Matrix

<img width="256" height="256" alt="test" src="https://github.com/user-attachments/assets/558b73d4-642e-45f0-a329-b9b784aeb976" />

The goal of the project is to use a LED display to display things.

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

class Config {
        int parse( std::string );
        std::string get( std::string );
        std::string get( int );
        std::vector<std::string> keys();
        std::vector<std::string> values();
        int size();
        std::vector<std::string> _keys;
        std::vector<std::string> _values;
}

class Network {
        crow::SimpleApp _app;
        int _port;
        std::string _url;
        std::future<void> _crow_thread;
        reLogger* _logger;
        std::vector<Player*> _players;
        MainMenu* _game;
        int getPlayerIndexByConnection( crow::websocket::connection* c );
        Network( Config* conf, MainMenu* main );
        ~Network();
        bool init();
        bool run();
        bool stop();
}

class Player {
        std::string _name;
        std::string _id;
        Game* _game;
        crow::websocket::connection* _connection;
        Player( crow::websocket::connection* connection );
        std::string getID();
        bool setName( std::string );
        std::string getName();
        bool updateGame( std::string );
        bool setGame( Game* g );
        bool exitGame();
}

MainMenu --|> Game
MainMenu "*" .. "*"  Player
Network "*" *-- "*" Player
Pong --|> Game
Stdout --|> Display
Sdl --|> Display
Sdl <-- Config
Network <--Config

Game .. Image
Display .. Image
Game "*" *-- "1" MainMenu
```

## Build

Currently, it is not possible to build the project without building all the drivers and their dependencies, meaning that even if you plan to only use it with stdout, you will need SDL.

Install SDL2, FreeType and asio header files.
```bash
#Fedora
sudo dnf install sdl2-devel freetype-devel asio-devel

#Debian
sudo apt install libsdl2-dev libfreetype-dev libasio-dev
```

Build the project.
```bash
cmake -S . -B build
make

# Or with Ninja
cmake -S . -B build -G Ninja
ninja
```

You can also generate the documentation with Doxygen.
```bash
doxygen
```

## Run

To run the program, you need to pass as a parameter the path to a config file. You can find an exemple config file at the root of the repository (`exemple_config`). Configuration currently look like this :

```ini
matrix_x_size=256
matrix_y_size=256
font_path=
driver=sdl
sdl_x_size=512
sdl_y_size=512
port=8080
```

You should set the font_path with a valid ttf file to display fonts.

CrowCpp look for static files and template files in `/static` and `/templates` folders, you should place the content of `/www` in `/build` or create a link for Crow to deliver the web pages.

3D objects and assets for games should all be stored in `/assets` folder accessible from the current path. 

## Communication

The UI and the server communicate via websocket using this format:

- From player to server:
```
0,[player name]
1,0,[Index of the game to vote for]
1,1,[game commands]
```

- From server to player:
```
0,[controller type]
```
