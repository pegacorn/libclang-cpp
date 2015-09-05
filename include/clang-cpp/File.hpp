// -*- tab-width: 4 -*-
/*!
   @file File.hpp

   Copyright (c) 2015 pegacorn
*/
#ifndef clang_cpp_File_hpp
#define clang_cpp_File_hpp

#include <memory>
#include <ostream>
#include <string>
#include <time.h>
#include <utility>
#include "clang-c/Index.h"
#include "clang-cpp/switch_port.hpp"


namespace clangxx {

class TranslationUnit;

class CLANGXX_API File
{
  public:
	static File from_name(std::shared_ptr<TranslationUnit> translation_unit,
						  const std::string &file_name);

//	static File from_cursor_result(Cursor cursor);

  private:
	std::shared_ptr<TranslationUnit>	m_translation_unit;
	// warning: define m_translation_unit before m_cx_file
	CXFile	m_cx_file;

  private:
	File(CXFile &&cx_file, std::shared_ptr<TranslationUnit> &translation_unit) noexcept
		: m_translation_unit(translation_unit)
		, m_cx_file(std::move(cx_file))
	{}

  public:
	~File();

	File(const File &) = delete;
	File(File &&other) noexcept = default;

	File &operator=(const File &) = delete;
	File &operator=(File &&other) noexcept = default;

  public:
	std::string name() const;

	time_t time() const;

	std::string str() const {
		return name();
	}

	std::string repr() const;
}; // class File

inline std::ostream &operator<<(std::ostream &os, const File &file)
{
	return os << file.str();
}

} //namespace clangxx


#endif // clang_cpp_File_hpp
