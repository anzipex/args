#pragma once

class StringArrayArgumentMarshaller : public ArgumentMarshaller {
public:
    ~StringArrayArgumentMarshaller() override = default;
    bool set(std::vector<std::string>::iterator currentArgument) override {
        std::string arg = *(currentArgument--);
        std::replace(arg.begin(), arg.end(), ',', ' ');
        std::stringstream iss(arg);
        std::string parameter;
        while (iss >> parameter) {
            value_.push_back(parameter);
        }
        return true;
    };

    static std::vector<std::string> getValue(const ArgumentMarshaller &am) {
        const StringArrayArgumentMarshaller &saa =
                dynamic_cast<const StringArrayArgumentMarshaller &>(am);
        return saa.value_;
    }

private:
    std::vector<std::string> value_;
};
