// -*- tab-width: 4 -*-
/*!
   @file Exception.cpp

   Copyright (c) 2015 pegacorn
*/
#include <sstream>
#include <string>
#include "clang-c/Index.h"
#include "clang-cpp/Exception.hpp"


namespace clangxx {

std::string TranslationUnitSaveError::make_what(CXSaveError kind, const std::string &what)
{
	std::ostringstream ostream;
	ostream << "Error " << kind << ": " << what;
	return ostream.str();
}

} // namespace clangxx
