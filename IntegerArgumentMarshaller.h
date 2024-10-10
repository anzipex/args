#pragma once

class IntegerArgumentMarshaller : public ArgumentMarshaller {
public:
    virtual bool set(std::vector<std::string>::iterator currentArgument) override {
        std::string parameter = *(currentArgument--);
        if (!parameter.empty() && isValid(parameter)) {
            intValue_ = std::stoi(parameter);
            return true;
        } else {
            std::cerr << "'" << *(currentArgument) << "'"
                      << " has "
                      << "'" << parameter << "'"
                      << ", expected integer value" << std::endl;
            return false;
        }
    };

    static int getValue(const ArgumentMarshaller &am) {
        const IntegerArgumentMarshaller &ia = dynamic_cast<const IntegerArgumentMarshaller &>(am);
        return ia.intValue_;
    }

private:
    int intValue_ = 0;

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
