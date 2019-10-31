/** 
 * @file DoubleArgumentMarshaler.h
 * @brief Описание класса DoubleArgumentMarshaler
 * @author a.akulin
 * @date September 26, 2019
 */

#ifndef DOUBLEARGUMENTMARSHALER_H
#define DOUBLEARGUMENTMARSHALER_H

class DoubleArgumentMarshaler : public ArgumentMarshaler {
public:

    virtual bool set(std::vector<std::string>::iterator currentArgument) override {
        std::string parameter = *(currentArgument--);
        if (!parameter.empty() && isValid(parameter)) {
            _doubleValue = std::stod(parameter);
            return true;
        } else {
            std::cerr << "'" << *(currentArgument) << "'" << " has " <<
                "'" << parameter << "'" << ", expected double value" << std::endl;
            return false;
        }
    };

    static double getValue(ArgumentMarshaler &am) {
        const DoubleArgumentMarshaler &da = dynamic_cast<const DoubleArgumentMarshaler &>(am);
        return da._doubleValue;
    }

private:
    double _doubleValue = 0;

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

#endif /* DOUBLEARGUMENTMARSHALER_H */