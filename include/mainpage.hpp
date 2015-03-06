/**
 *  @file mainpage.hpp
 *  @version 1.0
 *  @date 3/6/2015
 *  @author <a href="https://github.com/Ragora">Robert MacGregor</a>
 *  @brief Main page file for use with Doxygen. This file does absolutely nothing otherwise.
 *
 *  @mainpage Welcome
 *  EasyDelegate is, as the name implies, a <a href="http://msdn.microsoft.com/en-us/library/ms173171.aspx">delegate system.</a>
 *  It is written for C++11 with the intention to allow C++ programmers to easily create references to static and member methods
 *  that may be passed around like any other variable and invoked at any point in the code. This is helpful for a variety of reasons,
 *  such as in an <a href="http://en.wikipedia.org/wiki/Sink_(computing)">event/listener implementation</a>.
 *
 *  @section Example
 *  This quick example below demonstrates how simple it is to utilize the EasyDelegate library, though for a more complete example
 *  please refer to ex_delegateset.cpp.
 *  @code
 *  class MyCustomClass
 *  {
 *      public:
 *          unsigned int myMemberMethod(char *str, float flt, double dbl)
 *          {
 *              std::cout << "MyCustomClass::myMemberMethod: " << str << "," << flt << "," << dbl << std::endl;
 *              return 2;
 *          }
 *  };
 *
 *  int main(int argc, char *argv[])
 *  {
 *      // Typedef our event type for later shortcutting
 *      typedef EasyDelegate::DelegateSet<unsigned int, char*, float, double> MyEventType;
 *
 *      // Instantiate our delegate set and the custom class type.
 *      MyEventType myDelegateSet;
 *      MyCustomClass *myCustomClassInstance = new MyCustomClass();
 *
 *      // Instantiate our member Delegate; we obviously want it to persist even when out of scope in a production environment so we use a pointer.
 *      // Notice how the class type is only cared about here rather than in the DelegateSet typedef or in the push_back call below. This is because both
 *      // StaticDelegate and MemberDelegate types may be stored in the same DelegateSet as they have the same signature, the only difference is that
 *      // MemberDelegate types need a this pointer to call against.
 *      MyEventType::MemberDelegateType<MyCustomClass> *myMemberDelegate = new MyEventType::MemberDelegateType<MyCustomClass>(myCustomClassInstance, &MyCustomClass::myMemberMethod);
 *
 *      // Register the the myMemberMethod of MyCustomClass
 *      myDelegateSet.push_back(myMemberDelegate);
 *
 *      // You can invoke the DelegateSet in the same way you invoke the delegate directly
 *      myDelegateSet.invoke("Foo", 3.14, 3.14159);
 *      myMemberDelegate->invoke("Foo", 3.14, 3.14159);
 *
 *      delete myMemberDelegate;
 *      delete myCustomClassInstance; // Note that this causes any related MemberDelegate instances to become 'bad'.
 *  }
 *  @endcode
 *
 *  The above code sample will output:
 *
 *  "MyCustomClass::myMemberMethod: Foo,3.14,3.14159"
 *
 *  "MyCustomClass::myMemberMethod: Foo,3.14,3.14159"
 *
 *  @section Support Supported Compilers and Operating Systems
 *  EasyDelegate has been compiled and known to run on the following systems:
 *  <ul>
 *      <li><a href="https://gcc.gnu.org/">GCC 4.8.2</a> on LUbuntu 14.04 AMD64</li>
 *      <li><a href="http://www.microsoft.com/en-us/download/details.aspx?id=34673">Microsoft Visual Studio 2012 Express</a> on Windows 7 x64</li>
 *  </ul>
 *
 *  It should compile and run underneath of any compiler that supports <a href="http://en.wikipedia.org/wiki/Variadic_template">variadic templates</a>
 *  or has full C++11 support, though.
 *
 *  @section Limits Limitations
 *  Due to the nature of the library, there is one limitation which derives from the usage of templates which is the inability to work with <a href="http://en.cppreference.com/w/cpp/utility/variadic">variadic methods</a>
 *  because the ellipses ('...') involved are interpreted as a variadic template argument expansion and thusly will fail to compile. A work around for this if such
 *  functionality is necessary to your project would be to use a void* declaration instead and use that to pass a struct in which is then casted to the proper struct
 *  type on the called function's end. There may be a way to properly implement variadic method support, but I have not come up with anything yet.
 *
 *  @section Links Links
 *  <ul>
 *      <li><a href="https://github.com/Ragora">Author: Robert MacGregor</a></li>
 *      <li><a href="https://github.com/Ragora/EasyDelegate">EasyDelegate on GitHub</a></li>
 *  </ul>
 */
