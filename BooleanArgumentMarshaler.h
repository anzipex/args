/** 
 * @file BooleanArgumentMarshaler.h
 * @brief BooleanArgumentMarshaler class description
 * @author a.akulin
 * @date September 26, 2019
 */

#ifndef BOOLEANARGUMENTMARSHALER_H
#define BOOLEANARGUMENTMARSHALER_H

class BooleanArgumentMarshaler : public ArgumentMarshaler {
public:

    virtual bool set(std::vector<std::string>::iterator) override {
        _boolValue = true;
        return true;
    };

    static bool getValue(const ArgumentMarshaler &am) {
        const BooleanArgumentMarshaler &bm = dynamic_cast<const BooleanArgumentMarshaler &>(am);
        return bm._boolValue;
    };

private:
    bool _boolValue = false;
};

#endif /* BOOLEANARGUMENTMARSHALER_H */
