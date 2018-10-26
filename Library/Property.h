#pragma once

#include "Getter.h"
#include "Setter.h"

template <typename T>
class Property {
public:
	Property(Getter<T>* getter = nullptr, Setter<T>* setter = nullptr) {
		m_getter = (getter == nullptr ? GetterFactory<T>::DEFAULT : getter);
		m_setter = (setter == nullptr ? SetterFactory<T>::DEFAULT : setter);
	}

	static Property<T> of(Getter<T>* getter, Setter<T>* setter) {
		return Property<T>(getter, setter);
	}

	template <class C>
	static Property<T> of(C* obj, T(C::*getter)()const, void(C::*setter)(const T&)) {
		return Property<T>(GetterFactory<T>::of(obj, getter),
						   SetterFactory<T>::of(obj, setter));
	}

	static Property<T> readOnlyOf(Getter<T>* getter) {
		return Property<T>(getter, SetterFactory<T>::READ_ONLY);
	}

	template <class C>
	static Property<T> readOnlyOf(C* obj, T(C::*getter)()const) {
		return Property<T>(GetterFactory<T>::of(obj, getter), SetterFactory<T>::READ_ONLY);
	}

	static Property<T> readOnlyOf(T constant) {
		return Property<T>(GetterFactory<T>::of(constant), SetterFactory<T>::READ_ONLY);
	}

	static Property<T> readOnlyOf(T(*getter)()) {
		return Property<T>(GetterFactory<T>::of(getter), SetterFactory<T>::READ_ONLY);
	}

	T get() const {
		return m_getter->get(m_field);
	}

	void set(const T& value) {
		m_setter->set(value, m_field);
	}

	operator T() const {
		return m_getter->get(m_field);
	}

	void operator =(const T& value) {
		m_setter->set(value, m_field);
	}

private:
	Getter<T>* m_getter;
	Setter<T>* m_setter;
	T m_field;
};
