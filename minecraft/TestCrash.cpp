
#include "minecraft/TestCrash.hpp"

#include "discord/SendMsg.hpp"

#include <stdio.h>
#include <stdlib.h>


#include <QDebug>

TestCrash::TestCrash()
{

	this->nextBackup = QDateTime::currentDateTime() ;
	this->nextBackup = this->nextBackup.addDays(1);
	this->nextBackup.setTime(QTime(4, 30));

	qDebug() << "next backup : " << this->nextBackup ;

	this->reboot = true ;
	this->makeBackup = false ;

	this->backupTimer = new QTimer() ;

	this->backupTimer->setInterval(60*1000) ;
	this->backupTimer->start() ;

	connect(this->backupTimer, 
			SIGNAL(timeout()),
			this,
			SLOT(checkBackup()));

	connect(	this, SIGNAL(minecraftSendCmd(QString)),
				this, SLOT(minecraftWrite(QString)));

	startMinecraft() ;	
}	



TestCrash::~TestCrash()
{

}

void TestCrash::startMinecraft() 
{

	if (this->minecraftProcess != NULL)
	{
		qDebug() << "null" ;
		//delete(this->minecraftProcess) ;
	}

	this->minecraftProcess = new QProcess(this);

	this->minecraftProcess->setWorkingDirectory("./server/") ;
	this->minecraftProcess->setProgram("./start.sh") ;
	//this->minecraftProcess->setArguments({"test"}) ;

	// --------------

	connect(	this->minecraftProcess, 
				SIGNAL(stateChanged(QProcess::ProcessState)), 
				this, 
				SLOT(minecraftStateChange(QProcess::ProcessState))) ;

	connect(	this->minecraftProcess, 
				SIGNAL(errorOccurred(QProcess::ProcessError)), 
				this, 
				SLOT(minecraftErrorOccurred(QProcess::ProcessError))) ;

	connect(	this->minecraftProcess, 
				SIGNAL(readyReadStandardOutput()), 
				this, 
				SLOT(minecraftReadyRead())) ;

	this->minecraftProcess->start() ;
}

void TestCrash::minecraftStateChange(QProcess::ProcessState newState)
{
	//qDebug() << newState ;
	//qDebug() << this->minecraftProcess->exitStatus() ;

	if (newState == QProcess::NotRunning )
	{

		if (reboot)
		{
			if (this->makeBackup)
			{
				runBackup() ;
			}

			startMinecraft() ;
		}
		else
		{
			qDebug("EXIT");
			exit(0) ;
		}
		
	}
}

void TestCrash::minecraftErrorOccurred(QProcess::ProcessError error) 
{
	qDebug() << "error : " << error ;
}

void TestCrash::minecraftReadyRead()
{

	QTextStream out(stdout) ;

	QByteArray readOut = this->minecraftProcess->readAll() ;

	readOut.replace('\n', ' ');

	QString string = readOut ;

	out << string << '\n' ;
}

void TestCrash::minecraftWrite(QString cmd)
{
	//qDebug() << "test " << cmd ;
	QByteArray data = cmd.toLocal8Bit() ;
	data.append('\n');
	this->minecraftProcess->write(data);
}

void TestCrash::checkBackup() 
{
	
	if (this->nextBackup < QDateTime::currentDateTime() )
	{
		qDebug() << "backup timer" ;

		this->nextBackup = this->nextBackup.addDays(1);

		runBackup() ;

		qDebug() << "next backup : " << this->nextBackup ;
	}
}

void TestCrash::runBackup()
{
	QProcess backupProcess ;

	backupProcess.setWorkingDirectory("./server/") ;
	backupProcess.setProgram("./backup.sh") ;

	qDebug() << "start backup" ;

	backupProcess.start() ;

	backupProcess.waitForFinished(5*60*1000);

	qDebug() << "backup finich" ;

	this->makeBackup = false ;
}

void TestCrash::stopDelay()
{
	emit minecraftSendCmd("say Arret du serveur dans 1 min.");
	sleep(30) ;
	emit minecraftSendCmd("say Arret du serveur dans 30 s.");
	sleep(20);
	emit minecraftSendCmd("say Arret du serveur dans 10 s.");
	sleep(1);
	emit minecraftSendCmd("say 9");
	sleep(1);
	emit minecraftSendCmd("say 8");
	sleep(1);
	emit minecraftSendCmd("say 7");
	sleep(1);
	emit minecraftSendCmd("say 6");
	sleep(1);
	emit minecraftSendCmd("say 5");
	sleep(1);
	emit minecraftSendCmd("say 4");
	sleep(1);
	emit minecraftSendCmd("say 3");
	sleep(1);
	emit minecraftSendCmd("say 2");
	sleep(1);
	emit minecraftSendCmd("say 1");
	sleep(1);
	emit minecraftSendCmd("say Arret du serveur ...");
}

void TestCrash::run()
{	

	//SendMsg * sendMsgObj = new SendMsg() ;

	while(1)
	{	
		char command[200] ;

		scanf("%s", command);

		QString string = command ;
		
		//qDebug() << "command : " << string ;

		if (string == "!stop-daemon")
		{
			qDebug() << "STOP DAEMON" ;
			stopDelay();
			emit minecraftSendCmd("stop");
			reboot = false ;
		}
		else if (string == "!backup")
		{
			makeBackup = true ;
			stopDelay();
			emit minecraftSendCmd("stop");
		}
		else
		{
			emit minecraftSendCmd(string) ;
			//this->minecraftProcess->write()
		}

	}

}