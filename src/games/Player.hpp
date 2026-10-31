#pragma once
#include <string>


/**
 * @class Player
 * @brief Represent a connected player.
 * @param std::string The name of the player
 * @param std::string The (unique) id of the player.
 */
class Player {

    private:

        std::string _name;
        std::string _id;

    public:

        Player(std::string name, std::string id);

        /**
         * @fn gtID
         * @brief Return the player ID.
         */
        std::string getID();

};
