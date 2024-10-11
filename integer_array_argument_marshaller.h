#pragma once

class IntegerArrayArgumentMarshaller : public ArgumentMarshaller {
public:
    ~IntegerArrayArgumentMarshaller() override = default;
    bool set(std::vector<std::string>::iterator currentArgument) override {
        std::string arg = *(currentArgument--);
        std::replace(arg.begin(), arg.end(), ',', ' ');
        std::stringstream iss(arg);
        std::string parameter;
        while (iss >> parameter) {
            if (!parameter.empty() && isValid(parameter)) {
                value_.push_back(std::stoi(parameter));
            } else {
                std::cerr << "'" << *(currentArgument) << "'"
                          << " has "
                          << "'" << parameter << "'"
                          << ", expected integer value\n";
                return false;
            }
        }
        return true;
    };

    static std::vector<int> getValue(const ArgumentMarshaller &am) {
        const IntegerArrayArgumentMarshaller &iaa =
                dynamic_cast<const IntegerArrayArgumentMarshaller &>(am);
        return iaa.value_;
    }

private:
    std::vector<int> value_;

    static bool isValid(std::string parameter) {
        if (parameter[0] == '-') {
            parameter.erase(0, 1);
        }

        for (char &c : parameter) {
            if (c == '-') {
                return false;
            }
            if ((isdigit(c) == 0) || (isalpha(c) != 0)) {
                return false;
            };
        }
        return true;
    };
};
