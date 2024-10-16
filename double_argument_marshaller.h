#pragma once

class DoubleArgumentMarshaller : public ArgumentMarshaller {
public:
    ~DoubleArgumentMarshaller() override = default;
    bool set(std::vector<std::string>::iterator currentArgument) override {
        std::string parameter = *(currentArgument--);
        if (!parameter.empty() && isValid(parameter)) {
            value_ = std::stod(parameter);
            return true;
        }
        std::cerr << "'" << *(currentArgument) << "'"
                  << " has "
                  << "'" << parameter << "'"
                  << ", expected double value\n";
        return false;
    };

    static double getValue(const ArgumentMarshaller &am) {
        const DoubleArgumentMarshaller &da = dynamic_cast<const DoubleArgumentMarshaller &>(am);
        return da.value_;
    }

private:
    double value_ = 0;

    static bool isValid(std::string parameter) {
        if (parameter[0] == '-') {
            parameter.erase(0, 1);
        }

        bool dotEncountered = false;

        for (char &c : parameter) {
            if (c == '.') {
                if (dotEncountered) {
                    return false;
                }
                dotEncountered = true;
            } else if (isdigit(c) == 0) {
                return false;
            }
        }

        return !parameter.empty() && parameter != ".";
    };
};
