/*
 * MIT License
 * Copyright (c) 2021 Josu Saez Ondozabal
 * contact me: saezondev@gmail.com
 * github: https://github.com/saezon/jsolibrary
 */
#pragma once

#include <vector>
#include <stdint.h>

 /*
  * Simple single-header library
  * 1. Include the file
  *    #include "Events.h"
  * 2. Declare a delegate type with the parameter number and type you want
  *    DECLARE_DELEGATE_NO_PARAM(FNoParamDelegate);
  *    DECLARE_DELEGATE_ONE_PARAM(FOneParamDelegate, TYPE);
  *    DECLARE_DELEGATE_TWO_PARAM(FTwoParamDelegate, TYPE1, TYPE2);
  *    DECLARE_DELEGATE_THREE_PARAM(FThreeParamDelegate, TYPE1, TYPE2, TYPE3);
  * 3. Instantiate a delegate
  *    FOneParamDelegate delegate;
  * 4. Add listeners to delegate
  *    void Callback(int value)
  *    {
  *      cout << "Callback: " << value << endl;
  *    }
  *    class Foo
  *    {
  *    public:
  *      void callback(int value)
  *      {
  *        cout << value << endl;
  *      }
  *    };
  *
  *    int main() {
  *      FOneParam delegate;
  *      Foo foo;
  *      delegate.addListener<Foo, &Foo::callback>(&foo);
  *      delegate.addListener<&Callback>();
  *    }
  * 5. Broadcast the event
  *    delegate.broadcast(0);
  * 6. Output
  *    0
  *    Callback: 0
  * 7. Remove bindings
  *    1. Remove all bindings of the event
  *      delegate.removeAll();
  *    2. Remove a specific listener binding from delegate
  *      delegate.removeListener<&Callback>();
  *      delegate.removeListener<Foo, &Foo::callback>(&foo);
  */

namespace jso {
  using namespace std;
  using InstancePointer = void*;
#define EVENT_CONSTRUCTORS_IMPLEMENTATION() \
  Event() {} \
  Event(const uint16_t reserve_size) { _callbacks.reserve(reserve_size); } \
  ~Event() {}
#define EVENT_REMOVE_ALL_IMPLEMENTATION() _callbacks.clear();
#define EVENT_ADD_LISTENER_FUNCTION_IMPLEMENTATION() \
   for (const auto& callback : _callbacks) { \
     if (callback->first == nullptr && callback->second == &FunctionCallback<Function>)\
       return; }\
   auto ptr = std::make_unique<Callback>(nullptr, &FunctionCallback<Function>); \
   _callbacks.push_back(move(ptr));
#define EVENT_ADD_LISTENER_CLASS_METHOD_IMPLEMENTATION() \
  for (const auto& callback : _callbacks) { \
    if (callback->first == instance && \
      callback->second == &ClassMethodCallback<C, Function>) \
      return;} \
  auto ptr = make_unique<Callback>(instance, &ClassMethodCallback<C, Function>); \
  _callbacks.push_back(move(ptr));
#define EVENT_BROADCAST_IMPLEMENTATION(...) \
  for (const auto& callback : _callbacks) \
  callback->second(callback->first, __VA_ARGS__);
#define EVENT_REMOVE_LISTENER_FUNCTION_IMPLEMENTATION() \
  uint16_t i; \
  bool found = false; \
  for (i = 0; i < _callbacks.size(); i++) \
    if (_callbacks[i]->first == nullptr && _callbacks[i]->second == &FunctionCallback<Function>) { \
      found = true;\
      break; }\
  if (found) _callbacks.erase(_callbacks.begin() + i);
#define EVENT_REMOVE_LISTENER_CLASS_METHOD_IMPLEMENTATION() \
  uint16_t i; \
  bool found = false; \
  for (i = 0; i < _callbacks.size(); i++) \
    if (_callbacks[i]->first == instance && _callbacks[i]->second == &ClassMethodCallback<C, Function>) { \
      found = true;\
      break; }\
  if (found) _callbacks.erase(_callbacks.begin() + i);\

  //! NO PARAM
  template<typename T>
  class Event
  {
    typedef void (*InternalFunction)(InstancePointer);
    using Callback = std::pair<InstancePointer, InternalFunction>;
    template<void (*Function)()>
    static void FunctionCallback(InstancePointer)
    {
      return (Function)();
    }

    template<class C, void (C::* Function)()>
    static void ClassMethodCallback(InstancePointer instance)
    {
      return (static_cast<C*>(instance)->*Function)();
    }

  public:
    EVENT_CONSTRUCTORS_IMPLEMENTATION();
    template<void (*Function)()>
    void addListener()
    {
      EVENT_ADD_LISTENER_FUNCTION_IMPLEMENTATION();
    }
    template<class C, void (C::* Function)()>
    void addListener(C* instance)
    {
      EVENT_ADD_LISTENER_CLASS_METHOD_IMPLEMENTATION();
    }
    void broadcast() const
    {
      EVENT_BROADCAST_IMPLEMENTATION();
    }
    void removeAll()
    {
      EVENT_REMOVE_ALL_IMPLEMENTATION();
    }
    template<void (*Function)()>
    void removeListener()
    {
      EVENT_REMOVE_LISTENER_FUNCTION_IMPLEMENTATION();
    }
    template<class C, void (C::* Function)()>
    void removeListener(C* instance)
    {
      EVENT_REMOVE_LISTENER_CLASS_METHOD_IMPLEMENTATION();
    }

  private:
    using CallbackArray = vector<unique_ptr<Callback>>;
    CallbackArray _callbacks;
  };

  //! ONE PARAM
  template<typename R, typename ARG0>
  class Event<R(ARG0)>
  {
    typedef void (*InternalFunction)(InstancePointer, ARG0);
    using Callback = std::pair<InstancePointer, InternalFunction>;
    template<void (*Function)(ARG0)>
    static void FunctionCallback(InstancePointer, ARG0 arg0)
    {
      return (Function)(arg0);
    }

    template<class C, void (C::* Function)(ARG0)>
    static void ClassMethodCallback(InstancePointer instance, ARG0 arg0)
    {
      return (static_cast<C*>(instance)->*Function)(arg0);
    }

  public:
    EVENT_CONSTRUCTORS_IMPLEMENTATION();
    template<void (*Function)(ARG0)>
    void addListener()
    {
      EVENT_ADD_LISTENER_FUNCTION_IMPLEMENTATION();
    }
    template<class C, void (C::* Function)(ARG0)>
    void addListener(C* instance)
    {
      EVENT_ADD_LISTENER_CLASS_METHOD_IMPLEMENTATION();
    }
    void broadcast(ARG0 arg0) const
    {
      EVENT_BROADCAST_IMPLEMENTATION(arg0);
    }
    void removeAll()
    {
      EVENT_REMOVE_ALL_IMPLEMENTATION();
    }
    template<void (*Function)(ARG0)>
    void removeListener()
    {
      EVENT_REMOVE_LISTENER_FUNCTION_IMPLEMENTATION();
    }
    template<class C, void (C::* Function)(ARG0)>
    void removeListener(C* instance)
    {
      EVENT_REMOVE_LISTENER_CLASS_METHOD_IMPLEMENTATION();
    }

  private:
    using CallbackArray = vector<unique_ptr<Callback>>;
    CallbackArray _callbacks;
  };

  //! TWO PARAM
  template<typename R, typename ARG0, typename ARG1>
  class Event<R(ARG0, ARG1)>
  {
    typedef void (*InternalFunction)(InstancePointer, ARG0, ARG1);
    using Callback = std::pair<InstancePointer, InternalFunction>;
    template<void (*Function)(ARG0, ARG1)>
    static void FunctionCallback(InstancePointer, ARG0 arg0, ARG1 arg1)
    {
      return (Function)(arg0, arg1);
    }

    template<class C, void (C::* Function)(ARG0, ARG1)>
    static void ClassMethodCallback(InstancePointer instance, ARG0 arg0, ARG1 arg1)
    {
      return (static_cast<C*>(instance)->*Function)(arg0, arg1);
    }

  public:
    EVENT_CONSTRUCTORS_IMPLEMENTATION();
    template<void (*Function)(ARG0, ARG1)>
    void addListener()
    {
      EVENT_ADD_LISTENER_FUNCTION_IMPLEMENTATION();
    }
    template<class C, void (C::* Function)(ARG0, ARG1)>
    void addListener(C* instance)
    {
      EVENT_ADD_LISTENER_CLASS_METHOD_IMPLEMENTATION();
    }
    void broadcast(ARG0 arg0, ARG1 arg1) const
    {
      EVENT_BROADCAST_IMPLEMENTATION(arg0, arg1);
    }
    void removeAll()
    {
      EVENT_REMOVE_ALL_IMPLEMENTATION();
    }
    template<void (*Function)(ARG0, ARG1)>
    void removeListener()
    {
      EVENT_REMOVE_LISTENER_FUNCTION_IMPLEMENTATION();
    }
    template<class C, void (C::* Function)(ARG0, ARG1)>
    void removeListener(C* instance)
    {
      EVENT_REMOVE_LISTENER_CLASS_METHOD_IMPLEMENTATION();
    }

  private:
    using CallbackArray = vector<unique_ptr<Callback>>;
    CallbackArray _callbacks;
  };

  //! THREE PARAM
  template<typename R, typename ARG0, typename ARG1, typename ARG2>
  class Event<R(ARG0, ARG1, ARG2)>
  {
    typedef void (*InternalFunction)(InstancePointer, ARG0, ARG1, ARG2);
    using Callback = std::pair<InstancePointer, InternalFunction>;
    template<void (*Function)(ARG0, ARG1, ARG2)>
    static void FunctionCallback(InstancePointer, ARG0 arg0, ARG1 arg1, ARG2 arg2)
    {
      return (Function)(arg0, arg1, arg2);
    }

    template<class C, void (C::* Function)(ARG0, ARG1)>
    static void ClassMethodCallback(InstancePointer instance, ARG0 arg0, ARG1 arg1, ARG2 arg2)
    {
      return (static_cast<C*>(instance)->*Function)(arg0, arg1, arg2);
    }

  public:
    EVENT_CONSTRUCTORS_IMPLEMENTATION();
    template<void (*Function)(ARG0, ARG1, ARG2)>
    void addListener()
    {
      EVENT_ADD_LISTENER_FUNCTION_IMPLEMENTATION();
    }
    template<class C, void (C::* Function)(ARG0, ARG1, ARG2)>
    void addListener(C* instance)
    {
      EVENT_ADD_LISTENER_CLASS_METHOD_IMPLEMENTATION();
    }
    void broadcast(ARG0 arg0, ARG1 arg1, ARG2 arg2) const
    {
      EVENT_BROADCAST_IMPLEMENTATION(arg0, arg1, arg2);
    }
    void removeAll()
    {
      EVENT_REMOVE_ALL_IMPLEMENTATION();
    }
    template<void (*Function)(ARG0, ARG1, ARG2)>
    void removeListener()
    {
      EVENT_REMOVE_LISTENER_FUNCTION_IMPLEMENTATION();
    }
    template<class C, void (C::* Function)(ARG0, ARG1, ARG2)>
    void removeListener(C* instance)
    {
      EVENT_REMOVE_LISTENER_CLASS_METHOD_IMPLEMENTATION();
    }

  private:
    using CallbackArray = vector<unique_ptr<Callback>>;
    CallbackArray _callbacks;
  };
}

#define DECLARE_DELEGATE_NO_PARAM(name) using name = jso::Event<void>
#define DECLARE_DELEGATE_ONE_PARAM(name, type) using name = jso::Event<void(type)>
#define DECLARE_DELEGATE_TWO_PARAM(name, type1, type2) using name = jso::Event<void(type1, type2)>
#define DECLARE_DELEGATE_THREE_PARAM(name, type1, type2, type3) using name = jso::Event<void(type1, type2, type3)>
