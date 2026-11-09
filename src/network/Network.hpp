#pragma once
#include <mutex>

#include "logger.h"
#include "crow.h"
#include "crow/app.h"

#include "../utils/Config.hpp"
#include "../games/MainMenu.hpp"
#include "Player.hpp"


class reLogger : public crow::ILogHandler {

public:

    reLogger() {}
    void log(const std::string& message, crow::LogLevel level) {

        switch (level)
        {
            case crow::LogLevel::Debug:
                logger(0, message.c_str());
                break;
            case crow::LogLevel::Info:
                logger(1, message.c_str());
                break;
            case crow::LogLevel::Warning:
                logger(2, message.c_str());
                break;
            case crow::LogLevel::Error:
                logger(3, message.c_str());
                break;
            case crow::LogLevel::Critical:
                logger(4, message.c_str());
                break;
        }

    }

};


class Network {

    private:

        crow::SimpleApp _app;
        int _port;
        std::string _url;
        std::future<void> _crow_thread;
        std::vector<Player*> _players;
        MainMenu* _game;
        std::mutex _players_mutex;
        uint32_t _sent_version = 0;
        int _next_player_number = 0;

        int getPlayerIndexByConnection( crow::websocket::connection* c );
        static std::vector<Value> parseRequest( const std::string& data );

        /**
         * @fn stateMessages
         * @brief Build the server-client messages describing the menu state
         */
        std::vector<std::string> stateMessages();

    public:

        Network( Config* conf, MainMenu* main );
        ~Network();
        bool init();
        bool run();
        bool stop();

        /**
         * @fn sync
         * @brief Send the menu state to every client if it changed. Call once per frame.
         */
        bool sync();

};
