#pragma once

#include "CallSetterOfReadOnlyProperty.h"

template <typename T>
class ObserverNotifier {
public:
	virtual void notify(const T& value) = 0;
};

template <typename T>
class ObservableSetter {
public:
	virtual ~ObservableSetter() {}
	virtual void set(const T& value, T& field, ObserverNotifier<T>* notifier) const = 0;
};

template <typename T>
class DefaultObservableSetter : public ObservableSetter<T> {
public:
	virtual void set(const T& value, T& field, ObserverNotifier<T>* notifier) const {
		field = value;
		notifier->notify(field);
	}
};

template <class C, typename T>
class ClassObservableSetter : public ObservableSetter<T> {
public:
	ClassObservableSetter(C* obj, void(C::*setter)(const T&)) :
		m_obj(obj),
		m_setter(setter)
	{}

	virtual void set(const T& value, T& field, ObserverNotifier<T>* notifier) const {
		(void)(field);
		(void)(notifier);
		(m_obj->*m_setter)(value);
	}

private:
	C* m_obj;
	void(C::*m_setter)(const T&);
};

template <typename T>
class StaticObservableSetter : public ObservableSetter<T> {
public:
	StaticObservableSetter(void(*setter)(const T&)) :
		m_setter(setter)
	{}

	virtual void set(const T& value, T& field, ObserverNotifier<T>* notifier) const {
		(void)(field);
		(void)(notifier);
		(*m_setter)(value);
	}

private:
	void(*m_setter)(const T&);
};

template <typename T>
class ReadOnlyObservableSetter : public ObservableSetter<T> {
public:
	virtual void set(const T& value, T& field, ObserverNotifier<T>* notifier) const {
		(void)(value);
		(void)(field);
		(void)(notifier);
		throw CallSetterOfReadOnlyPropertyException();
	}
};

template <typename T>
class ObservableSetterFactory {
public:
	static ObservableSetter<T>* const DEFAULT;

	static ObservableSetter<T>* const READ_ONLY;

	template <class C>
	static ObservableSetter<T>* of(C* obj, void(C::*setter)(const T&)) {
		return new ClassObservableSetter<C, T>(obj, setter);
	}

	static ObservableSetter<T>* of(void (*setter)(const T&)) {
		return new StaticObservableSetter<T>(setter);
	}
};

template <typename T>
ObservableSetter<T>* const ObservableSetterFactory<T>::DEFAULT = new DefaultObservableSetter<T>();

template <typename T>
ObservableSetter<T>* const ObservableSetterFactory<T>::READ_ONLY = new ReadOnlyObservableSetter<T>();
