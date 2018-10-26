#pragma once

#include <stdexcept>

template <typename T>
class Setter {
public:
	virtual ~Setter() {}
	virtual void set(const T& value, T& field) const = 0;
};

template <typename T>
class DefaultSetter : public Setter<T> {
public:
	virtual void set(const T& value, T& field) const {
		field = value;
	}
};

template <class C, typename T>
class ClassSetter : public Setter<T> {
public:
	ClassSetter(C* obj, void(C::*setter)(const T&)) :
		m_obj(obj),
		m_setter(setter)
	{}

	virtual void set(const T& value, T& field) const {
		(void)(field);
		(m_obj->*m_setter)(value);
	}

private:
	C* m_obj;
	void(C::*m_setter)(const T&);
};

template <typename T>
class StaticSetter : public Setter<T> {
public:
	StaticSetter(void(*setter)(const T&)) :
		m_setter(setter)
	{}

	virtual void set(const T& value, T& field) const {
		(void)(field);
		(*m_setter)(value);
	}

private:
	void(*m_setter)(const T&);
};

class CallSetterOfReadOnlyPropertyException : public std::logic_error {
public:
	CallSetterOfReadOnlyPropertyException() :
		std::logic_error("Attempt to set value for read only property") {}
};

template <typename T>
class ReadOnlySetter : public Setter<T> {
public:
	virtual void set(const T& value, T& field) const {
		(void)(value);
		(void)(field);
		throw CallSetterOfReadOnlyPropertyException();
	}
};

template <typename T>
class SetterFactory {
public:
	static Setter<T>* const DEFAULT;

	static Setter<T>* const READ_ONLY;

	template <class C>
	static Setter<T>* of(C* obj, void(C::*setter)(const T&)) {
		return new ClassSetter<C, T>(obj, setter);
	}

	static Setter<T>* of(void (*setter)(const T&)) {
		return new StaticSetter<T>(setter);
	}
};

template <typename T>
Setter<T>* const SetterFactory<T>::DEFAULT = new DefaultSetter<T>();

template <typename T>
Setter<T>* const SetterFactory<T>::READ_ONLY = new ReadOnlySetter<T>();
