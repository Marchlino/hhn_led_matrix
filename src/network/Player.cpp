#include <cstdint>
#include <string>
#include <vector>

#include "Player.hpp"
#include "../utils/request.hpp"
#include "logger.h"


Player::Player( crow::websocket::connection* connection ) {

    this->_connection = connection;
    this->_name = "";
    this->_game = nullptr;

    // The connection may already be closing when the ID is needed later.
    try {
        this->_id = connection->get_remote_ip() + ":" + std::to_string( connection->get_remote_port() );
    } catch ( const std::exception& ) {
        // Socket already gone.
        this->_id = "conn@" + std::to_string( reinterpret_cast<uintptr_t>( connection ) );
    }

}

std::string Player::getID() {

    return this->_id;

}


crow::websocket::connection* Player::getConnection() {

    return this->_connection;

}


bool Player::setName( std::string n ) {

    std::string clean;
    for ( char c: n )
        if ( c != ',' && static_cast<unsigned char>( c ) >= 0x20 && c != 0x7f )
            clean += c;

    if ( clean.size() > 16 ) {
        size_t cut = 16;
        while ( cut > 0 && ( static_cast<unsigned char>( clean[cut] ) & 0xC0 ) == 0x80 )
            cut--;
        clean.resize( cut );
    }

    if ( clean.empty() ) return false;

    std::lock_guard<std::mutex> lock( this->_name_mutex );
    this->_name = clean;
    return true;

}


std::string Player::getName() {

    std::lock_guard<std::mutex> lock( this->_name_mutex );
    return this->_name;

}


bool Player::updateGame( std::vector<Value> request ) {

    if ( request.empty() ) return false;

    if ( request[0].number && *request[0].number == 0 ) {

        if ( request.size() >= 2 && request[1].string )
            return this->setName( *request[1].string );

        if ( request.size() >= 2 && request[1].number )
            return this->setName( std::to_string( *request[1].number ) );

        return false;

    }

    if ( this->_game == nullptr ) return false;

    request.erase(request.begin());
    return this->_game->updatePlayer( this, request );

}


bool Player::setGame( Game* g ) {

    this->_game = g;
    return this->_game->addPlayer( this );

}


bool Player::exitGame() {

    if ( this->_game == nullptr ) return false;
    return this->_game->removePlayer( this );

}
