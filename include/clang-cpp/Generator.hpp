// -*- tab-width: 4 -*-
/*!
   @file Generator.hpp

   Copyright (c) 2015 pegacorn
*/
#ifndef clang_cpp_Generator_hpp
#define clang_cpp_Generator_hpp

#include <iterator>
#include <type_traits>


namespace clangxx {

template<
	typename Value
	, typename Distance = int
	, typename Generator = Value (*)(typename std::make_unsigned<Distance>::type)
	>
class RandomAccessGenerator
	: public std::iterator<std::random_access_iterator_tag
						   , Value
						   , Distance>
{
  private:
	using NonConstValue		= std::remove_const<Value>;

  public:
	using index_type		= typename std::make_unsigned<Distance>::type;
	using generator_type	= Generator;

  private:
	const generator_type	&m_generator;
	index_type				m_index{};

  public:
	explicit RandomAccessGenerator(const generator_type &generator, index_type index = 0) noexcept
		: m_generator(generator)
		, m_index{index}
	{}

  public:
	Value operator*() const {
		return (*this)[0];
	}

	RandomAccessGenerator &operator++() {
		return *this += 1;
	}

	RandomAccessGenerator operator++(int) {
		const auto prev = *this;
		++(*this);
		return prev;
	}

	RandomAccessGenerator &operator--() {
		return *this -= 1;
	}

	RandomAccessGenerator operator--(int) {
		const auto prev = *this;
		--(*this);
		return prev;
	}

	RandomAccessGenerator &operator+=(Distance n) {
		m_index += n;
		return *this;
	}

	RandomAccessGenerator operator+(Distance n) const {
		auto next = *this;
		next += n;
		return next;
	}

	RandomAccessGenerator &operator-=(Distance n) {
		return *this += -n;
	}

	RandomAccessGenerator operator-(Distance n) const {
		auto next = *this;
		next -= n;
		return next;
	}

	Value operator[](Distance n) const {
		return m_generator(m_index + n);
	}


	friend RandomAccessGenerator operator+(Distance n, const RandomAccessGenerator &iter) {
		return iter + n;
	}

	friend Distance operator-(const RandomAccessGenerator &x, const RandomAccessGenerator &y) {
		return x.m_index - y.m_index;
	}

#define clangxx_DEFINE_BINARY_OPERATOR(d_op) \
	friend bool operator d_op(const RandomAccessGenerator &x, const RandomAccessGenerator &y) { \
		return x.m_index d_op y.m_index; \
	}

clangxx_DEFINE_BINARY_OPERATOR(==)
clangxx_DEFINE_BINARY_OPERATOR(!=)
clangxx_DEFINE_BINARY_OPERATOR(<)
clangxx_DEFINE_BINARY_OPERATOR(<=)
clangxx_DEFINE_BINARY_OPERATOR(>)
clangxx_DEFINE_BINARY_OPERATOR(>=)

#undef clangxx_DEFINE_BINARY_OPERATOR

}; // class RandomAccessGenerator

} // namespace clangxx


#endif // clang_cpp_Generator_hpp
