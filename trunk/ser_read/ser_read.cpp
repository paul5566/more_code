#include <stdio.h>
#include <QtSerialPort/QSerialPort>
#include <QTextStream>
#include <QCoreApplication>
#include <QStringList>

QT_USE_NAMESPACE	//  use serial name space

int main(int argc, char **argv)
{
	int baud_rate;
	char *tmp;
	QCoreApplication coreApplication(argc, argv);
	QStringList arg_list;
	QSerialPort ser_port;
	QString ser_port_name;
	QByteArray rxbuf;

	arg_list = QCoreApplication::arguments();   // fill in argument list
	if(argc < 2){
		printf("Usage : %s <Serial Port Name> [BaudRate]\n", qPrintable(arg_list.first()));
		return -1;
    }

	rxbuf = ser_port.readAll();			// set rxbuf to read buf
	baud_rate = (argc > 2)? arg_list.at(2).toInt() : QSerialPort::Baud9600;
	ser_port_name = arg_list.at(1);
	
	ser_port.setPortName(ser_port_name);	// set port name
	ser_port.setBaudRate(baud_rate);		// set baud-rate

	if(!ser_port.open(QIODevice::ReadOnly)){	// open port (readOnly)
		printf("Fail to open port %s, error %s\n", qPrintable(ser_port_name), qPrintable(ser_port.errorString()));
		return -1;
	}
		
	/*
	 * Blocks until new data is available for reading and the readyRead() signal has been emitted
	 * , or until msecs milliseconds have passed
	*/
	while(ser_port.waitForReadyRead(8000)){
		rxbuf.append(ser_port.readAll());
	}

	if(ser_port.error() == QSerialPort::ReadError){
		printf("Failed to read from port %s, error %s\n", qPrintable(ser_port_name), qPrintable(ser_port.errorString()));
		return -1;
	}else if(ser_port.error() == QSerialPort::TimeoutError && rxbuf.isEmpty()){
		printf("Timeout !! no data was currently availabe for reading form port %s\n", qPrintable(ser_port_name));
		return 0;
	}

	printf("Data successfully received from port %s\n", qPrintable(ser_port_name));
	tmp = rxbuf.data();
	printf("Recv : %s\n", tmp);

	return 0;
}
