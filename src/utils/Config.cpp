#include <fstream>

#include "Config.hpp"
#include "logger.h"

int Config::parse( std::string path ) {

    std::ifstream config_file( path );
    if ( !config_file.is_open() ) {

        logger( 4, "Can't open config file" );
        return 1;

    }

    std::string line;
    std::string key;
    std::string value;
    while ( getline ( config_file, line ) ) {

        key = line.substr( 0, line.find_first_of('=') );
        value = line.substr( line.find_first_of('=') + 1 );

        this->_keys.push_back(key);
        this->_values.push_back(value);

        logger(
            0, ("Added key " + key + " with value " + value +
            " to configuration").c_str()
        );

    }

    logger( 1, "Loaded config file !" );

    return 0;

}


std::string Config::get( std::string s ) {

    for ( int i = 0; i < this->size(); ++i ) {

        if ( this->_keys[i] == s )
            return this->_values[i];

    }

    logger( 2, ("Could not find key \"" + s + "\" in configuration").c_str() );
    return "";

}


std::string Config::get( int i ) {

    if ( i >= 0 && i < this->size() )
        return this->_values[i];

    logger(
        2, ("Could not find value at index \"" + std::to_string(i) +
        "\" in configuration").c_str()
    );
    return "";

}


std::vector<std::string> Config::keys() {

    return this->_keys;

}


std::vector<std::string> Config::values() {

    return this->_values;

}


int Config::size() {

    return this->_values.size();

}
