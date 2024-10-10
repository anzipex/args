#pragma once

class ArgumentMarshaller {
public:
    virtual bool set(std::vector<std::string>::iterator currentArgument) = 0;
};
