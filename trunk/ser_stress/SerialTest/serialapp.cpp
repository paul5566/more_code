#include "serialapp.h"

SerialApp::SerialApp(QString portName, qint32 baudRate, QObject *parent) 
: QObject(parent)
{
    m_serial = new QSerialPort(this);
    
	m_serial->setPortName(portName);
	if(m_serial->open(QIODevice::ReadWrite)){
		m_serial->setBaudRate(baudRate);
		m_serial->setDataBits(QSerialPort::Data8);
		m_serial->setParity(QSerialPort::NoParity);
		m_serial->setStopBits(QSerialPort::OneStop);
		m_serial->setFlowControl(QSerialPort::NoFlowControl);
		qDebug() << "Open Serial port:" << portName << ", BaudRate " << baudRate;
    }
	
	initTxbuf();
	
	connect(m_serial, SIGNAL(readyRead()), this, SLOT(readHandle()));
	connect(m_serial, SIGNAL(readChannelFinished()), this, SLOT(result()));
	connect(m_serial, SIGNAL(bytesWritten(qint64)), this, SLOT(writeHandle()));

	writeAll();
}

SerialApp::~SerialApp()
{
    if(m_serial->isOpen()){
        qDebug() << "close serial port";
        m_serial->close();
    }
    delete m_serial;
}

void SerialApp::compare(void)
{
	for(int i = 0; i < rxbuf.size() -1; i++){
		if(qAbs((unsigned char)rxbuf.at(i+1) -
			(unsigned char )rxbuf.at(i)) != 1){
			if((unsigned char)rxbuf.at(i) != '!' &&
				(unsigned char)rxbuf.at(i) != '}'){
				err++;
//				printf("Data mess:(%d)<%c , %c>\n", i, rxbuf.at(i), rxbuf.at(i+1));
			}
		}
	}
}

void SerialApp::writeAll(void)
{
	qint64 total = 0;
	qint64 tmp = 0;
	
	do{
		tmp = m_serial->write(txbuf);
		total += tmp;
	}while(total != DATALEN);
	txlen += total;
}

void SerialApp::initTxbuf(void)
{
	int i;

	txbuf.resize(DATALEN);

	for(i = 0; i < DATALEN; i++){
		if(i == 0){
			txbuf[i] = '!';
		}else{
			txbuf[i] = txbuf[i-1] + 1;
			if(txbuf[i] > '}'){
				txbuf[i] = '!';
			}
		}
	}
	
	qDebug() << txbuf;

/*
	for(i = 0; i < DATALEN; i++){
		txbuf[i] = i;
	}
*/
	txlen = 0;
	rxlen = 0;
	err = 0;
}

void SerialApp::result(void)
{
    if(m_serial->isOpen()){
		compare();
		printf("txlen = %lld, rxlen = %lld, err = %d   \r", txlen, rxlen, err);
//		qDebug() << rxbuf;	
    }else{
        qDebug() << "No serial port availible";
    }
}

void SerialApp::writeHandle(void)
{
	if(m_serial->isOpen()){
		writeAll();
	}else{
		printf("No serial port availible\n");
	}
}

void SerialApp::readHandle(void)
{
	if(m_serial->isOpen()){
		rxbuf.resize(DATALEN);
		rxbuf = m_serial->readAll();

		do{
			rxbuf.append(m_serial->read(m_serial->bytesAvailable()));
		}while(m_serial->bytesAvailable() != 0);

		rxlen += rxbuf.size();
		emit m_serial->readChannelFinished();
	}else{
		printf("No serial port availible\n");
	}
}

		
