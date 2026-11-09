
#include "Network.hpp"
#include "Player.hpp"


Network::Network( Config* conf, MainMenu* main ) {

    std::string port = conf->get( "port" );
    if ( port == "" ) {
        logger( 2, "Selecting default port 8080 as no port has been specified in configuration." );
        this->_port = 8080;
    } else
        this->_port = std::stoi( port );

    this->_game = main;

}


int Network::getPlayerIndexByConnection( crow::websocket::connection* c ) {

    for ( int i = 0; i < this->_players.size(); i++ ) {

        if ( this->_players[i]->getConnection() == c )
            return i;

    }

    return -1;

}


std::vector<Value> Network::parseRequest( const std::string& data ) {

    std::vector<Value> request;
    size_t start = 0;

    while ( true ) {

        size_t pos = data.find( ',', start );
        std::string token = data.substr( start, pos == std::string::npos ? std::string::npos : pos - start );

        bool is_number = !token.empty() && token.length() <= 3 &&
            token.find_first_not_of( "0123456789" ) == std::string::npos &&
            std::stoi( token ) <= 255;

        Value v;
        if ( is_number )
            v.number = static_cast<uint8_t>( std::stoi( token ) );
        else
            v.string = token;
        request.push_back( v );

        if ( pos == std::string::npos ) break;
        start = pos + 1;

    }

    return request;

}


bool Network::init() {

    CROW_ROUTE( this->_app, "/" )
        ([&] {

            char name[256];
            gethostname( name, 256 );
            crow::mustache::context x;
            x["servername"] = name;
            x["port"] = this->_port;

            auto page = crow::mustache::load( "index.html" );
            return page.render( x );

        });

    CROW_WEBSOCKET_ROUTE( this->_app, "/ws" )
        .onopen([&]( crow::websocket::connection& conn ) {

            CROW_LOG_INFO << "new websocket connection from " << conn.get_remote_ip();
            std::lock_guard<std::mutex> lock( this->_players_mutex );
            Player* new_p = new Player( &conn );
            new_p->setName( "Player " + std::to_string( this->_next_player_number++ ) );
            this->_players.push_back( new_p );

            conn.send_text( "0," + new_p->getName() );
            new_p->setGame( this->_game );
            for ( const std::string& m: stateMessages() )
                conn.send_text( m );

        })
        .onclose([&]( crow::websocket::connection& conn, const std::string& reason, uint16_t ) {

            CROW_LOG_INFO << "websocket connection closed: " << reason;
            std::lock_guard<std::mutex> lock( this->_players_mutex );
            int p = getPlayerIndexByConnection( &conn );
            if ( p == -1 ) return;

            Player* player = this->_players[p];
            this->_players.erase( this->_players.begin() + p );
            player->exitGame();
            delete player;

        })
        .onmessage([&]( crow::websocket::connection& conn, const std::string& data, bool is_binary ) {

            std::vector<Value> request = parseRequest( data );

            std::lock_guard<std::mutex> lock( this->_players_mutex );
            int p = getPlayerIndexByConnection( &conn );
            if ( p == -1 ) return;
            this->_players[p]->updateGame( request );

            if ( request[0].number && *request[0].number == 0 )
                conn.send_text( "0," + this->_players[p]->getName() );

        });

    return true;
}


std::vector<std::string> Network::stateMessages() {

    std::string controller = "1," + std::to_string( this->_game->getCurrentGame() ) +
        "," + std::to_string( this->_game->getCurrentControllerType() );

    std::string games = "2";
    for ( const std::string& name: this->_game->getGameNames() )
        games += "," + name;

    std::string votes = "3";
    for ( int count: this->_game->getVoteCounts() )
        votes += "," + std::to_string( count );

    return { games, votes, controller };

}


bool Network::sync() {

    uint32_t version = this->_game->getStateVersion();
    if ( version == this->_sent_version ) return true;

    this->_sent_version = version;
    std::vector<std::string> messages = stateMessages();

    std::lock_guard<std::mutex> lock( this->_players_mutex );
    for ( Player* p: this->_players )
        for ( const std::string& m: messages )
            p->getConnection()->send_text( m );

    return true;

}


bool Network::run() {

    static reLogger crow_logger;
    crow::logger::setHandler( &crow_logger );
    this->_crow_thread = this->_app.port( this->_port ).run_async();
    this->_app.signal_clear();
    return true;

}


bool Network::stop() {

    this->_app.stop();
    return true;

}


Network::~Network() {

    this->stop();
    if (this->_crow_thread.valid())
        this->_crow_thread.get();

    for ( Player* p: this->_players ) {
        p->exitGame();
        delete p;
    }
    this->_players.clear();

}
