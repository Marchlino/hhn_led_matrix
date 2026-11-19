#pragma once
#include <string>
#include <vector>

/**
 * @class Config
 * @brief Load and store configuration
 */
class Config {

    public:

        /**
         * @fn parse
         * @brief Parse a file at the given file path.
         * @param path Path of the file to parse
         */
        int parse( std::string );

        /**
         * @fn get
         * @brief Return the value given a key.
         * @param key Key linked to the value
         * @return std::string Value or empty string
         */
        std::string get( std::string );

        /**
         * @fn get
         * @brief Return the value given a indice.
         * @param i Indice of the value
         * @return std::string Value or empty string
         */
        std::string get( int );

        /**
         * @fn keys
         * @brief Return all the keys.
         * @return std::vector<std::string> Keys stored
         */
        std::vector<std::string> keys();

        /**
         * @fn keys
         * @brief Return all the values.
         * @return std::vector<std::string> Values stored
         */
        std::vector<std::string> values();

        /**
         * @fn size
         * @brief Return the number of values stored
         * @return int Number of values stored
         */
        int size();

    private:

        std::vector<std::string> _keys;
        std::vector<std::string> _values;

};
