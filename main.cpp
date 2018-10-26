#include <QCoreApplication>
#include <QtDebug>



class Test {
public:
	QString str() const {
		return "Hello method!";
	}
};

QString str() {
	return "Hello global!";
}

int main(int argc, char *argv[]) {
	QCoreApplication a(argc, argv);

	Test test;

	const Getter<QString>& g1 = GetterFactory<QString>::DEFAULT;
	const Getter<QString>& g2 = GetterFactory<QString>::of(&test, &Test::str);
	const Getter<QString>& g3 = GetterFactory<QString>::of(&::str);

	qDebug() << g1.get("Arg");
	qDebug() << g2.get("Arg");
	qDebug() << g3.get("Arg");

	return a.exec();
}
