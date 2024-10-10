#pragma once

class FloatArgumentMarshaller : public ArgumentMarshaller {
public:
    virtual bool set(std::vector<std::string>::iterator currentArgument) override {
        std::string parameter = *(currentArgument--);
        if (!parameter.empty() && isValid(parameter)) {
            floatValue_ = std::stof(parameter);
            return true;
        } else {
            std::cerr << "'" << *(currentArgument) << "'"
                      << " has "
                      << "'" << parameter << "'"
                      << ", expected float value" << std::endl;
            return false;
        }
    };

    static float getValue(const ArgumentMarshaller &am) {
        const FloatArgumentMarshaller &fa = dynamic_cast<const FloatArgumentMarshaller &>(am);
        return fa.floatValue_;
    }

private:
    float floatValue_ = 0;

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
