#pragma once

#include <stdexcept>

#ifndef OPAQUE_DATA_TYPE
// OPAQUE_DATA_TYPE should be defined by who's including this file before including it, and undefined after it.
static_assert(false)
#endif

#define ENT_METHOD\
  if(opaque == nullptr) {\
    throw std::runtime_error("method received an invalid state (null opaque)");\
  }\
  OPAQUE_DATA_TYPE& self = *(OPAQUE_DATA_TYPE*)opaque;

#define ENT_DTOR\
  if(opaque == nullptr) {\
    printf("error: method received an invalid state (null opaque)");\
    return;\
  }\
  OPAQUE_DATA_TYPE& self = *(OPAQUE_DATA_TYPE*)opaque;