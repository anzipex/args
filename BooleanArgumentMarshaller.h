#pragma once

class BooleanArgumentMarshaller : public ArgumentMarshaller {
public:
    virtual bool set(std::vector<std::string>::iterator) override {
        boolValue_ = true;
        return true;
    };

    static bool getValue(const ArgumentMarshaller &am) {
        const BooleanArgumentMarshaller &bm = dynamic_cast<const BooleanArgumentMarshaller &>(am);
        return bm.boolValue_;
    };

private:
    bool boolValue_ = false;
};
