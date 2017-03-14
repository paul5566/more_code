#include <stdio.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
//	ui(new Ui::MainWindow), serial()
	ui(new Ui::MainWindow), tester()
{
	ui->setupUi(this);
	// Start button connect
	connect(ui->startButton1, SIGNAL(clicked()), 
		this, SLOT(startButton_clicked()));
	// Close button connect
	connect(ui->closeButton1, SIGNAL(clicked()), 
		this, SLOT(closeButton_clicked()));
}

MainWindow::~MainWindow()
{
	closeSerialPort();
	delete ui;
}

int MainWindow::openSerialPort(QSerialPort *serial, struct port_info *pInfo)
{
	serial->setPortName(pInfo->name);

	if(serial->open(QIODevice::ReadWrite)){
		serial->setBaudRate(pInfo->BaudRate);
		serial->setDataBits(QSerialPort::Data8);
		serial->setParity(QSerialPort::NoParity);
		serial->setStopBits(QSerialPort::OneStop);
		serial->setFlowControl(QSerialPort::NoFlowControl);
	}else{
		QMessageBox::critical(this, "Error", serial->errorString());
		ui->statusBar->showMessage("Open port error");
		return -1;
	}
	
	ui->statusBar->showMessage(QString("Connected to %1, BaudRate %2.")
					.arg(pInfo->name).arg(pInfo->BaudRate));

	return 0;
}

void MainWindow::closeSerialPort(void)
{
//	serial->close();
	closePortStatus();
	ui->startButton1->setEnabled(true);
	ui->statusBar->showMessage("Closing connection....");
}


void MainWindow::startButton_clicked(void)
{
	struct port_info pInfo;
	QSerialPort serial;

	pInfo.BaudRate = ui->comboBox->currentText().toInt();
	pInfo.name = ui->comName1->text();
	
	if(openSerialPort(&serial, &pInfo) == -1){
		ui->startButton1->setEnabled(true);
	}else{
		ui->startButton1->setEnabled(false);
		openPortStatus();
		/* Start to VCOM Test */
		/* run com1Thread */
		thread = new QThread;
		tester = new Tester;
		connect(thread, SIGNAL(started()), tester, SLOT(startTest()));
		thread->start();
	}
}

void Tester::startTest(void)
{
	printf("FucK ********\n");
}

void MainWindow::closeButton_clicked(void)
{
	closeSerialPort();
}

Tester::Tester(void)
{
}

Tester::~Tester(void)
{
}


