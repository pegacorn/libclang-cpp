// -*- tab-width: 4 -*-
/*!
  @file switch_port.hpp

  Copyright (c) 2015 pegacorn
*/
#ifndef clang_cpp_switch_port_hpp
#define clang_cpp_switch_port_hpp


// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
	#define CLANGXX_HELPER_DLL_IMPORT	__declspec(dllimport)
	#define CLANGXX_HELPER_DLL_EXPORT	__declspec(dllexport)
	#define CLANGXX_HELPER_DLL_LOCAL
#else
	#if __GNUC__ >= 4
		#define CLANGXX_HELPER_DLL_IMPORT	__attribute__ ((visibility ("default")))
		#define CLANGXX_HELPER_DLL_EXPORT	__attribute__ ((visibility ("default")))
		#define CLANGXX_HELPER_DLL_LOCAL	__attribute__ ((visibility ("hidden")))
	#else
		#define CLANGXX_HELPER_DLL_IMPORT
		#define CLANGXX_HELPER_DLL_EXPORT
		#define CLANGXX_HELPER_DLL_LOCAL
	#endif
#endif

// Now we use the generic helper definitions above to define CLANGXX_API and CLANGXX_LOCAL.
// CLANGXX_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// CLANGXX_LOCAL is used for non-api symbols.

#ifdef CLANGXX_DLL	// defined if CLANGXX is compiled as a DLL
	#ifdef CLANGXX_EXPORTS	// defined if we are building the CLANGXX DLL (instead of using it)
		#define CLANGXX_API CLANGXX_HELPER_DLL_EXPORT
	#else
		#define CLANGXX_API CLANGXX_HELPER_DLL_IMPORT
	#endif
	#define CLANGXX_LOCAL CLANGXX_HELPER_DLL_LOCAL
#else				// this means CLANGXX is a static lib.
	#define CLANGXX_API
	#define CLANGXX_LOCAL
#endif


#endif // clang_cpp_switch_port_hpp
