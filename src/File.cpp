// -*- tab-width: 4 -*-
/*!
   @file File.cpp

   Copyright (c) 2015 pegacorn
*/
#include "clang-cpp/File.hpp"

#include <memory>
#include <sstream>
#include <string>
#include <time.h>
#include <utility>
#include "clang-c/Index.h"
#include "clang-cpp/Exception.hpp"
#include "clang-cpp/TranslationUnit.hpp"
#include "clang-cpp/UniqueCXObject.hpp"


namespace clangxx {

File File::from_name(std::shared_ptr<const TranslationUnit> translation_unit,
					 const std::string &file_name)
{
	CXFile cx_file(clang_getFile(translation_unit->native_handle(), file_name.c_str()));
	if ( !cx_file ) {
		CLANGXX_THROW_LogicError("The file was not a part of this translation unit.");
	}

	return File(std::move(cx_file), translation_unit);
}

File::File(CXFile &&cx_file, std::shared_ptr<const TranslationUnit> &translation_unit) noexcept
	: m_translation_unit(translation_unit)
	, m_cx_file(std::move(cx_file))
{}

File::~File() = default;

File::File(const File &/*other*/) = default;
File::File(File &&/*other*/) noexcept = default;

File &File::operator=(const File &/*other*/) = default;
File &File::operator=(File &&/*other*/) noexcept = default;

std::string File::name() const
{
	UniqueCXString cx_string(clang_getFileName(m_cx_file));
	if ( !cx_string ) {
		CLANGXX_THROW_LogicError("Error retrieving the complete file and path name of the given file.");
	}

	return clang_getCString(cx_string.get());
}

time_t File::time() const
{
	return clang_getFileTime(m_cx_file);
}

std::string File::repr() const
{
	std::ostringstream ostream;
	ostream << "<File: " << name() << ">";
	return ostream.str();
}

} // namespace clangxx
