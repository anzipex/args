#pragma once

class IntegerArgumentMarshaller : public ArgumentMarshaller {
public:
    virtual bool set(std::vector<std::string>::iterator currentArgument) override {
        std::string parameter = *(currentArgument--);
        if (!parameter.empty() && isValid(parameter)) {
            value_ = std::stoi(parameter);
            return true;
        }
        std::cerr << "'" << *(currentArgument) << "'"
                  << " has "
                  << "'" << parameter << "'"
                  << ", expected integer value" << std::endl;
        return false;
    };

    static int getValue(const ArgumentMarshaller &am) {
        const IntegerArgumentMarshaller &ia = dynamic_cast<const IntegerArgumentMarshaller &>(am);
        return ia.value_;
    }

private:
    int value_ = 0;

    static bool isValid(std::string parameter) {
        if (parameter[0] == '-') {
            parameter.erase(0, 1);
        }

        for (char &c : parameter) {
            if (c == '-') {
                return false;
            }
            if (!isdigit(c) || isalpha(c)) {
                return false;
            };
        }
        return true;
    };
};
