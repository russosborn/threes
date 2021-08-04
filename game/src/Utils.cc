#include "Utils.h"

namespace ro {

  std::vector<std::string> strsplit(const std::string& str, const std::string& delim) {
    std::vector<std::string> result;
    size_t start=0, end=str.find(delim);

    while( end != std::string::npos ) {
      result.push_back( str.substr(start,end-start) );
      start = end + delim.length();
      end = str.find(delim, start);
    }
    result.push_back( str.substr(start,end) );

    return result;
    
  }


} // ns ro
