#pragma once

#include <stdexcept>

class CallSetterOfReadOnlyPropertyException : public std::logic_error {
public:
	CallSetterOfReadOnlyPropertyException() :
		std::logic_error("Attempt to set value for read only property") {}
};
