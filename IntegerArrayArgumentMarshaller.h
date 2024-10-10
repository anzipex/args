#pragma once

class IntegerArrayArgumentMarshaller : public ArgumentMarshaller {
public:
    virtual bool set(std::vector<std::string>::iterator currentArgument) override {
        std::string arg = *(currentArgument--);
        std::replace(arg.begin(), arg.end(), ',', ' ');
        std::stringstream iss(arg);
        std::string parameter = "";
        while (iss >> parameter) {
            if (!parameter.empty() && isValid(parameter)) {
                intArrayValue_.push_back(std::stoi(parameter));
            } else {
                std::cerr << "'" << *(currentArgument) << "'"
                          << " has "
                          << "'" << parameter << "'"
                          << ", expected integer value" << std::endl;
                return false;
            }
        }
        return true;
    };

    static std::vector<int> getValue(ArgumentMarshaller &am) {
        const IntegerArrayArgumentMarshaller &iaa =
                dynamic_cast<const IntegerArrayArgumentMarshaller &>(am);
        return iaa.intArrayValue_;
    }

private:
    std::vector<int> intArrayValue_;

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
