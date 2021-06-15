#pragma once

#include <cassert>
#include <iostream>

#include <memory>
#include <functional>
#include <map>
//  inline void ASSERT(bool condition, const char* message) {
//    assert( (message, condition) );
//  }

// from StackOverflow, do/while allows semicolon after.
#ifndef NDEBUG
# define ASSERT(condition, message) \
  do { \
  if (! (condition)) { \
  std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
	    << " line " << __LINE__ << ": " << message << std::endl;	\
  std::terminate();							\
  }									\
  } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

namespace ro {

  template<typename T>
  class ObjectFromStrFactory {
  public:
    using ObjectPtr     = std::unique_ptr<T>;
    using ObjectCreator = std::function<ObjectPtr(const std::string&)>;

    static ObjectPtr create(const std::string& name, const std::string& argsStr) {
      auto itr = s_factories.find(name);
      ASSERT(itr != s_factories.end(), std::string("no factory registered for ") + name);
      const ObjectCreator& creatorFn = itr->second;
      return( ObjectPtr(creatorFn(argsStr)) );
    }

    static void registerCreator(const std::string& name, ObjectCreator creator) {
      ASSERT( s_factories.find(name) == s_factories.end(),
	      std::string("double registration ") + name );
      s_factories[name] = creator;
    }

  private:
    static std::map<std::string, ObjectCreator> s_factories;
  };

  template<typename T>
  std::map<std::string, typename ObjectFromStrFactory<T>::ObjectCreator>
  ObjectFromStrFactory<T>::s_factories;

} // ns ro
