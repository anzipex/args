#include "args.h"

Args::Args(const std::string &schema, int argc, char **argv) :
argChar_(),
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
        args_.emplace_back(argv[i]);
    }
}

std::vector<std::string> Args::split(const std::string &schema, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(schema);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void Args::parseSchema(const std::string &schema) {
    std::vector<std::string> vecSchema = split(schema, ' ');
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
                  << " is invalid schema argument\n";
    }
}

void Args::validateSchemaElementId(char elementId) {
    if (isalpha(elementId) == 0) {
        std::cerr << "'" << elementId << "'"
                  << " is invalid schema argument\n";
    }
}

bool Args::isKey(std::string arg) {
    return (arg.size() == 2) && (arg[0] == '-') && (isalpha(arg[1]) != 0);
}

void Args::parseArgStrings() {
    for (curArg_ = std::begin(args_); curArg_ != std::end(args_); ++curArg_) {
        if (isKey(*curArg_)) {
            argChar_ = (*curArg_).at(1);
            if (nextNotKey()) {
                (void)*(++curArg_);
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
        if (it->first == (*curArg_).at(1) && it->second.empty()) {
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
        return checkRequiredArgsKeys();
    }
    return true;
}

void Args::checkValidity() {
    if (!validity_.empty()) {
        for (std::vector<bool>::size_type i = 0; i < validity_.size(); ++i) {
            if (!validity_[i]) {
                break;
            }
        }
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
                      << " is required\n";
            return false;
        }
    }
    return true;
}

bool Args::checkRequiredKeysValue(const std::string &schemaKey) {
    for (auto arg = std::begin(args_); arg != std::end(args_); ++arg) {
        if (*arg == schemaKey) {
            if (std::next(arg) != std::end(args_) && !isKey(*(std::next(arg)))) {
                return true;
            }
            std::cerr << "'" << schemaKey << "'"
                      << " is required value\n";
            return false;
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
