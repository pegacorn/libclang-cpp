// -*- tab-width: 4 -*-
/*!
  @file UniqueCXObject.hpp

  Copyright (c) 2015 pegacorn
*/
#ifndef clang_cpp_UniqueCXObject_hpp
#define clang_cpp_UniqueCXObject_hpp

#include <cstddef>
#include <utility>
#include "clang-c/BuildSystem.h"
#include "clang-c/CXCompilationDatabase.h"
#include "clang-c/CXString.h"
#include "clang-c/Index.h"


namespace clangxx {

template<class TCXObject>
bool is_null(TCXObject cxObject)
{
	return cxObject == nullptr;
}

inline bool is_null(const CXString &cxString)
{
	return cxString.data == nullptr;
}

inline bool is_null(const CXTUResourceUsage &cxTUResourceUsage)
{
	return (cxTUResourceUsage.data == nullptr)
		&& (cxTUResourceUsage.entries == nullptr);
}


template<typename TCXObject, void (*t_disposer)(TCXObject)>
struct CXObjectDisposer
{
	constexpr CXObjectDisposer() noexcept = default;

	void operator()(TCXObject cx_object) const noexcept {
		t_disposer(cx_object);
	}
}; // struct CXObjectDisposer


template<class TCXObject, void (*t_disposer)(TCXObject)>
class UniqueCXObject
{
  public:
	using element_type	= TCXObject;
	using deleter_type	= CXObjectDisposer<TCXObject, t_disposer>;

  private:
	element_type	m_cx_object;
	deleter_type	m_disposer;

  public:
	// constructors
	constexpr UniqueCXObject() noexcept
		: m_cx_object()
	{}

	UniqueCXObject(std::nullptr_t) noexcept
		: UniqueCXObject()
	{}

	explicit UniqueCXObject(element_type other) noexcept
		: m_cx_object(std::move(other))
	{}

	UniqueCXObject(const UniqueCXObject &) = delete;

	UniqueCXObject(UniqueCXObject &&other) noexcept
		: m_cx_object(other.release())
	{}

	// destructor
	~UniqueCXObject() {
		reset();
	}

	// assignment
	UniqueCXObject &operator=(UniqueCXObject &) = delete;

	UniqueCXObject &operator=(UniqueCXObject &&rhs) noexcept {
		reset(rhs.release());
		return *this;
	}

	UniqueCXObject &operator=(std::nullptr_t) noexcept {
		reset();
		return *this;
	}

  public:
	// observers
	element_type operator*() const {
		return m_cx_object;
	}

	element_type operator->() const noexcept {
		return m_cx_object;
	}

	element_type get() const noexcept {
		return m_cx_object;
	}

	const deleter_type &get_deleter() noexcept {
		return m_disposer;
	}

	explicit operator bool() const noexcept {
		return !is_null(m_cx_object);
	}

	// modifiers
	element_type release() noexcept {
		element_type backup(std::move(m_cx_object));
		m_cx_object = element_type();
		return backup;
	}

	void reset(element_type other = element_type()) noexcept {
		element_type tmp(std::move(m_cx_object));
		m_cx_object = std::move(other);
		if ( !is_null(tmp) ) {
			m_disposer(tmp);
		}
	}

	void swap(UniqueCXObject &other) noexcept {
		std::swap(m_cx_object, other);
	}
}; // class UniqueCXObject

#define clangxx_DEFINE_UniqueCXObject(DCXObject, d_disposer) \
	using Unique ## DCXObject = UniqueCXObject<DCXObject, d_disposer>
#define clangxx_DEFINE_UniqueCXObjectPtr(DCXObject, d_disposer) \
	using Unique ## DCXObject = UniqueCXObject<DCXObject *, d_disposer>

// BuildSystem.h
clangxx_DEFINE_UniqueCXObject(CXVirtualFileOverlay, &clang_VirtualFileOverlay_dispose);
clangxx_DEFINE_UniqueCXObject(CXModuleMapDescriptor, &clang_ModuleMapDescriptor_dispose);

// CXCompilationDatabase.h
clangxx_DEFINE_UniqueCXObject(CXCompilationDatabase, &clang_CompilationDatabase_dispose);
clangxx_DEFINE_UniqueCXObject(CXCompileCommands, &clang_CompileCommands_dispose);

// CXString.h
clangxx_DEFINE_UniqueCXObject(CXString, &clang_disposeString);

// Index.h
clangxx_DEFINE_UniqueCXObject(CXIndex, &clang_disposeIndex);
clangxx_DEFINE_UniqueCXObjectPtr(CXSourceRangeList, &clang_disposeSourceRangeList);
clangxx_DEFINE_UniqueCXObject(CXDiagnosticSet, &clang_disposeDiagnosticSet);
clangxx_DEFINE_UniqueCXObject(CXDiagnostic, &clang_disposeDiagnostic);
clangxx_DEFINE_UniqueCXObject(CXTranslationUnit, &clang_disposeTranslationUnit);
clangxx_DEFINE_UniqueCXObject(CXTUResourceUsage, &clang_disposeCXTUResourceUsage);
clangxx_DEFINE_UniqueCXObjectPtr(CXPlatformAvailability, &clang_disposeCXPlatformAvailability);
clangxx_DEFINE_UniqueCXObject(CXCursorSet, &clang_disposeCXCursorSet);
clangxx_DEFINE_UniqueCXObjectPtr(CXCursor, &clang_disposeOverriddenCursors);
clangxx_DEFINE_UniqueCXObjectPtr(CXCodeCompleteResults, &clang_disposeCodeCompleteResults);
clangxx_DEFINE_UniqueCXObject(CXRemapping, &clang_remap_dispose);
clangxx_DEFINE_UniqueCXObject(CXIndexAction, &clang_IndexAction_dispose);

#undef clangxx_DEFINE_UniqueCXObjectPtr
#undef clangxx_DEFINE_UniqueCXObject

#if 0
// TODO: CXToken
template<>
struct CXObjectDisposer<CXToken *>
{
	constexpr CXObjectDisposer() noexcept = default;

	void operator()(CXTranslationUnit cxUnit, CXToken *cxTokens, unsigned numCXTokens) const noexcept {
		clang_disposeTokens(cxUnit, cxToken, numCXTokens);
	}
}; // struct CXObjectDisposer<CXToken *>
#endif

} // namespace clangxx


#endif // clang_cpp_UniqueCXObject_hpp
