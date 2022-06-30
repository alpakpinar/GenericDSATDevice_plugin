#ifndef __EXCEPTIONS_HH__
#define __EXCEPTIONS_HH__

#include "BUException/ExceptionBase.hh"

namespace BUException{
    ExceptionClassGenerator(INVALID_ADDRESS,"Address is out of range of the array\n")
}

#endif