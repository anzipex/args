#pragma once

class DoubleArgumentMarshaller : public ArgumentMarshaller {
public:
    virtual bool set(std::vector<std::string>::iterator currentArgument) override {
        std::string parameter = *(currentArgument--);
        if (!parameter.empty() && isValid(parameter)) {
            value_ = std::stod(parameter);
            return true;
        }
        std::cerr << "'" << *(currentArgument) << "'"
                  << " has "
                  << "'" << parameter << "'"
                  << ", expected double value" << std::endl;
        return false;
    };

    static double getValue(ArgumentMarshaller &am) {
        const DoubleArgumentMarshaller &da = dynamic_cast<const DoubleArgumentMarshaller &>(am);
        return da.value_;
    }

private:
    double value_ = 0;

    static bool isValid(std::string parameter) {
        if (parameter[0] == '-') {
            parameter.erase(0, 1);
        }

        for (char &c : parameter) {
            if (c == '-') {
                return false;
            } else if (!isdigit(c) || isalpha(c)) {
                return false;
            };
        }
        return true;
    };
};
