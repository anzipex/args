#pragma once

class StringArrayArgumentMarshaler : public ArgumentMarshaler {
public:
    virtual bool set(std::vector<std::string>::iterator currentArgument) override {
        std::string arg = *(currentArgument--);
        std::replace(arg.begin(), arg.end(), ',', ' ');
        std::stringstream iss(arg);
        std::string parameter = "";
        while (iss >> parameter) {
            _stringArrayValue.push_back(parameter);
        }
        return true;
    };

    static std::vector<std::string> getValue(ArgumentMarshaler &am) {
        const StringArrayArgumentMarshaler &saa =
                dynamic_cast<const StringArrayArgumentMarshaler &>(am);
        return saa._stringArrayValue;
    }

private:
    std::vector<std::string> _stringArrayValue;
};
