#pragma once

#include <cassert>

namespace ro {

  void ASSERT(bool condition, const char* message) {
    assert((condition && message));
  }
  

} // ns ro
