#pragma once

#include "Getter.h"
#include "ObservableSetter.h"

#include <list>

template <typename T>
class Observer {
public:
	virtual void changed(const T& value) = 0;
};

template <typename T>
class ObservableProperty {
public:
	ObservableProperty(Getter<T>* getter = nullptr, ObservableSetter<T>* setter = nullptr) :
		m_notifier(this) {
		m_getter = (getter == nullptr ? GetterFactory<T>::DEFAULT : getter);
		m_setter = (setter == nullptr ? ObservableSetterFactory<T>::DEFAULT : setter);
	}

	static ObservableProperty<T> of(Getter<T>* getter, ObservableSetter<T>* setter) {
		return ObservableProperty<T>(getter, setter);
	}

	template <class C>
	static ObservableProperty<T> of(C* obj, T(C::*getter)()const, void(C::*setter)(const T&)) {
		return ObservableProperty<T>(GetterFactory<T>::of(obj, getter),
									 ObservableSetterFactory<T>::of(obj, setter));
	}

	static ObservableProperty<T> readOnlyOf(Getter<T>* getter) {
		return ObservableProperty<T>(getter, ObservableSetterFactory<T>::READ_ONLY);
	}

	template <class C>
	static ObservableProperty<T> readOnlyOf(C* obj, T(C::*getter)()const) {
		return ObservableProperty<T>(GetterFactory<T>::of(obj, getter),
									 ObservableSetterFactory<T>::READ_ONLY);
	}

	static ObservableProperty<T> readOnlyOf(T constant) {
		return ObservableProperty<T>(GetterFactory<T>::of(constant),
									 ObservableSetterFactory<T>::READ_ONLY);
	}

	static ObservableProperty<T> readOnlyOf(T(*getter)()) {
		return ObservableProperty<T>(GetterFactory<T>::of(getter),
									 ObservableSetterFactory<T>::READ_ONLY);
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

	void observe(Observer<T>* observer) {
		m_observers.push_back(observer);
	}

	void notifyObservers() {
		T value = get();
		for (Observer<T>* observer : m_observers)
			observer->changed(value);
	}

private:
	class DefaultObserverNotifier : public ObserverNotifier<T> {
	public:
		DefaultObserverNotifier(ObservableProperty<T>* property) :
			m_property(property)
		{}

		virtual void notify(const T& value) {
			for (Observer<T>* observer : m_observers)
				observer->changed(value);
		}

	private:
		ObservableProperty<T>* m_property;
	} m_notifier;

	Getter<T>* m_getter;
	ObservableSetter<T>* m_setter;
	T m_field;
	std::list<Observer<T>*> m_observers;
};
