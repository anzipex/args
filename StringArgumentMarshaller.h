#pragma once

class StringArgumentMarshaller : public ArgumentMarshaller {
public:
    virtual bool set(std::vector<std::string>::iterator currentArgument) override {
        value_ = *(currentArgument);
        return true;
    };

    static std::string getValue(const ArgumentMarshaller &am) {
        const StringArgumentMarshaller &sm = dynamic_cast<const StringArgumentMarshaller &>(am);
        return sm.value_;
    }

private:
    std::string value_;
};
