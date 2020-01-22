/** 
 * @file IntegerArrayArgumentMarshaler.h
 * @brief IntegerArrayArgumentMarshaler class description
 * @author a.akulin
 * @date October 2, 2019
 */

#ifndef INTEGERARRAYARGUMENTMARSHALER_H
#define INTEGERARRAYARGUMENTMARSHALER_H

class IntegerArrayArgumentMarshaler : public ArgumentMarshaler {
public:

    virtual bool set(std::vector<std::string>::iterator currentArgument) override {
        std::string arg = *(currentArgument--);
        std::replace(arg.begin(), arg.end(), ',', ' ');
        std::stringstream iss(arg);
        std::string parameter = "";
        while (iss >> parameter) {
            if (!parameter.empty() && isValid(parameter)) {
                _intArrayValue.push_back(std::stoi(parameter));
            } else {
                std::cerr << "'" << *(currentArgument) << "'" << " has " <<
                    "'" << parameter << "'" << ", expected integer value" << std::endl;
                return false;
            }
        }
        return true;
    };

    static std::vector<int> getValue(ArgumentMarshaler &am) {
        const IntegerArrayArgumentMarshaler &iaa = dynamic_cast<const IntegerArrayArgumentMarshaler &>(am);
        return iaa._intArrayValue;
    }

private:
    std::vector<int> _intArrayValue;

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

#endif /* INTEGERARRAYARGUMENTMARSHALER_H */
