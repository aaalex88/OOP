#pragma once

#include "stdafx.h"

#include <assert.h>

#include "Container.h"


template<typename T>
inline Container<T>::Container()
	: m_size(0)
	, m_first(nullptr)
	, m_last(nullptr)
{
}

template<typename T>
Container<T>::Container(const Container & cont)
	: Container()
{
	cont.Foreach([this](const T & val) { PushBack(val); });
}

template<typename T>
Container<T>::~Container()
{
	Reset();
}


template<typename T>
void Container<T>::PushFront(T val)
{
	if (m_size == 0) {
		m_first = m_last = new List(val);
		m_size++;
		return;
	}

	m_first = new List(val, nullptr, m_first);

	if (m_first->next != nullptr) {
		m_first->next->prev = m_first;
	}

	m_size++;
}

template<typename T>
void Container<T>::PushBack(T val)
{
	if (m_size == 0) {
		m_first = m_last = new List(val);
		m_size++;
		return;
	}

	m_last->next = new List(val, m_last, nullptr);
	m_last = m_last->next;

	m_size++;
}

template<typename T>
T Container<T>::PopFront()
{
	assert(m_size != 0);

	m_size--;

	if (m_first == m_last) {
		T ret = m_first->val;
		delete m_first;
		m_first = m_last = nullptr;
		return ret;
	}

	T ret = m_first->val;
	m_first = m_first->next;
	delete m_first->prev;
	m_first->prev = nullptr;

	return ret;
}

template<typename T>
T Container<T>::PopBack()
{
	assert(m_size != 0);

	m_size--;

	if (m_first == m_last) {
		T ret = m_first->val;
		delete m_first;
		m_first = m_last = nullptr;
		return ret;
	}

	T ret = m_last->val;
	m_last = m_last->prev;
	delete m_last->next;
	m_last->next = nullptr;

	return ret;
}


template<typename T>
T Container<T>::operator[](size_t index) const
{
	assert(index < m_size);

	List * l = m_first;

	for (size_t i = 0; i < index; ++i) {
		l = l->next;
	}

	return l->val;
}

template<typename T>
T Container<T>::First() const
{
	assert(m_first != nullptr);

	return m_first->val;
}

template<typename T>
T Container<T>::Last() const
{
	assert(m_last != nullptr);

	return m_last->val;
}

template<typename T>
template<typename Command>
inline void Container<T>::Foreach(Command com)
{
	List * it = m_first;
	while (it != nullptr) {
		com(it->val);
		it = it->next;
	}
}

template<typename T>
template<typename Command>
inline void Container<T>::Foreach(Command com) const
{
	List * it = m_first;
	while (it != nullptr) {
		com(it->val);
		it = it->next;
	}
}

template<typename T>
size_t Container<T>::Size() const
{
	// TODO assert length!
	return m_size;
}

template<typename T>
bool Container<T>::IsEmpty() const
{
	return Size() == 0;
}

template<typename T>
void Container<T>::Reset()
{
	List * it = m_first;
	while (it != nullptr) {
		List * next = it->next;
		delete it;
		it = next;
	}

	m_first = m_last = nullptr;
	m_size = 0;
}

