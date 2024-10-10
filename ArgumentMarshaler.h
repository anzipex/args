#pragma once

class ArgumentMarshaler {
public:
    virtual bool set(std::vector<std::string>::iterator currentArgument) = 0;
};
