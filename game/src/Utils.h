#pragma once

#include <cassert>

namespace ro {

  inline void ASSERT(bool condition, const char* message) {
    assert((message, condition));
  }
  

} // ns ro
