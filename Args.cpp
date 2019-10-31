/** 
 * @file Args.cpp
 * @brief Реализация класса Args
 * @author a.akulin
 * @date September 11, 2019
 */

#include <iostream>
#include <algorithm>
#include "Args.h"

Args::Args(std::string schema, int argc, char** argv) :
_argChar(),
_valid(false),
_marshalers(),
_schemaArgs(),
_curArg(),
_args(),
_requiredShemaKeys(),
_requiredArgsKeys(),
_validity() {
    sequenceArgs(argc, argv);
    parseSchema(schema);
    parseArgStrings();
    if (checkRequirements()) {
        checkValidity();
    }
}

void Args::sequenceArgs(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        _args.push_back(argv[i]);
    }
}

std::vector<std::string> Args::split(const std::string& schema, char delimiter) {
    std::vector<std::string> vecSchema;
    std::stringstream ss(schema);
    std::string item;
    while (getline(ss, item, delimiter)) {
        vecSchema.push_back(item);
    }
    return vecSchema;
}

void Args::parseSchema(std::string schema) {
    std::vector<std::string> vecSchema = split(schema, ',');
    for (unsigned int i = 0; i < vecSchema.size(); ++i) {
        if (vecSchema[i].length() > 0) {
            parseSchemaElement(vecSchema[i]);
        }
    }
}

void Args::parseSchemaElement(std::string element) {
    char elementId = element.at(0);
    std::string elementTail = element.substr(1);
    validateSchemaElementId(elementId);
    _schemaArgs.insert({elementId, elementTail});

    if (elementTail.length() == 0 || elementTail == "!") {
        std::unique_ptr<BooleanArgumentMarshaler> item(new BooleanArgumentMarshaler());
        _marshalers[elementId] = std::move(item);
    } else if (elementTail == "*" || elementTail == "*!") {
        std::unique_ptr<StringArgumentMarshaler> item(new StringArgumentMarshaler());
        _marshalers[elementId] = std::move(item);
    } else if (elementTail == "#" || elementTail == "#!") {
        std::unique_ptr<IntegerArgumentMarshaler> item(new IntegerArgumentMarshaler());
        _marshalers[elementId] = std::move(item);
    } else if (elementTail == "#%" || elementTail == "#%!") {
        std::unique_ptr<FloatArgumentMarshaler> item(new FloatArgumentMarshaler());
        _marshalers[elementId] = std::move(item);
    } else if (elementTail == "##" || elementTail == "##!") {
        std::unique_ptr<DoubleArgumentMarshaler> item(new DoubleArgumentMarshaler());
        _marshalers[elementId] = std::move(item);
    } else if (elementTail == "[*]" || elementTail == "[*]!") {
        std::unique_ptr<StringArrayArgumentMarshaler> item(new StringArrayArgumentMarshaler());
        _marshalers[elementId] = std::move(item);
    } else if (elementTail == "[#]" || elementTail == "[#]!") {
        std::unique_ptr<IntegerArrayArgumentMarshaler> item(new IntegerArrayArgumentMarshaler());
        _marshalers[elementId] = std::move(item);
    } else if (elementTail == "[#%]" || elementTail == "[#%]!") {
        std::unique_ptr<FloatArrayArgumentMarshaler> item(new FloatArrayArgumentMarshaler());
        _marshalers[elementId] = std::move(item);
    } else {
        _schemaArgs.erase(elementId);
        std::cerr << "'" << elementId << elementTail << "'" << " is invalid schema argument" <<
            std::endl;
    }
}

void Args::validateSchemaElementId(char elementId) {
    if (!isalpha(elementId)) {
        std::cerr << "'" << elementId << "'" << " is invalid schema argument" << std::endl;
    }
}

bool Args::isKey(std::string arg) {
    return (arg.size() == 2) && (arg[0] == '-') && isalpha(arg[1]);
}

void Args::parseArgStrings() {
    for (_curArg = std::begin(_args); _curArg != std::end(_args); ++_curArg) {
        if (isKey(*_curArg)) {
            _argChar = (*_curArg).at(1);
            if (nextNotKey()) {
                *(++_curArg);
                setCurrentArg();
            } else if (isKey(*_curArg) && keyIsBool()) {
                setCurrentArg();
            } else {
                _validity.push_back(false);
                break;
            }
        }
    }
}

bool Args::nextNotKey() {
    return (std::next(_curArg) != std::end(_args) && !isKey(*(std::next(_curArg))));
}

bool Args::keyIsBool() {
    for (auto it = _schemaArgs.begin(); it != _schemaArgs.end(); ++it) {
        if (it->first == (*_curArg).at(1) && it->second == "") {
            return true;
        }
    }
    return false;
}

void Args::setCurrentArg() {
    _validity.push_back(_marshalers[_argChar]->set(_curArg));
}

bool Args::checkRequirements() {
    makeRequiredSchemaKeys();
    if (checkRequiredSchemaKeys()) {
        if (checkRequiredArgsKeys()) {
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }
}

void Args::checkValidity() {
    for (int i = 0; i < _validity.size(); ++i) {
        if (!_validity[i]) {
            _valid = false;
            break;
        }
        _valid = true;
    }
}

bool Args::checkRequiredArgsKeys() {
    for (auto& schemaKey : _requiredShemaKeys) {
        if (std::find(_args.begin(), _args.end(), schemaKey) != _args.end()) {
            if (!checkRequiredKeysValue(schemaKey)) {
                break;
            }
        } else {
            std::cerr << "'" << schemaKey << "'" << " is required" << std::endl;
            return false;
        }
    }
    return true;
}

bool Args::checkRequiredKeysValue(std::string schemaKey) {
    for (auto arg = std::begin(_args); arg != std::end(_args); ++arg) {
        if (*arg == schemaKey) {
            if (std::next(arg) != std::end(_args) && !isKey(*(std::next(arg)))) {
                return true;
            } else {
                std::cerr << "'" << schemaKey << "'" << " is required value" << std::endl;
                return false;
            }
        }
    }
}

void Args::makeRequiredSchemaKeys() {
    for (const auto& sa : _schemaArgs) {
        if (sa.second.back() == '!') {
            _requiredShemaKeys.push_back("-" + std::string(1, sa.first));
        }
    }
}

bool Args::checkRequiredSchemaKeys() const {
    return (!_requiredShemaKeys.empty());
}

bool Args::check(char arg) const {
    for (int i = 0; i < _args.size(); ++i) {
        if (isKey(_args[i]) && _args[i].at(1) == arg) {
            return true;
        }
    }
    return false;
}

bool Args::getBoolean(char arg) {
    return BooleanArgumentMarshaler::getValue(*_marshalers.at(arg));
}

std::string Args::getString(char arg) {
    return StringArgumentMarshaler::getValue(*_marshalers.at(arg));
}

int Args::getInt(char arg) {
    return IntegerArgumentMarshaler::getValue(*_marshalers.at(arg));
}

float Args::getFloat(char arg) {
    return FloatArgumentMarshaler::getValue(*_marshalers.at(arg));
}

double Args::getDouble(char arg) {
    return DoubleArgumentMarshaler::getValue(*_marshalers.at(arg));
}

std::vector<std::string> Args::getStringArray(char arg) {
    return StringArrayArgumentMarshaler::getValue(*_marshalers.at(arg));
}

std::vector<int> Args::getIntArray(char arg) {
    return IntegerArrayArgumentMarshaler::getValue(*_marshalers.at(arg));
}

std::vector<float> Args::getFloatArray(char arg) {
    return FloatArrayArgumentMarshaler::getValue(*_marshalers.at(arg));
}

bool Args::isValid() const {
    return _valid;
}