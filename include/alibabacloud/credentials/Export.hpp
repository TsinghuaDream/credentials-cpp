#ifndef ALIBABACLOUD_CREDENTIALS_EXPORT_HPP_
#define ALIBABACLOUD_CREDENTIALS_EXPORT_HPP_

// Export/import macro for Windows DLLs and default visibility on other platforms
#if defined(_WIN32) || defined(__CYGWIN__)
  #if defined(ALIBABACLOUD_CREDENTIALS_BUILD)
    #define ALIBABACLOUD_CREDENTIALS_EXPORT __declspec(dllexport)
  #else
    #define ALIBABACLOUD_CREDENTIALS_EXPORT __declspec(dllimport)
  #endif
#else
  #if __GNUC__ >= 4
    #define ALIBABACLOUD_CREDENTIALS_EXPORT __attribute__((visibility("default")))
  #else
    #define ALIBABACLOUD_CREDENTIALS_EXPORT
  #endif
#endif

// Macros to suppress MSVC C4251 warning (STL types in exported classes)
// These warnings are safe when the DLL and client use the same compiler and runtime
#ifdef _MSC_VER
  #define ALIBABACLOUD_CREDENTIALS_SUPPRESS_STL_WARNING_PUSH \
    __pragma(warning(push)) \
    __pragma(warning(disable: 4251))
  
  #define ALIBABACLOUD_CREDENTIALS_SUPPRESS_STL_WARNING_POP \
    __pragma(warning(pop))
#else
  #define ALIBABACLOUD_CREDENTIALS_SUPPRESS_STL_WARNING_PUSH
  #define ALIBABACLOUD_CREDENTIALS_SUPPRESS_STL_WARNING_POP
#endif

#endif // ALIBABACLOUD_CREDENTIALS_EXPORT_HPP_
