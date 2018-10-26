#include <QtTest>

#include "Setter.h"

class TestSetter : public QObject {
	Q_OBJECT

private slots:
	void testDefaultSetter();
	void testClassSetter();
	void testStaticClassSetter();
	void testStaticSetter();
};

//------------------------------------------------------------------------------

void TestSetter::testDefaultSetter() {
	Setter<int>* intSetter = SetterFactory<int>::DEFAULT;
	Setter<QString>* strSetter = SetterFactory<QString>::DEFAULT;

	int intField = 0;
	QString strField = "";

	intSetter->set(42, intField);
	strSetter->set("Hello Setter!", strField);

	QCOMPARE(intField, 42);
	QCOMPARE(strField, "Hello Setter!");
}

//------------------------------------------------------------------------------

void TestSetter::testClassSetter() {
	class C {
	public:
		C() : m_int(0) {}
		int integer() const {
			return m_int;
		}
		QString string() const {
			return m_str;
		}
		void setInt(const int& value) {
			m_int = value;
		}
		void setString(const QString& str) {
			m_str = str;
		}
	private:
		int m_int;
		QString m_str;
	} c;

	Setter<int>* intSetter = SetterFactory<int>::of(&c, &C::setInt);
	Setter<QString>* strSetter = SetterFactory<QString>::of(&c, &C::setString);

	int intStub;
	QString stringStub;

	intSetter->set(42, intStub);
	strSetter->set("Hello Setter!", stringStub);

	QCOMPARE(c.integer(), 42);
	QCOMPARE(c.string(), "Hello Setter!");
}

//------------------------------------------------------------------------------

class C {
public:
	static int m_int;
	static QString m_str;

	static void setInt(const int& value) {
		m_int = value;
	}
	static void setString(const QString& str) {
		m_str = str;
	}
};

int C::m_int = 0;
QString C::m_str = "";

void TestSetter::testStaticClassSetter() {
	Setter<int>* intSetter = SetterFactory<int>::of(&C::setInt);
	Setter<QString>* strSetter = SetterFactory<QString>::of(&C::setString);

	C::m_int = 42;
	C::m_str = "";

	int intStub;
	QString stringStub;

	intSetter->set(42, intStub);
	strSetter->set("Hello Setter!", stringStub);

	QCOMPARE(C::m_int, 42);
	QCOMPARE(C::m_str, "Hello Setter!");
}

//------------------------------------------------------------------------------

static int globalInt;
static QString globalString;

void setGlobalInt(const int& value) {
	globalInt = value;
}

void setGlobalString(const QString& value) {
	globalString = value;
}

void TestSetter::testStaticSetter() {
	Setter<int>* intSetter = SetterFactory<int>::of(&::setGlobalInt);
	Setter<QString>* strSetter = SetterFactory<QString>::of(&::setGlobalString);

	globalInt = 0;
	globalString = "";

	int intStub;
	QString stringStub;

	intSetter->set(42, intStub);
	strSetter->set("Hello Setter!", stringStub);

	QCOMPARE(globalInt, 42);
	QCOMPARE(globalString, "Hello Setter!");
}

QTEST_APPLESS_MAIN(TestSetter)

#include "tst_testsetter.moc"
