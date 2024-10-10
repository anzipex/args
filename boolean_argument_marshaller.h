#pragma once

class BooleanArgumentMarshaller : public ArgumentMarshaller {
public:
    virtual bool set(std::vector<std::string>::iterator) override {
        value_ = true;
        return true;
    };

    static bool getValue(const ArgumentMarshaller &am) {
        const BooleanArgumentMarshaller &bm = dynamic_cast<const BooleanArgumentMarshaller &>(am);
        return bm.value_;
    };

private:
    bool value_ = false;
};
