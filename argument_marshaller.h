#pragma once

class ArgumentMarshaller {
public:
    virtual ~ArgumentMarshaller() = default;
    virtual bool set(std::vector<std::string>::iterator currentArgument) = 0;
};
