#include "Game.hpp"
#include "../utils/Object3D.hpp"


typedef struct {

    Player* player;
    Object3D spaceship;

} SpaceInvader_Player;


typedef struct {

    Object3D obj;
    int life;

} SpaceInvader_Asteroid;


class SpaceInvader: public Game {

    private:

        std::vector<SpaceInvader_Asteroid*> _asteroids;
        std::vector<SpaceInvader_Player*> _players;
        std::vector<Vec2> _bullets;
        Object3D_Mesh* _asteroids_mesh;
        Object3D_Mesh* _player_mesh;
        Camera* _camera;
        uint_fast32_t _iterations = 0;
        int _global_score;

        int _getPlayerIndex(Player* );

    public:

        SpaceInvader();
        ~SpaceInvader();
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
