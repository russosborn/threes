#pragma once

#include <cassert>
#include <iostream>

#include <memory>
#include <functional>
#include <map>

#include <vector>
#include <string>
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


  /* Example usage for some Base with Derived1/Derived2 derived classes:
   * // instantiate factory
   * using BaseFactory = ro::ObjectFromStrFactory<Base>;
   * static BaseFactory s_BaseFactory;
   * using BasePtr = BaseFactory::ObjectPtr;
   *
   * // or whatever factory methods
   * BasePtr createDerived1(const std::string& cfg) { return(BasePtr(new Derived1(cfg))); }
   * BasePtr createDerived2(const std::string& cfg) { return(BasePtr(new Derived2(cfg))); }
   *
   * s_BaseFactory.registerCreator("derived1", createDerived1);
   * s_BaseFactory.registerCreator("derived2", createDerived2);
   */


  std::vector<std::string> strsplit(const std::string& str, const std::string& delim);
  
} // ns ro
