#pragma once

class FloatArrayArgumentMarshaller : public ArgumentMarshaller {
public:
    virtual bool set(std::vector<std::string>::iterator currentArgument) override {
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
                          << ", expected float value" << std::endl;
                return false;
            }
        }
        return true;
    };

    static std::vector<float> getValue(ArgumentMarshaller &am) {
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
