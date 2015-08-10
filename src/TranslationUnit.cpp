// -*- tab-width: 4 -*-
/*!
   @file TranslationUnit.cpp

   Copyright (c) 2015 pegacorn
*/
#include "clang-cpp/TranslationUnit.hpp"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "clang-c/CXString.h"
#include "clang-cpp/File.hpp"
#include "clang-cpp/Index.hpp"
#include "clang-cpp/memory.hpp"
#include "clang-cpp/UniqueCXObject.hpp"
//clang-cpp/Exception.hpp
//clang-cpp/SourceLocation.hpp


namespace clangxx {

class TranslationUnitImpl
{
  public:
	static std::shared_ptr<TranslationUnit> from_source(
	  const std::string &filename, const std::vector<std::string> &args,
	  const std::vector<UnsavedFile> &unsaved_files,
	  CXTranslationUnit_Flags options,
	  std::shared_ptr<Index> &index)
	{
		std::vector<const char *> args_array; args_array.reserve(args.size());
		for ( const auto &arg : args ) {
			args_array.push_back(arg.c_str());
		}

		std::vector<CXUnsavedFile> unsaved_array;
		unsaved_array.reserve(unsaved_files.size());
		std::vector<std::string> contents_array;
		contents_array.reserve(unsaved_array.size());
		for ( std::size_t i{0}; i < unsaved_files.size(); ++i ) {
			std::ostringstream ostream;
			std::copy(std::istreambuf_iterator<char>(*unsaved_files[i].contents),
					  std::istreambuf_iterator<char>(),
					  std::ostreambuf_iterator<char>(ostream));
			contents_array.push_back(ostream.str());

			CXUnsavedFile cx_unsaved_file;
			cx_unsaved_file.Filename = unsaved_files[i].filename.c_str();
			cx_unsaved_file.Contents = contents_array[i].c_str();
			cx_unsaved_file.Length = contents_array[i].size();

			unsaved_array.push_back(std::move(cx_unsaved_file));
		}

		UniqueCXTranslationUnit ptr(clang_parseTranslationUnit(
			index->native_handle(), filename.c_str(),
			args_array.data(), args_array.size(),
			unsaved_array.data(), unsaved_array.size(),
			options));
		if ( !ptr ) {
//TODO			throw TranslationUnitLoadError{"Error parsing translation unit."};
		}

//		return make_shared<TranslationUnit>(std::move(ptr), index);
		return std::shared_ptr<TranslationUnit>(new TranslationUnit(std::move(ptr), index));
	}

	static std::shared_ptr<TranslationUnit> from_ast_file(
	  const std::string &filename, std::shared_ptr<Index> &index)
	{
		UniqueCXTranslationUnit ptr(clang_createTranslationUnit(
									index->native_handle(), filename.c_str()));
		if ( !ptr ) {
//TODO			throw TranslationUnitLoadError{filename};
		}

//		return make_shared<TranslationUnit>(std::move(ptr), index);
		return std::shared_ptr<TranslationUnit>(new TranslationUnit(std::move(ptr), index));
	}

  private:
	std::shared_ptr<Index>	m_index;
	// warning: define m_index before m_cx_translation_unit
	UniqueCXTranslationUnit	m_cx_translation_unit;

  public:
	TranslationUnitImpl(UniqueCXTranslationUnit &&ptr, std::shared_ptr<Index> &index)
		: m_index(index)
		, m_cx_translation_unit(std::move(ptr))
	{}

  public:
	CXTranslationUnit native_handle() const {
		return m_cx_translation_unit.get();
	}

	std::string spelling() const {
		UniqueCXString cx_string(clang_getTranslationUnitSpelling(
								 m_cx_translation_unit.get()));
		if ( !cx_string ) {
// TODO: thorw
		}

		return clang_getCString(cx_string.get());
	}

	void reparse(const std::vector<UnsavedFile> &unsaved_files,
				 CXTranslationUnit_Flags options)
	{
		std::vector<CXUnsavedFile> unsaved_array;
		unsaved_array.reserve(unsaved_files.size());
		std::vector<std::string> contents_array;
		contents_array.reserve(unsaved_array.size());
		for ( std::size_t i{0}; i < unsaved_files.size(); ++i ) {
			std::ostringstream ostream;
			std::copy(std::istreambuf_iterator<char>(*unsaved_files[i].contents),
					  std::istreambuf_iterator<char>(),
					  std::ostreambuf_iterator<char>(ostream));
			contents_array.push_back(ostream.str());

			CXUnsavedFile cx_unsaved_file;
			cx_unsaved_file.Filename = unsaved_files[i].filename.c_str();
			cx_unsaved_file.Contents = contents_array[i].c_str();
			cx_unsaved_file.Length = contents_array[i].size();

			unsaved_array.push_back(std::move(cx_unsaved_file));
		}

		const int error_code{clang_reparseTranslationUnit(
			m_cx_translation_unit.get(),
			unsaved_array.size(), unsaved_array.data(),
			options)};
		if ( error_code ) {
//TODO: throw
		}
	}

	void save(const std::string &filename) {
		const auto options = clang_defaultSaveOptions(m_cx_translation_unit.get());
		const int error_code{clang_saveTranslationUnit(
			  m_cx_translation_unit.get(), filename.c_str(), options)};
		if ( !error_code ) {
//TODO: throw
		}
	}
}; // class TranslationUnitImpl

using Impl = TranslationUnitImpl;

std::shared_ptr<TranslationUnit> TranslationUnit::from_source(
  const std::string &filename, const std::vector<std::string> *args/* = nullptr*/,
  const std::vector<UnsavedFile> *unsaved_files/* = nullptr*/,
  CXTranslationUnit_Flags options/* = CXTranslationUnit_None*/,
  std::shared_ptr<Index> index/* = nullptr*/)
{
	static const std::vector<std::string> empty_args;
	if ( !args ) {
		args = &empty_args;
	}

	static const std::vector<UnsavedFile> empty_unsaved_files;
	if ( !unsaved_files ) {
		unsaved_files = &empty_unsaved_files;
	}

	if ( !index ) {
		index = Index::create();
	}

	return Impl::from_source(filename, *args, *unsaved_files, options, index);
}

std::shared_ptr<TranslationUnit> TranslationUnit::from_ast_file(
  const std::string &filename, std::shared_ptr<Index> index/* = nullptr*/)
{
	if ( !index ) {
		index = Index::create();
	}

	return Impl::from_ast_file(filename, index);
}

TranslationUnit::TranslationUnit(UniqueCXTranslationUnit &&ptr, std::shared_ptr<Index> &index)
	: m_impl(new Impl(std::move(ptr), index))
{}

TranslationUnit::TranslationUnit(TranslationUnit &&other) noexcept
{
	m_impl = std::move(other.m_impl);
	other.m_impl = nullptr;
}

TranslationUnit::~TranslationUnit()
{
	delete m_impl;
}

TranslationUnit &TranslationUnit::operator=(TranslationUnit &&rhs) noexcept
{
	std::swap(m_impl, rhs.m_impl);
	return *this;
}

CXTranslationUnit TranslationUnit::native_handle() const
{
	return m_impl->native_handle();
}

std::string TranslationUnit::spelling() const
{
	return m_impl->spelling();
}

void TranslationUnit::reparse(const std::vector<UnsavedFile> *unsaved_files/* = nullptr*/,
							  CXTranslationUnit_Flags options/* = CXTranslationUnit_None*/)
{
	static const std::vector<UnsavedFile> empty_unsaved_files;
	if ( !unsaved_files ) {
		unsaved_files = &empty_unsaved_files;
	}

	m_impl->reparse(*unsaved_files, options);
}

void TranslationUnit::save(const std::string &filename)
{
	m_impl->save(filename);
}

} // namespace clangxx
