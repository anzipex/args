#pragma once

class StringArgumentMarshaller : public ArgumentMarshaller {
public:
    virtual bool set(std::vector<std::string>::iterator currentArgument) override {
        stringValue_ = *(currentArgument);
        return true;
    };

    static std::string getValue(const ArgumentMarshaller &am) {
        const StringArgumentMarshaller &sm = dynamic_cast<const StringArgumentMarshaller &>(am);
        return sm.stringValue_;
    }

private:
    std::string stringValue_ = "";
};
