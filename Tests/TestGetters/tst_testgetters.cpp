#include <QtTest>

#include "Getter.h"

class TestGetters : public QObject {
	Q_OBJECT

private slots:
	void testDefaultGetter();
	void testClassGetter();
	void testStaticClassGetter();
	void testStaticGetter();
};

//------------------------------------------------------------------------------

void TestGetters::testDefaultGetter() {
	Getter<int>* intGetter = GetterFactory<int>::DEFAULT;
	Getter<QString>* strGetter = GetterFactory<QString>::DEFAULT;

	QCOMPARE(intGetter->get(42), 42);
	QCOMPARE(strGetter->get("Hello Getter!"), "Hello Getter!");
}

//------------------------------------------------------------------------------

void TestGetters::testClassGetter() {
	class C {
	public:
		int getInt() const {
			return 42;
		}

		QString getString() const {
			return "Hello Getter!";
		}
	} c;

	Getter<int>* intGetter = GetterFactory<int>::of(&c, &C::getInt);
	Getter<QString>* strGetter = GetterFactory<QString>::of(&c, &C::getString);

	QCOMPARE(intGetter->get(0), 42);
	QCOMPARE(strGetter->get(""), "Hello Getter!");
}

//------------------------------------------------------------------------------

void TestGetters::testStaticClassGetter() {
	class C {
	public:
		static int getInt() {
			return 42;
		}

		static QString getString() {
			return "Hello Getter!";
		}
	};

	Getter<int>* intGetter = GetterFactory<int>::of(&C::getInt);
	Getter<QString>* strGetter = GetterFactory<QString>::of(&C::getString);

	QCOMPARE(intGetter->get(0), 42);
	QCOMPARE(strGetter->get(""), "Hello Getter!");
}

//------------------------------------------------------------------------------

int getInt() {
	return 42;
}

QString getString() {
	return "Hello Getter!";
}

void TestGetters::testStaticGetter() {
	Getter<int>* intGetter = GetterFactory<int>::of(&::getInt);
	Getter<QString>* strGetter = GetterFactory<QString>::of(&::getString);

	QCOMPARE(intGetter->get(0), 42);
	QCOMPARE(strGetter->get(""), "Hello Getter!");
}

QTEST_APPLESS_MAIN(TestGetters)

#include "tst_testgetters.moc"
