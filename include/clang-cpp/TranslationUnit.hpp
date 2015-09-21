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
#include "clang-cpp/Cursor.hpp"
#include "clang-cpp/File.hpp"
//#include "clang-cpp/SourceLocation.hpp"
#include "clang-cpp/switch_port.hpp"
#include "clang-cpp/UniqueCXObject.hpp"
#include "clang-cpp/UnsavedFile.hpp"


namespace clangxx {

class Index;

class CLANGXX_API TranslationUnit: public std::enable_shared_from_this<TranslationUnit>
{
	class Impl;

  public:
	static std::shared_ptr<TranslationUnit> from_source(
	  const std::string &filename, const std::vector<std::string> *args = nullptr,
	  const std::vector<UnsavedFile> *unsaved_files = nullptr,
	  CXTranslationUnit_Flags options = CXTranslationUnit_None,
	  std::shared_ptr<Index> index = nullptr);

	static std::shared_ptr<TranslationUnit> from_ast_file(
	  const std::string &filename, std::shared_ptr<Index> index = nullptr);

  private:
	std::unique_ptr<Impl>	m_impl;

  private:
	TranslationUnit(UniqueCXTranslationUnit &&ptr, std::shared_ptr<const Index> index);

  public:
	~TranslationUnit();

	TranslationUnit(const TranslationUnit &) = delete;
	TranslationUnit(TranslationUnit &&other) noexcept;

	TranslationUnit &operator=(const TranslationUnit &) = delete;
	TranslationUnit &operator=(TranslationUnit &&other) noexcept;

  public:
	CXTranslationUnit native_handle() const noexcept;

	Cursor cursor() const {
		return Cursor::from_result(shared_from_this());
	}

	std::string spelling() const;

//	get_includes();

	File get_file(const std::string &filename) const {
		return File::from_name(shared_from_this(), filename);
	}
#if 0
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

	get_extent(const std::string &filename, locations) const;

	diagnostics() const;
#endif
	void reparse(const std::vector<UnsavedFile> *unsaved_files = nullptr,
				 CXTranslationUnit_Flags options = CXTranslationUnit_None);

	void save(const std::string &filename);

#if 0
	CodeCompletionResults codeComplete(path, line, column, unsaved_files=None,
				 include_macros=False, include_code_patterns=False,
				 include_brief_comments=False);

	get_tokens(locations=None, extent=None);
#endif
}; // class TranslationUnit

} // namespace clangxx


#endif // clang_cpp_TranslationUnit_hpp
