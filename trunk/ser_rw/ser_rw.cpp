#include <unistd.h>
#include <QtSerialPort/QSerialPort>
#include <QTextStream>
#include <QCoreApplication>
#include <QStringList>

#define WDATA "Hello world"

QT_USE_NAMESPACE
QTextStream stdo(stdout);	// printf

int serial_open(QSerialPort *serial, QString serial_name, int baud)
{
	serial->setPortName(serial_name);	// set port name 

	if(!serial->open(QIODevice::ReadWrite)){	// open port (Read || Write)
		stdo << QObject::tr("Open %1 FAILED, error %2").arg(serial_name).arg(serial->errorString()) << endl;
		return -1;
	}
	/* Must be open first !! */
	serial->setBaudRate(baud);  // set baud rate
	serial->setStopBits(QSerialPort::OneStop);
	serial->setParity(QSerialPort::NoParity);
	serial->setDataBits(QSerialPort::Data8);
	serial->setFlowControl(QSerialPort::NoFlowControl);

	return 0;
}

int main(int argc, char **argv)
{
	qint64 txlen, rxlen;
	QCoreApplication coreApplication(argc, argv);
	QStringList arg_list;
	QSerialPort serial;
	QString serial_name;
	QByteArray txbuf;
	QByteArray rxbuf;	// read write buf

	arg_list = QCoreApplication::arguments();
	if(argc < 3){
		stdo << QObject::tr("Usage : %1 <Serial Port Name> [Baud Rate]").arg(arg_list.first()) << endl;
		return -1;
	}

	if(serial_open(&serial, arg_list.at(1), arg_list.at(2).toInt()) == -1){
		return -1;
	} 	

//	QByteArray txbuf(WDATA, sizeof(WDATA));	// set the write data
	txlen = 0;
	rxlen = 0;

	int i;
	txbuf.resize(1024);
    for(i = 0; i < 1024; i++){                                                                                                                                         
        if(i == 0){
            txbuf[i] = '!';
        }else{
            txbuf[i] = txbuf[i-1] + 1;
            if(txbuf[i] > '}'){
                txbuf[i] = '!';
            }
        }
    }


	do{	
		if(serial.isWritable()){
			txlen = serial.write(txbuf);
			if(txlen < 0){
				stdo << QObject::tr("Write Fail, error %1").arg(serial.errorString()) << endl;
				break;
			}else if(txlen !=txbuf.size()){
				stdo << QObject::tr("txlen (%1) != txbuf_size (%2)").arg(txlen).arg(txbuf.size()) << endl;
				break;
			}else if(!serial.waitForBytesWritten(3000)){
				stdo << QObject::tr("Write data timeout, error %1").arg(serial.errorString()) << endl;
				break;
			}
			stdo << QObject::tr("Write SUCCESS, txlen %1").arg(txlen) << endl;
		}
/*------------------------------------------ Write SUCCESS ---------------------------------------------------*/
		if(serial.isReadable()){
			rxbuf = serial.readAll();
			do{	// donot wait !
				rxbuf.append(serial.readAll());
			}while(serial.waitForReadyRead(1000));
			if(serial.error() == QSerialPort::ReadError){
				stdo << QObject::tr("Fail to read data, error %1").arg(serial.errorString());
				break;
			}else if(serial.error() == QSerialPort::TimeoutError && rxbuf.isEmpty()){
				stdo << QObject::tr("Read data timeout, error %1").arg(serial.errorString());
				break;
			}
			stdo << QObject::tr("ReadLen %1").arg(rxbuf.size()) << endl;
			printf("READ : %s\n", rxbuf.data());
			txlen = 0;	
		}
	}while(1);
	
	serial.close();
	return 0;
}
	
	



