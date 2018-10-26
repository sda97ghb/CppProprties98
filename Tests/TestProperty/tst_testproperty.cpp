#include <QtTest>

#include "Getter.h"
#include "Property.h"
#include "Setter.h"

class TestProperty : public QObject {
	Q_OBJECT

private slots:
	void testDefaultProperties();
	void testFunctorProperties();
	void testMethodProperties();
	void testReadOnlyProperties();
};

//------------------------------------------------------------------------------

void TestProperty::testDefaultProperties() {
	class User {
	public:
		Property<QString> Name;
		Property<int> Age;
	} user;

	user.Name = "John Doe";
	user.Age = 42;

	QString name = user.Name;
	int age = user.Age;

	QCOMPARE(name, "John Doe");
	QCOMPARE(age, 42);
}

//------------------------------------------------------------------------------

class User {
public:
	class NameGetter : public Getter<QString> {
	public:
		NameGetter(User* user);
		QString get(const QString& field) const;
	private:
		User* m_user;
	};

	class NameSetter : public Setter<QString> {
	public:
		NameSetter(User* user);
		void set(const QString& value, QString& field) const;
	private:
		User* m_user;
	};

	class AgeGetter : public Getter<int> {
	public:
		AgeGetter(User* user);
		int get(const int& field) const;
	private:
		User* m_user;
	};

	class AgeSetter : public Setter<int> {
	public:
		AgeSetter(User* user);
		void set(const int& value, int& field) const;
	private:
		User* m_user;
	};

	Property<QString> Name;
	Property<int> Age;

	User();

private:
	QString m_name;
	int m_age;
};

User::NameGetter::NameGetter(User* user) :
	m_user(user)
{}

QString User::NameGetter::get(const QString& field) const {
	Q_UNUSED(field);
	return m_user->m_name;
}

User::NameSetter::NameSetter(User* user) : m_user(user) {}

void User::NameSetter::set(const QString& value, QString& field) const {
	Q_UNUSED(field);
	m_user->m_name = value;
}

User::AgeGetter::AgeGetter(User* user) :
	m_user(user)
{}

int User::AgeGetter::get(const int& field) const {
	Q_UNUSED(field);
	return m_user->m_age;
}

User::AgeSetter::AgeSetter(User* user) :
	m_user(user)
{}

void User::AgeSetter::set(const int& value, int& field) const {
	Q_UNUSED(field);
	m_user->m_age = value;
}

User::User() {
	Name = Property<QString>::of(new NameGetter(this), new NameSetter(this));
	Age = Property<int>::of(new AgeGetter(this), new AgeSetter(this));
}

void TestProperty::testFunctorProperties() {
	User user;

	user.Name = "John Doe";
	user.Age = 42;

	QString name = user.Name;
	int age = user.Age;

	QCOMPARE(name, "John Doe");
	QCOMPARE(age, 42);
}

//------------------------------------------------------------------------------

class MethodPropertiesUser {
public:
	Property<QString> Name;
	Property<int> Age;

	MethodPropertiesUser();

	QString name() const;
	void setName(const QString& name);

	int age() const;
	void setAge(const int& age);

private:
	QString m_name;
	int m_age;
};

MethodPropertiesUser::MethodPropertiesUser() {
	Name = Property<QString>::of(this, &MethodPropertiesUser::name, &MethodPropertiesUser::setName);
	Age = Property<int>::of(this, &MethodPropertiesUser::age, &MethodPropertiesUser::setAge);
}

QString MethodPropertiesUser::name() const {
	return m_name;
}

void MethodPropertiesUser::setName(const QString& name) {
	m_name = name;
}

int MethodPropertiesUser::age() const {
	return m_age;
}

void MethodPropertiesUser::setAge(const int& age) {
	m_age = age;
}

void TestProperty::testMethodProperties() {
	MethodPropertiesUser user;

	user.Name = "John Doe";
	user.Age = 42;

	QString name = user.Name;
	int age = user.Age;

	QCOMPARE(name, "John Doe");
	QCOMPARE(age, 42);
}

//------------------------------------------------------------------------------

class ReadOnlyUser {
public:
	class FirstNameGetter : public Getter<QString> {
	public:
		QString get(const QString& field) const;
	};

	Property<QString> FirstName;
	Property<QString> LastName;
	Property<QString> Email;
	Property<QString> Age;

	ReadOnlyUser();

	QString email() const;

	static QString age();
};

QString ReadOnlyUser::FirstNameGetter::get(const QString& field) const {
	Q_UNUSED(field);
	return "John";
}

ReadOnlyUser::ReadOnlyUser() {
	FirstName = Property<QString>::readOnlyOf(new FirstNameGetter());
	LastName = Property<QString>::readOnlyOf("Doe");
	Email = Property<QString>::readOnlyOf(this, &ReadOnlyUser::email);
	Age = Property<QString>::readOnlyOf(&ReadOnlyUser::age);
}

QString ReadOnlyUser::email() const {
	return "user@example.com";
}

QString ReadOnlyUser::age() {
	return "42";
}

void TestProperty::testReadOnlyProperties() {
	ReadOnlyUser user;

	QCOMPARE(user.FirstName, "John");
	QCOMPARE(user.LastName, "Doe");
	QCOMPARE(user.Email, "user@example.com");
	QCOMPARE(user.Age, "42");
}

QTEST_APPLESS_MAIN(TestProperty)

#include "tst_testproperty.moc"
