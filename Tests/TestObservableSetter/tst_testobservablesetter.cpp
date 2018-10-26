#include <QtTest>

#include "ObservableSetter.h"

class TestObservableSetter : public QObject {
	Q_OBJECT

private slots:
	void testDefaultObservableSetter();
	void testClassObservableSetter();
	void testStaticClassObservableSetter();
	void testStaticObservableSetter();
	void testReadOnlyObservableSetter();
};

//------------------------------------------------------------------------------

void TestObservableSetter::testDefaultObservableSetter() {
//	ObservableSetter<int>* intSetter = ObservableSetterFactory<int>::DEFAULT;
//	ObservableSetter<QString>* strSetter = ObservableSetterFactory<QString>::DEFAULT;

//	int intField = 0;
//	QString strField = "";

//	class ObserverNotifier : public ObserverNotifier<int>

//	intSetter->set(42, intField, );
//	strSetter->set("Hello Setter!", strField);

//	QCOMPARE(intField, 42);
//	QCOMPARE(strField, "Hello Setter!");
}

//------------------------------------------------------------------------------

void TestObservableSetter::testClassObservableSetter() {

}

//------------------------------------------------------------------------------

void TestObservableSetter::testStaticClassObservableSetter() {

}

//------------------------------------------------------------------------------

void TestObservableSetter::testStaticObservableSetter() {

}

//------------------------------------------------------------------------------

void TestObservableSetter::testReadOnlyObservableSetter() {

}

QTEST_APPLESS_MAIN(TestObservableSetter)

#include "tst_testobservablesetter.moc"
