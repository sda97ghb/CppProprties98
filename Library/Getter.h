#pragma once

template <typename T>
class Getter {
public:
	virtual ~Getter() {}
	virtual T get(const T& field) const = 0;
};

template <typename T>
class DefaultGetter : public Getter<T> {
public:
	virtual T get(const T& field) const {
		return field;
	}
};

template <class C, typename T>
class ClassGetter : public Getter<T> {
public:
	ClassGetter(C* obj, T(C::*getter)()const) :
		m_obj(obj),
		m_getter(getter)
	{}

	virtual T get(const T& field) const {
		(void)(field);
		return (m_obj->*m_getter)();
	}

private:
	C* m_obj;
	T(C::*m_getter)()const;
};

template <typename T>
class StaticGetter : public Getter<T> {
public:
	StaticGetter(T(*getter)()) :
		m_getter(getter)
	{}

	virtual T get(const T& field) const {
		(void)(field);
		return (*m_getter)();
	}

private:
	T(*m_getter)();
};

template <typename T>
class ConstGetter : public Getter<T> {
public:
	ConstGetter(const T& constant) :
		m_constant(constant)
	{}

	virtual T get(const T& field) const {
		(void)(field);
		return m_constant;
	}

private:
	T m_constant;
};

template <typename T>
class GetterFactory {
public:
	static Getter<T>* const DEFAULT;

	template <class C>
	static Getter<T>* of(C* obj, T(C::*getter)()const) {
		return new ClassGetter<C, T>(obj, getter);
	}

	static Getter<T>* of(T(*getter)()) {
		return new StaticGetter<T>(getter);
	}

	static Getter<T>* of(const T& constant) {
		return new ConstGetter<T>(constant);
	}
};

template <typename T>
Getter<T>* const GetterFactory<T>::DEFAULT = new DefaultGetter<T>();
