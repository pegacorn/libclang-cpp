// -*- tab-width: 4 -*-
/*!
   @file UnsavedFile.hpp

   Copyright (c) 2015 pegacorn
*/
#ifndef clang_cpp_UnsavedFile_hpp
#define clang_cpp_UnsavedFile_hpp

#include <istream>
#include <memory>
#include <string>


namespace clangxx {

struct UnsavedFile
{
	std::string						filename;
	std::unique_ptr<std::istream>	contents;
}; // struct UnsavedFile

} // namespace clangxx


#endif // clang_cpp_UnsavedFile_hpp
