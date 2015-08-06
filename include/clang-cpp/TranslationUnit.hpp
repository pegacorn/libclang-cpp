// -*- tab-width: 4 -*-
/*!
   @file TranslationUnit.hpp

   Copyright (c) 2015 pegacorn
*/
#ifndef clang_cpp_TranslationUnit_hpp
#define clang_cpp_TranslationUnit_hpp

#include <memory>
#include <string>
#include <vector>
#include "clang-c/Index.h"
#include "clang-cpp/switch_port.hpp"
#include "clang-cpp/UniqueCXObject.hpp"
#include "clang-cpp/UnsavedFile.hpp"


namespace clangxx {

class File;
class Index;
class SourceLocation;

class TranslationUnitImpl;

class CLANGXX_API TranslationUnit
{
	friend TranslationUnitImpl;

  public:
	static std::unique_ptr<TranslationUnit> from_source(
	  const std::string &filename, const std::vector<std::string> *args = nullptr,
	  const std::vector<UnsavedFile> *unsaved_files = nullptr,
	  CXTranslationUnit_Flags options = CXTranslationUnit_None,
	  std::shared_ptr<Index> index = nullptr);

	static std::unique_ptr<TranslationUnit> from_ast_file(
	  const std::string &filename, std::shared_ptr<Index> index = nullptr);

  private:
	TranslationUnitImpl	*m_impl;

  private:
	TranslationUnit(UniqueCXTranslationUnit &&ptr, std::shared_ptr<Index> &index);

	TranslationUnit(const TranslationUnit &) = delete;

  public:
	TranslationUnit(TranslationUnit &&other) noexcept;

	~TranslationUnit();

	TranslationUnit &operator=(const TranslationUnit &) = delete;

	TranslationUnit &operator=(TranslationUnit &&rhs) noexcept;

  public:
	CXTranslationUnit native_handle() const;

//	Cursor cursor() const;

	std::string spelling() const;
#if 0
	get_includes() const {
	}

	std::unique_ptr<File> get_file(const std::string &filename) const {
		return File::from_name(*this, filename);
	}

	std::unique_ptr<SourceLocation> get_location(
	  const std::string &filename, unsigned offset) const
	{
		auto file = get_file(filename);
		return SourceLocation::from_offset(*this, file, offset);
	}

	std::unique_ptr<SourceLocation> get_location(
	  const std::string &filename, const std::pair<unsigned, unsigned> &position) const
	{
		auto file = get_file(filename);
		return SourceLocation::from_position(*this, file, position);
	}
#endif
}; // class TranslationUnit

} // namespace clangxx


#endif // clang_cpp_TranslationUnit_hpp
