#pragma once




template <typename T>
class Container
{
public:
	Container();
	Container(const Container & cont);
	~Container();

	void PushFront(T val);
	void PushBack(T val);

	T PopFront();
	T PopBack();

	T operator[] (size_t index) const;
	T First() const;
	T Last() const;

	template <typename Command>
	void Foreach(Command com);
	template <typename Command>
	void Foreach(Command com) const;

	size_t Size() const;
	bool IsEmpty() const;

	void Reset();

private:
	struct List
	{
		T val;
		List * prev;
		List * next;

		List() : prev(nullptr), next(nullptr) {}
		List(const T & _val) : val(_val), prev(nullptr), next(nullptr) {}
		List(const T & _val, List * _prev, List* _next) : val(_val), prev(_prev), next(_next) {}
		~List() {}
	};

	List * m_first;
	List * m_last;
	size_t m_size;
};