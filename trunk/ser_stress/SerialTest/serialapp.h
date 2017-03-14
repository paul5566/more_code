#ifndef SERIALAPP_H
#define SERIALAPP_H

#include <QSerialPort>
#include <QDebug>
#include <QObject>
#include <QByteArray>

#define DATALEN	1024

class SerialApp : public QObject
{
    Q_OBJECT
public:
    explicit SerialApp(QString portName, qint32 baudRate, QObject *parent = 0);
    ~SerialApp();

signals:

public slots:
    void result(void);
	void writeHandle(void);
	void readHandle(void);

private:
    QSerialPort *m_serial;

	QByteArray txbuf;
	QByteArray rxbuf;
	qint64 txlen;
	qint64 rxlen;
	int err;
	
	void initTxbuf(void);
	void writeAll(void);
	void compare(void);
};

#endif // SERIALAPP_H
