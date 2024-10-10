#pragma once

class IntegerArgumentMarshaler : public ArgumentMarshaler {
public:
    virtual bool set(std::vector<std::string>::iterator currentArgument) override {
        std::string parameter = *(currentArgument--);
        if (!parameter.empty() && isValid(parameter)) {
            _intValue = std::stoi(parameter);
            return true;
        } else {
            std::cerr << "'" << *(currentArgument) << "'"
                      << " has "
                      << "'" << parameter << "'"
                      << ", expected integer value" << std::endl;
            return false;
        }
    };

    static int getValue(const ArgumentMarshaler &am) {
        const IntegerArgumentMarshaler &ia = dynamic_cast<const IntegerArgumentMarshaler &>(am);
        return ia._intValue;
    }

private:
    int _intValue = 0;

    static bool isValid(std::string parameter) {
        if (parameter[0] == '-') {
            parameter.erase(0, 1);
        }

        for (char &c : parameter) {
            if (c == '-') {
                return false;
            } else if (!isdigit(c) || isalpha(c)) {
                return false;
            };
        }
        return true;
    };
};
