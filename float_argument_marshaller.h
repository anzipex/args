#pragma once

class FloatArgumentMarshaller : public ArgumentMarshaller {
public:
    ~FloatArgumentMarshaller() override = default;
    bool set(std::vector<std::string>::iterator currentArgument) override {
        std::string parameter = *(currentArgument--);
        if (!parameter.empty() && isValid(parameter)) {
            value_ = std::stof(parameter);
            return true;
        }
        std::cerr << "'" << *(currentArgument) << "'"
                  << " has "
                  << "'" << parameter << "'"
                  << ", expected float value\n";
        return false;
    };

    static float getValue(const ArgumentMarshaller &am) {
        const FloatArgumentMarshaller &fa = dynamic_cast<const FloatArgumentMarshaller &>(am);
        return fa.value_;
    }

private:
    float value_ = 0;

    static bool isValid(std::string parameter) {
        if (parameter[0] == '-') {
            parameter.erase(0, 1);
        }

        bool dotEncountered = false;

        for (char &c : parameter) {
            if (c == '.') {
                if (dotEncountered) {
                    return false;
                }
                dotEncountered = true;
            } else if (isdigit(c) == 0) {
                return false;
            }
        }

        return !parameter.empty() && parameter != ".";
    };
};
