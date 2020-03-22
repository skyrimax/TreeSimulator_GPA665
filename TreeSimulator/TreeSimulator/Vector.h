#pragma once

template <class T>
class Vector
{
public:
	//Type to indicate size of container
	typedef unsigned int size_type;

	//Default constructor
	Vector();
	Vector(size_type n);
	Vector(size_type n, T& val);
	Vector(const Vector<T>& vector);
	Vector(Vector<T>&& vector);
	~Vector();

	//Assignement operator overloading
	Vector<T>& operator=(const Vector<T>& vector);
	Vector<T>& operator=(Vector<T>&& vector);

	//Capacity related member functions
	size_type size() const;
	void resize(size_type n);
	void resize(size_type n, const T& val);
	size_type capacity() const;
	bool empty() const;
	void reserve(size_type n);
	void shrink_to_fit();

	//Element access methods
	T& operator[](size_type n);
	const T& operator[](size_type n) const;
	T& at(size_type n);
	const T& at(size_type n) const;
	T& front();
	const T& front() const;
	T& back();
	const T& back() const;
	T* data();
	const T* data() const;

	//Modifiers
	void assign(size_type n, const T& val);
	void push_back(const T& val);
	void push_back(T&& val);
	void pop_back();
	void erase(size_type pos, size_type n = 1);
	void swap(Vector<T>& vector);
	void clear();

private:
	T* m_data;
	size_type m_size;
	size_type m_capacity;
};

template<class T>
void swap(Vector<T>& x, Vector<T>& y);

#include "Vector.tpp"