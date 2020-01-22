/** 
 * @file StringArgumentMarshaler.h
 * @brief StringArgumentMarshaler class description
 * @author a.akulin
 * @date September 26, 2019
 */

#ifndef STRINGARGUMENTMARSHALER_H
#define STRINGARGUMENTMARSHALER_H

class StringArgumentMarshaler : public ArgumentMarshaler {
public:

    virtual bool set(std::vector<std::string>::iterator currentArgument) override {
        _stringValue = *(currentArgument);
        return true;
    };

    static std::string getValue(const ArgumentMarshaler &am) {
        const StringArgumentMarshaler &sm = dynamic_cast<const StringArgumentMarshaler &>(am);
        return sm._stringValue;
    }

private:
    std::string _stringValue = "";
};

#endif /* STRINGARGUMENTMARSHALER_H */
