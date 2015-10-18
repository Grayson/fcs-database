#pragma once

#include <cstddef>

namespace fcs
{
namespace database
{

class null_t {};
static const null_t null {};

template<typename T>
class nullable
{
public:
	nullable() {};
	nullable(const T & value)
		: m_value { value }
		, m_hasValue { true }
	{};
	
	bool has_value() const { return m_hasValue; }
	
	operator const T & () const { return m_value; }
	operator bool () const { return has_value(); }
	
private:
	T m_value;
	bool m_hasValue = false;
};

template<typename T>
bool operator == (nullable<T> nullableValue, T value)
{
	return nullableValue.has_value() && (T)nullableValue == value;
}

class data
{
public:
	data(void * pointer, std::size_t size)
	: m_pointer { pointer }
	, m_size { size }
	{};
	
	const void * GetPointer() const { return m_pointer; }
	std::size_t GetSize() const { return m_size; }
	
private:
	void * m_pointer;
	std::size_t m_size;
};

}
}