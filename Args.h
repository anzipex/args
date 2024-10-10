#pragma once

#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <sstream>

#include "ArgumentMarshaler.h"
#include "BooleanArgumentMarshaler.h"
#include "StringArgumentMarshaler.h"
#include "IntegerArgumentMarshaler.h"
#include "FloatArgumentMarshaler.h"
#include "DoubleArgumentMarshaler.h"
#include "StringArrayArgumentMarshaler.h"
#include "IntegerArrayArgumentMarshaler.h"
#include "FloatArrayArgumentMarshaler.h"

class Args final {
public:
    /**
     * @brief Takes format string parameters and a set of arguments for parsing
     * @param schema, format string.
     * 1) You must specify the key name (only one letter)
     * 2) A character of the type must be specified:
     *    no character - Boolean
     *    * - String
     *    # - Integer
     *    #% - Float
     *    ## - Double
     *    [*] - vector String
     *    [#] - vector Integer
     *    [#%] - vector Float
     * 3) ! after the type name - the key must be on the command line
     * @param argc, number of arguments
     * @param argv, arguments
     */
    Args(std::string schema, int argc, char **argv);
    ~Args() = default;

    /**
     * @brief Check for a key in the passed arguments
     * @param arg, command line key
     * @return Check result
     */
    bool check(char arg) const;

    /**
     * @brief Get the bool value by the corresponding key name
     * @param arg, format string key
     * @return Value
     */
    bool getBoolean(char arg);

    /**
     * @brief Get string value by the corresponding key name
     * @param arg, format string key
     * @return Value
     */
    std::string getString(char arg);

    /**
     * @brief Get integer value by the corresponding key name
     * @param arg, format string key
     * @return Value
     */
    int getInt(char arg);

    /**
     * @brief Get the float value by the corresponding key name
     * @param arg, format string key
     * @return Value
     */
    float getFloat(char arg);

    /**
     * @brief Get the double value by the corresponding key name
     * @param arg, format string key
     * @return Value
     */
    double getDouble(char arg);

    /**
     * @brief Get the value of the string vector by the corresponding key name
     * @param arg, format string key
     * @return Value
     */
    std::vector<std::string> getStringArray(char arg);

    /**
     * @brief Get the value of the integer vector by the corresponding key name
     * @param arg, format string key
     * @return Value
     */
    std::vector<int> getIntArray(char arg);

    /**
     * @brief Get the value of the float vector by the corresponding key name
     * @param arg, format string key
     * @return Value
     */
    std::vector<float> getFloatArray(char arg);

    /**
     * @brief Check that all parameters are parsed without errors
     * @return check result
     */
    bool isValid() const;

private:
    /**
     * @brief Gets a vector for each argument
     * @param argc, number of arguments
     * @param argv, arguments
     */
    void sequenceArgs(int argc, char **argv);

    /**
     * @brief Format string parsing
     * @param schema, format string
     */
    void parseSchema(std::string schema);

    /**
     * @brief Split the format string by the corresponding character
     * @param schema, format string
     * @param delimiter, delimiter
     * @return Keys and their meanings
     */
    static std::vector<std::string> split(const std::string &schema, char delimiter);

    /**
     * @brief Parsing each element of the format string
     * @param element
     */
    void parseSchemaElement(std::string element);

    /**
     * @brief Check if argument is key
     * @param arg
     * @return Check result
     */
    static bool isKey(std::string arg);

    /**
     * @brief Check for key name matching
     * @param elementId, key name
     */
    static void validateSchemaElementId(char elementId);

    /**
     * @brief Parsing each command line argument
     */
    void parseArgStrings();

    /**
     * @brief Check if the next argument is the last
     */
    bool nextNotKey();

    /**
     * @brief Check if argument is a bool key
     */
    bool keyIsBool();

    /**
     * @brief Set current argument
     */
    void setCurrentArg();

    /**
     * @brief Check for required keys and their values
     * @return Availability of required keys and their values
     */
    bool checkRequirements();

    /**
     * @brief Validation of all arguments
     */
    void checkValidity();

    /**
     * @brief Make a vector with required keys, if any
     */
    void makeRequiredSchemaKeys();

    /**
     * @brief Checking for required keys in a format string
     * @return The presence of at least one key
     */
    bool checkRequiredSchemaKeys() const;

    /**
     * @brief Check for the value of a required key among command line arguments
     * @param schemaKey, required format string key
     * @return The value of the key from the command line
     */
    bool checkRequiredKeysValue(std::string schemaKey);

    /**
     * @brief Check for format and command line keys
     * @return The presence of a required key on the command line
     */
    bool checkRequiredArgsKeys();

    char _argChar; //! <key name
    bool _valid; //! <validity
    std::map<char, std::unique_ptr<ArgumentMarshaler> > _marshallers; //! <key names and values
    std::map<char, std::string> _schemaArgs; //! <format string from key names and data types
    std::vector<std::string>::iterator _curArg; //! <current argument
    std::vector<std::string> _args; //! <command line arguments
    std::vector<std::string> _requiredSchemaKeys; //! <required keys in the format string
    std::vector<std::string> _requiredArgsKeys; //! <required keys on the command line
    std::vector<bool> _validity; //! <validity of all arguments
};
