#include <QtTest>

#include "Getter.h"

class TestGetters : public QObject {
	Q_OBJECT

private slots:
	void testDefaultGetter();
	void testClassGetter();
	void testStaticClassGetter();
	void testStaticGetter();
	void testConstGetter();
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

	int intStub;
	QString strStub;

	QCOMPARE(intGetter->get(intStub), 42);
	QCOMPARE(strGetter->get(strStub), "Hello Getter!");
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

	int intStub;
	QString strStub;

	QCOMPARE(intGetter->get(intStub), 42);
	QCOMPARE(strGetter->get(strStub), "Hello Getter!");
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

	int intStub;
	QString strStub;

	QCOMPARE(intGetter->get(intStub), 42);
	QCOMPARE(strGetter->get(strStub), "Hello Getter!");
}

//------------------------------------------------------------------------------

void TestGetters::testConstGetter() {
	Getter<int>* intGetter = GetterFactory<int>::of(42);
	Getter<QString>* strGetter = GetterFactory<QString>::of("Hello Getter!");

	int intStub;
	QString strStub;

	QCOMPARE(intGetter->get(intStub), 42);
	QCOMPARE(strGetter->get(strStub), "Hello Getter!");
}

QTEST_APPLESS_MAIN(TestGetters)

#include "tst_testgetters.moc"
