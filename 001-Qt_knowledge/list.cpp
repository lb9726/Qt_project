#include<QDebug>
int main(int argc,char *agrv[])
{
	QList<QString> list;
	{
		QString str("This is a test string");
		list<<str;
	}
	qDebug()<<list[0]<< "How are you! ";
	return 0;
}
