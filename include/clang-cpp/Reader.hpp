// -*- tab-width: 4 -*-
/*!
   @file Reader.hpp

   Copyright (c) 2015 pegacorn
*/
#ifndef clang_cpp_Reader_hpp
#define clang_cpp_Reader_hpp

#include <iterator>
#include <stdexcept>
#include <type_traits>
#include "clang-cpp/Generator.hpp"


namespace clangxx {

template<
	typename Value
	, typename Index = unsigned int
	, typename Generator = Value (*)(Index)
	>
class RandomAccessReader
{
  public:
	using value_type				= Value;
	using size_type					= Index;
	using difference_type			= typename std::make_signed<Index>::type;
	using generator_type			= Generator;
	using const_iterator			= RandomAccessGenerator<const Value, difference_type, Generator>;
	using const_reverse_iterator	= std::reverse_iterator<const_iterator>;

  private:
	const generator_type	m_generator;
	const size_type			m_number{};

  public:
	RandomAccessReader(generator_type generator, size_type number)
		: m_generator(generator)
		, m_number(number)
	{}

  public:
	const_iterator begin() const noexcept {
		return const_iterator(m_generator, 0);
	}

	const_iterator cbegin() const noexcept {
		return begin();
	}

	const_iterator end() const noexcept {
		return const_iterator(m_generator, size());
	}

	const_iterator cend() const noexcept {
		return end();
	}

	const_reverse_iterator rbegin() const noexcept {
		return const_reverse_iterator(end());
	}

	const_reverse_iterator crbegin() const noexcept {
		return rbegin();
	}

	const_reverse_iterator rend() const noexcept {
		return const_reverse_iterator(begin());
	}

	const_reverse_iterator crend() const noexcept {
		return rend();
	}

	size_type size() const noexcept {
		return m_number;
	}

	bool empty() const noexcept {
		return size() == 0;
	}

	value_type operator[](size_type n) const {
		return m_generator(n);
	}

	value_type at(size_type n) const {
		if ( n >= size() ) {
			throw std::out_of_range("RandomAccessReader");
		}
		return (*this)[n];
	}

	value_type front() const {
		return (*this)[0];
	}

	value_type back() const {
		return (*this)[size() - 1];
	}
}; // class RandomAccessReader

} // namespace clangxx


#endif // clang_cpp_Reader_hpp
