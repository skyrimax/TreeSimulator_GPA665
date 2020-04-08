#include "array.h"

#pragma once

#include <cmath>
#include <utility>
#include <exception>

template<class T>
array<T>::array()
{
	//Create an array on one element and initialize internal attributes
	m_data = new T[1];
	m_size = 0;
	m_capacity = 1;
}

template<class T>
array<T>::array(size_type n)
{
	//Determining necessary capacity to store n elements
	m_capacity = (int)pow(2, ceil(log2((double)n)));

	//Allocating the array of necessary capacity
	m_data = new T[m_capacity];

	//Setting the usable size of the object at n
	m_size = n;
}

template<class T>
array<T>::array(size_type n, const T& val)
{
	//Determining necessary capacity to store n elements
	m_capacity = (int)pow(2, ceil(log2((double)n)));

	//Allocating the array of necessary capacity
	m_data = new T[m_capacity];

	//Setting the usable size of the object at n
	m_size = n;

	//Filling array with copies of specified val
	for (size_type i=0; i < n; ++i) {
		m_data[i] = T(val);
	}
}

template<class T>
array<T>::array(const array<T>& vector)
{
	//Copying internal attributes from other vector
	m_size = vector.m_size;
	m_capacity = vector.m_capacity;

	//Create arraw containing as many elements as the capacity specifies
	m_data = new T[m_capacity];

	//Filling array with copies of the elemenents present in the other array
	for (size_type i = 0; i < m_size; ++i) {
		m_data[i] = vector.m_data[i];
	}
}

template<class T>
array<T>::array(array<T>&& vector)
{
	//Transfert of the other vector's internal data to the new object
	m_data = vector.m_data;
	m_size = vector.m_size;
	m_capacity = vector.m_capacity;

	//Removing ownership of data from other object
	vector.m_data = nullptr;
	vector.m_size = 0;
	vector.m_capacity = 0;
}

template <class T>
array<T>::~array()
{
	//Deallocating the array
	delete[] m_data;
}

template<class T>
array<T>& array<T>::operator=(const array<T>& vector)
{
	if (this != &vector) {
		delete[] m_data;

		m_capacity = vector.m_capacity;
		m_size = vector.m_size;

		m_data = new T[m_capacity];

		for (size_type i=0; i < m_size; ++i) {
			m_data[i] = T(vector.m_data[i]);
		}
	}

	return *this;
}

template<class T>
array<T>& array<T>::operator=(array<T>&& vector)
{
	if (this != &vector) {
		delete[] m_data;

		// Transfert of the other vector's internal data to the calling object
		m_size = vector.m_size;
		m_capacity = vector.m_capacity;
		m_data = vector.m_data;

		// Removing ownership of data from other object
		vector.m_data = nullptr;
		vector.m_size = 0;
		vector.m_capacity = 0;
	}

	return *this;
}

template<class T>
typename array<T>::size_type array<T>::size() const
{
	return m_size;
}

template<class T>
void array<T>::resize(size_type n)
{
	//For loop to remove elements past n if n is smaller than current size
	for (int i = n; i < m_size; ++i) {
		m_data[i] = T();
	}

	//Will allocate more memory top the array if n if bigger than current capacity
	reserve(n);

	//Expending accessible size to match n
	m_size = n;
}

template<class T>
void array<T>::resize(size_type n, const T & val)
{
	//For loop to remove elements past n if n is smaller than current size
	for (int i = n; i < m_size; ++i) {
		m_data[i] = T();
	}

	//Will allocate more memory top the array if n if bigger than current capacity
	reserve(n);

	//Filling new space with copies of specified val
	for (int i = m_size; i < n; ++i) {
		m_data[i] = T(val);
	}

	//Expending accessible size to match n
	m_size = n;
}

template<class T>
typename array<T>::size_type array<T>::capacity() const
{
	return m_capacity;
}

template<class T>
bool array<T>::empty() const
{
	//Object is considered empty if size (nb of elements) is 0
	return m_size==0;
}

template<class T>
void array<T>::reserve(size_type n)
{
	//Operation will only be executed if desired capacity n is smaller then current capacity
	if (n > m_capacity) {
		//Determining necessary capacity
		size_type newCapacity = (size_type) pow(2, ceil(log2((double)n)));

		//Allocating new array of determined capacity
		T* newArray = new T[newCapacity];

		//Copy of elements from previous array to new array
		for (size_type i = 0; i < m_size; ++i) {
			newArray[i] = std::move(m_data[i]);
		}

		//Deallocation of previous array
		delete[] m_data;

		//New array is assigned as the object's internal arraw
		m_data = newArray;
		//The internal capacity is set to the determined capacity
		m_capacity = newCapacity;
	}
}

template<class T>
void array<T>::shrink_to_fit()
{
	//Operation will only be executed if reducing the size of the array will allow it to still contain all of it's elements
	if (m_capacity / 2 >= m_size) {
		//Determining necessary capacity
		size_type newCapacity = pow(2, ceil(log2((double)m_size)));

		//Allocating new array of determined capacity
		T* newArray = new T[newCapacity];

		//Copy of elements from previous array to new array
		for (int i = 0; i < m_size; i++) {
			newArray[i] = m_data[i];
		}

		//Deallocation of previous array
		delete[] m_data;

		//New array is assigned as the object's internal arraw
		m_data = newArray;
		//The internal capacity is set to the determined capacity
		m_capacity = newCapacity;
	}
}

// Return element at index n
template<class T>
T & array<T>::operator[](size_type n)
{
	return m_data[n];
}

// Return element at index n
template<class T>
const T & array<T>::operator[](size_type n) const
{
	return m_data[n];
}

// Return element at index n, throw exception if index is out of range
template<class T>
T & array<T>::at(size_type n)
{
	if (n < m_size) {
		return m_data[n];
	}
	else {
		throw std::out_of_range("vector::_M_range_check");
	}
}

// Return element at index n, throw exception if index is out of range
template<class T>
const T & array<T>::at(size_type n) const
{
	if (n < m_size) {
		return m_data[n];
	}
	else {
		throw std::out_of_range("vector::_M_range_check");
	}
}

// Return first element, throw exception if index is out of range
template<class T>
T & array<T>::front()
{
	if (m_size != 0) {
		return m_data[0];
	}
	else {
		throw std::out_of_range("vector::_M_range_check");
	}
}

// Return first element, throw exception if index is out of range
template<class T>
const T & array<T>::front() const
{
	if (m_size != 0) {
		return m_data[0];
	}
	else {
		throw std::out_of_range("vector::_M_range_check");
	}
}

// Return last element, throw exception if index is out of range
template<class T>
T & array<T>::back()
{
	if (m_size != 0) {
		return m_data[m_size - 1];
	}
	else {
		throw std::out_of_range("vector::_M_range_check");
	}
}

// Return last element, throw exception if index is out of range
template<class T>
const T & array<T>::back() const
{
	if (m_size != 0) {
		return m_data[m_size - 1];
	}
	else {
		throw std::out_of_range("vector::_M_range_check");
	}
}

// Return raw data array
template<class T>
T * array<T>::data()
{
	return m_data;
}

// Return raw data array
template<class T>
const T * array<T>::data() const
{
	return m_data;
}

// The container is recreated with only the specified value
template<class T>
void array<T>::assign(size_type n, const T & val)
{
	// CLear container
	delete[] m_data;

	// Calculate new requiered capacity
	m_capacity = pow(2, ceil(log2((double)n)));
	m_size = n;

	// Allocate memory for the container
	m_data = new T [m_capacity];

	// Assign each element to the desired value
	for (int i = 0; i < m_size; ++i) {
		m_data[i] = T(val);
	}
}

// Add value to the end of the container
template<class T>
void array<T>::push_back(const T & val)
{
	// Enlage container if necessary
	reserve(m_size + 1);

	// Assign value to past last element
	m_data[m_size] = val;

	// Add one element to the size
	++m_size;
}

// Move value to the end of the container
template<class T>
void array<T>::push_back(T && val)
{
	// Enlage container if necessary
	reserve(m_size + 1);

	// Move value to past last element
	m_data[m_size] = std::move(val);

	// Add one element to the size
	++m_size;
}

//Remove last element from the container
template<class T>
void array<T>::pop_back()
{
	// Decrease size by one element
	--m_size;

	// Reset last element
	m_data[m_size] = T();
}

// Remove n element starting at index pos
template<class T>
void array<T>::erase(size_type pos, size_type n)
{
	int i;

	// Move the data past limit over data to be erased
	for (i = pos; i < pos + n && i + n < m_size; ++i) {
		m_data[i] = m_data[i + n];
	}

	// Reset remaining data
	for (; i < m_size; ++i) {
		m_data[i] = T();
	}

	// Reduce sizer by n elements
	m_size -= n;
}

// This container and vector swap content
template<class T>
void array<T>::swap(array<T>& vector)
{
	// Save data from this container for swap
	size_type holdSize = m_size;
	size_type holdCapacity = m_capacity;
	T* holdArray = m_data;

	// Overwrite this container's data with the one from vector
	m_size = vector.m_size;
	m_capacity = vector.m_capacity;
	m_data = vector.m_data;

	// Overwrite vector's data with the one from this container's data save in local variables
	vector.m_size = holdSize;
	vector.m_capacity = holdCapacity;
	vector.m_data = holdArray;
}

// Remove all data from container
template<class T>
void array<T>::clear()
{
	// Reset all elements
	for (size_type i = 0; i < m_size; ++i) {
		m_data[i] = T();
	}

	// Size is set to 0
	m_size = 0;
}

// Convenience fonction to swap data from 2 containers
template<class T>
inline void swap(array<T>& x, array<T>& y)
{
	x.swap(y);
}