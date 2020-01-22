/** 
 * @file FloatArgumentMarshaler.h
 * @brief FloatArgumentMarshaler class description
 * @author a.akulin
 * @date September 26, 2019
 */

#ifndef FLOATARGUMENTMARSHALER_H
#define FLOATARGUMENTMARSHALER_H

class FloatArgumentMarshaler : public ArgumentMarshaler {
public:

    virtual bool set(std::vector<std::string>::iterator currentArgument) override {
        std::string parameter = *(currentArgument--);
        if (!parameter.empty() && isValid(parameter)) {
            _floatValue = std::stof(parameter);
            return true;
        } else {
            std::cerr << "'" << *(currentArgument) << "'" << " has " <<
                "'" << parameter << "'" << ", expected float value" << std::endl;
            return false;
        }
    };

    static float getValue(const ArgumentMarshaler & am) {
        const FloatArgumentMarshaler &fa = dynamic_cast<const FloatArgumentMarshaler &>(am);
        return fa._floatValue;
    }

private:
    float _floatValue = 0;

    static bool isValid(std::string parameter) {
        if (parameter[0] == '-') {
            parameter.erase(0, 1);
        }

        for (char& c : parameter) {
            if (c == '-') {
                return false;
            } else if (!isdigit(c) || isalpha(c)) {
                return false;
            };
        }
        return true;
    };
};

#endif /* FLOATARGUMENTMARSHALER_H */
