/** 
 * @file FloatArrayArgumentMarshaler.h
 * @brief FloatArrayArgumentMarshaler class description
 * @author a.akulin
 * @date October 3, 2019
 */

#ifndef FLOATARRAYARGUMENTMARSHALER_H
#define FLOATARRAYARGUMENTMARSHALER_H

class FloatArrayArgumentMarshaler : public ArgumentMarshaler {
public:

    virtual bool set(std::vector<std::string>::iterator currentArgument) override {
        std::string arg = *(currentArgument--);
        std::replace(arg.begin(), arg.end(), ',', ' ');
        std::stringstream iss(arg);
        std::string parameter = "";
        while (iss >> parameter) {
            if (!parameter.empty() && isValid(parameter)) {
                _floatArrayValue.push_back(std::stof(parameter));
            } else {
                std::cerr << "'" << *(currentArgument) << "'" << " has " <<
                    "'" << parameter << "'" << ", expected float value" << std::endl;
                return false;
            }
        }
        return true;
    };

    static std::vector<float> getValue(ArgumentMarshaler &am) {
        const FloatArrayArgumentMarshaler &faa = dynamic_cast<const FloatArrayArgumentMarshaler &>(am);
        return faa._floatArrayValue;
    }

private:
    std::vector<float> _floatArrayValue;

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

#endif /* FLOATARRAYARGUMENTMARSHALER_H */
