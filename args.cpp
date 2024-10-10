#include <iostream>
#include <algorithm>

#include "args.h"

Args::Args(std::string schema, int argc, char **argv) :
argChar_(),
valid_(false),
marshallers_(),
schemaArgs_(),
curArg_(),
args_(),
requiredSchemaKeys_(),
requiredArgsKeys_(),
validity_() {
    sequenceArgs(argc, argv);
    parseSchema(schema);
    parseArgStrings();
    if (checkRequirements()) {
        checkValidity();
    }
}

void Args::sequenceArgs(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        args_.push_back(argv[i]);
    }
}

std::vector<std::string> Args::split(const std::string &schema, char delimiter) {
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
    schemaArgs_.insert({elementId, elementTail});

    if (elementTail.length() == 0 || elementTail == "!") {
        std::unique_ptr<BooleanArgumentMarshaller> item(new BooleanArgumentMarshaller());
        marshallers_[elementId] = std::move(item);
    } else if (elementTail == "*" || elementTail == "*!") {
        std::unique_ptr<StringArgumentMarshaller> item(new StringArgumentMarshaller());
        marshallers_[elementId] = std::move(item);
    } else if (elementTail == "#" || elementTail == "#!") {
        std::unique_ptr<IntegerArgumentMarshaller> item(new IntegerArgumentMarshaller());
        marshallers_[elementId] = std::move(item);
    } else if (elementTail == "#%" || elementTail == "#%!") {
        std::unique_ptr<FloatArgumentMarshaller> item(new FloatArgumentMarshaller());
        marshallers_[elementId] = std::move(item);
    } else if (elementTail == "##" || elementTail == "##!") {
        std::unique_ptr<DoubleArgumentMarshaller> item(new DoubleArgumentMarshaller());
        marshallers_[elementId] = std::move(item);
    } else if (elementTail == "[*]" || elementTail == "[*]!") {
        std::unique_ptr<StringArrayArgumentMarshaller> item(new StringArrayArgumentMarshaller());
        marshallers_[elementId] = std::move(item);
    } else if (elementTail == "[#]" || elementTail == "[#]!") {
        std::unique_ptr<IntegerArrayArgumentMarshaller> item(new IntegerArrayArgumentMarshaller());
        marshallers_[elementId] = std::move(item);
    } else if (elementTail == "[#%]" || elementTail == "[#%]!") {
        std::unique_ptr<FloatArrayArgumentMarshaller> item(new FloatArrayArgumentMarshaller());
        marshallers_[elementId] = std::move(item);
    } else {
        schemaArgs_.erase(elementId);
        std::cerr << "'" << elementId << elementTail << "'"
                  << " is invalid schema argument" << std::endl;
    }
}

void Args::validateSchemaElementId(char elementId) {
    if (!isalpha(elementId)) {
        std::cerr << "'" << elementId << "'"
                  << " is invalid schema argument" << std::endl;
    }
}

bool Args::isKey(std::string arg) {
    return (arg.size() == 2) && (arg[0] == '-') && isalpha(arg[1]);
}

void Args::parseArgStrings() {
    for (curArg_ = std::begin(args_); curArg_ != std::end(args_); ++curArg_) {
        if (isKey(*curArg_)) {
            argChar_ = (*curArg_).at(1);
            if (nextNotKey()) {
                *(++curArg_);
                setCurrentArg();
            } else if (isKey(*curArg_) && keyIsBool()) {
                setCurrentArg();
            } else {
                validity_.push_back(false);
                break;
            }
        }
    }
}

bool Args::nextNotKey() {
    return (std::next(curArg_) != std::end(args_) && !isKey(*(std::next(curArg_))));
}

bool Args::keyIsBool() {
    for (auto it = schemaArgs_.begin(); it != schemaArgs_.end(); ++it) {
        if (it->first == (*curArg_).at(1) && it->second == "") {
            return true;
        }
    }
    return false;
}

void Args::setCurrentArg() {
    validity_.push_back(marshallers_[argChar_]->set(curArg_));
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
    if (validity_.empty()) {
        valid_ = true;
    }
    for (std::vector<bool>::size_type i = 0; i < validity_.size(); ++i) {
        if (!validity_[i]) {
            valid_ = false;
            break;
        }
        valid_ = true;
    }
}

bool Args::checkRequiredArgsKeys() {
    for (auto &schemaKey : requiredSchemaKeys_) {
        if (std::find(args_.begin(), args_.end(), schemaKey) != args_.end()) {
            if (!checkRequiredKeysValue(schemaKey)) {
                break;
            }
        } else {
            std::cerr << "'" << schemaKey << "'"
                      << " is required" << std::endl;
            return false;
        }
    }
    return true;
}

bool Args::checkRequiredKeysValue(std::string schemaKey) {
    for (auto arg = std::begin(args_); arg != std::end(args_); ++arg) {
        if (*arg == schemaKey) {
            if (std::next(arg) != std::end(args_) && !isKey(*(std::next(arg)))) {
                return true;
            } else {
                std::cerr << "'" << schemaKey << "'"
                          << " is required value" << std::endl;
                return false;
            }
        }
    }
    return false;
}

void Args::makeRequiredSchemaKeys() {
    for (const auto &sa : schemaArgs_) {
        if (sa.second.back() == '!') {
            requiredSchemaKeys_.push_back("-" + std::string(1, sa.first));
        }
    }
}

bool Args::checkRequiredSchemaKeys() const {
    return (!requiredSchemaKeys_.empty());
}

bool Args::check(char arg) const {
    for (std::vector<std::string>::size_type i = 0; i < args_.size(); ++i) {
        if (isKey(args_[i]) && args_[i].at(1) == arg) {
            return true;
        }
    }
    return false;
}

bool Args::getBoolean(char arg) {
    return BooleanArgumentMarshaller::getValue(*marshallers_.at(arg));
}

std::string Args::getString(char arg) {
    return StringArgumentMarshaller::getValue(*marshallers_.at(arg));
}

int Args::getInt(char arg) {
    return IntegerArgumentMarshaller::getValue(*marshallers_.at(arg));
}

float Args::getFloat(char arg) {
    return FloatArgumentMarshaller::getValue(*marshallers_.at(arg));
}

double Args::getDouble(char arg) {
    return DoubleArgumentMarshaller::getValue(*marshallers_.at(arg));
}

std::vector<std::string> Args::getStringArray(char arg) {
    return StringArrayArgumentMarshaller::getValue(*marshallers_.at(arg));
}

std::vector<int> Args::getIntArray(char arg) {
    return IntegerArrayArgumentMarshaller::getValue(*marshallers_.at(arg));
}

std::vector<float> Args::getFloatArray(char arg) {
    return FloatArrayArgumentMarshaller::getValue(*marshallers_.at(arg));
}

bool Args::isValid() const {
    return valid_;
}
