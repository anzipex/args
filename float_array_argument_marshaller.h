#pragma once

class FloatArrayArgumentMarshaller : public ArgumentMarshaller {
public:
    ~FloatArrayArgumentMarshaller() override = default;
    bool set(std::vector<std::string>::iterator currentArgument) override {
        std::string arg = *(currentArgument--);
        std::replace(arg.begin(), arg.end(), ',', ' ');
        std::stringstream iss(arg);
        std::string parameter;
        while (iss >> parameter) {
            if (!parameter.empty() && isValid(parameter)) {
                value_.push_back(std::stof(parameter));
            } else {
                std::cerr << "'" << *(currentArgument) << "'"
                          << " has "
                          << "'" << parameter << "'"
                          << ", expected float value\n";
                return false;
            }
        }
        return true;
    };

    static std::vector<float> getValue(const ArgumentMarshaller &am) {
        const FloatArrayArgumentMarshaller &faa =
                dynamic_cast<const FloatArrayArgumentMarshaller &>(am);
        return faa.value_;
    }

private:
    std::vector<float> value_;

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
