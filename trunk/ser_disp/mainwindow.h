#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QtCore>
#include <QMessageBox>
#include <QApplication>
#include <QTextStream>
#include <QString>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QThread>

#define TXDATALEN		1024
#define WRITEWAITTIME	1000
#define READWAITTIME	1000

namespace Ui{
	class MainWindow;
}

struct port_info{
	int BaudRate;
	QString name;
};

struct testResult{
	qint64 txlen;
	qint64 rxlen;
	int round;
};

class Tester;

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Ui::MainWindow *ui;
	QThread *thread;
	Tester *tester;
//	QSerialPort serial;		// Multi!?
/*	
	class Thread : public QThread
	{
	public:
		Thread(QSerialPort *serial);

		void run();	

	private:
		QSerialPort *serial;
	};
*/	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow(void);
	int openSerialPort(QSerialPort *serial, struct port_info *pInfo);
	void closeSerialPort(void);

	void openPortStatus(void);
	void closePortStatus(void);
	void portOKStatus(void);
	void portErrStatus(void);
	void updateResult(struct testResult *tRes);

	void simpleTest(QSerialPort *serial);

private slots:
	void startButton_clicked(void);
	void closeButton_clicked(void);
/*	
	void openPortStatus(void);
	void closePortStatus(void);
	void portOKStatus(void);
	void portErrStatus(void);
	void updateResult(struct testResult *tRes);
*/
};

class Tester : public QObject
{
	Q_OBJECT

public:
	Tester();
	~Tester();

signals:
	void start(void);
	void finished(void);

public slots:
	void startTest();
};
                                                                                                                                                                         
#endif
