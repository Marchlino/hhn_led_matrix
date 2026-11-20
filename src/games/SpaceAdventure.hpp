#include "Game.hpp"
#include "../utils/Object3D.hpp"


typedef struct {

    Player* player;
    Object3D spaceship;
    Vec2 direction;
    Vec2 position;
    Vec3 rotation;

} SpaceAdventure_Player;


typedef struct {

    Object3D obj;
    int life;
    Vec2 position;

} SpaceAdventure_Asteroid;


class SpaceAdventure: public Game {

    private:

        std::vector<SpaceAdventure_Asteroid*> _asteroids;
        std::vector<SpaceAdventure_Player*> _players;
        std::vector<Vec2> _bullets;
        Object3D_Mesh* _asteroids_mesh;
        Object3D_Mesh* _player_mesh;
        Camera* _camera;
        uint_fast32_t _iterations = 0;
        int _global_score;

        int _getPlayerIndex(Player* );

    public:

        SpaceAdventure();
        ~SpaceAdventure();
        bool addPlayer(Player* ) override;
        bool removePlayer(Player* )override;
        bool updatePlayer(Player* , std::vector<Value> ) override;
        bool isRunning() override;
        bool render(Image* i) override;
        bool update() override;
        bool reset() override;
        int getControllerType() override;
        std::string getName() override;

};
