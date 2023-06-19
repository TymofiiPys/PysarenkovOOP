#include "Converter.h"

template<>
Converter<std::string>::operator int() {
    return std::stoi(key_);
}

template<>
Converter<std::string>::operator double() {
    return std::stod(key_);
}

template<>
Converter<std::string>::operator std::string() {
    return key_;
}
