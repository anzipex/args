/** 
 * @file ArgumentMarshaler.h
 * @brief Описание интерфейса ArgumentMarshaler
 * @author a.akulin
 * @date September 26, 2019
 */

#ifndef ARGUMENTMARSHALER_H
#define ARGUMENTMARSHALER_H

class ArgumentMarshaler {
public:
    virtual bool set(std::vector<std::string>::iterator currentArgument) = 0;
};

#endif /* ARGUMENTMARSHALER_H */