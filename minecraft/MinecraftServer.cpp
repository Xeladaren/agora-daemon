
#include "minecraft/MinecraftServer.hpp"

#include <stdio.h>
#include <stdlib.h>


#include <QDebug>

MinecraftServer::MinecraftServer()
{

	this->nextBackup = QDateTime::currentDateTime() ;
	this->nextBackup = this->nextBackup.addDays(1);
	this->nextBackup.setTime(QTime(4, 30));

	qDebug() << "next backup : " << this->nextBackup ;

	this->reboot = true ;
	this->makeBackup = false ;

	this->playerList.clear() ;

	this->backupTimer = new QTimer() ;

	this->backupTimer->setInterval(60*1000) ;
	this->backupTimer->start() ;

	connect(this->backupTimer, 
			SIGNAL(timeout()),
			this,
			SLOT(checkBackup()));

	connect(	this, SIGNAL(minecraftSendCmd(QString)),
				this, SLOT(minecraftWrite(QString)));

	emit playerMsg("**Démarrage du serveur en cours ...**") ;

	startMinecraft() ;	
}	



MinecraftServer::~MinecraftServer()
{

}

void MinecraftServer::startMinecraft() 
{

	this->playerList.clear() ;

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

void MinecraftServer::minecraftStateChange(QProcess::ProcessState newState)
{
	//qDebug() << newState ;
	//qDebug() << this->minecraftProcess->exitStatus() ;

	if (newState == QProcess::NotRunning )
	{

		this->minecraftStarted = false ;

		if (reboot)
		{

			emit playerMsg("**Redémarrage du serveur en cours ...**") ;

			startMinecraft() ;
		}
		else
		{
			qDebug("EXIT");
		}
		
	}
}

void MinecraftServer::minecraftErrorOccurred(QProcess::ProcessError error) 
{
	qDebug() << "error : " << error ;
}

void MinecraftServer::minecraftReadyRead()
{

	QTextStream out(stdout) ;

	QByteArray readOut = this->minecraftProcess->readAll() ;

	QList<QByteArray> commandList = readOut.split('\n') ;

	for (int i = 0; i < commandList.count(); ++i)
	{
		QString command = commandList.at(i) ;

		if (command != "")
		{
			out << command << '\n' ;

			this->decodeMinecraftConsole(command) ;
		}
	}
}

void MinecraftServer::decodeMinecraftConsole(QString command)
{

	if (command.contains("[Server thread/INFO]: Done"))
	{
		qDebug() << "minecraft Started !!" ;
		this->playerList.clear() ;
		emit playerMsg("**Serveur On**") ;
		emit updatePalyerList(this->playerList) ;
		this->minecraftStarted = true ;
	}

	else if (command.contains("[Server thread/INFO]: <"))
	{
		
		
		int startPseudo = command.indexOf('<') + 1;
		int endPseudo = command.indexOf('>');
		int sizePseudo = endPseudo - startPseudo ;

		if (command.count() > (endPseudo+1))
		{

			QString pseudo = command.mid(startPseudo, sizePseudo) ;
			QString pmsg   = command.mid(endPseudo+1) ;

			QString msgModel = "**%1** :%2";

			QString msg = msgModel.arg(pseudo).arg(pmsg);

			emit playerMsg(msg) ;

		}

	}
	else if (command.contains("joined the game"))
	{
		int endPseudo = command.indexOf("joined") - 1;
		int startPseudo = command.lastIndexOf("INFO]:", endPseudo) + 6;
		int sizePseudo = endPseudo - startPseudo - 1 ;

		if (command.count() > (startPseudo+1+sizePseudo))
		{

			QString pseudo = command.mid(startPseudo+1, sizePseudo) ;

			this->playerList.append(pseudo) ;

			QString msgModel = "**%1 a rejoint le jeu.**";

			QString msg = msgModel.arg(pseudo);

			emit playerMsg(msg) ;

			emit updatePalyerList(this->playerList) ;
		}
		else
		{
			qDebug() << "Erreur de taille pour la connection" ;
		}
	}
	else if (command.contains("left the game"))
	{
		int endPseudo = command.indexOf("left") - 1;
		int startPseudo = command.lastIndexOf("INFO]:", endPseudo) + 6;
		int sizePseudo = endPseudo - startPseudo - 1 ;

		if (command.count() > (startPseudo+1+sizePseudo))
		{
			QString pseudo = command.mid(startPseudo+1, sizePseudo) ;

			this->playerList.removeAll(pseudo) ;

			QString msgModel = "**%1 a quitté le jeu**";

			QString msg = msgModel.arg(pseudo);

			emit playerMsg(msg) ;

			emit updatePalyerList(this->playerList) ;
		}
		else
		{
			qDebug() << "Erreur de taille pour la déconnection" ;
		}

	}
}

void MinecraftServer::minecraftWrite(QString cmd)
{
	//qDebug() << "test " << cmd ;
	if (this->minecraftStarted)
	{
		QByteArray data = cmd.toLocal8Bit() ;
		data.append('\n');
		this->minecraftProcess->write(data);
	}

}

void MinecraftServer::checkBackup() 
{
	
	if (this->nextBackup < QDateTime::currentDateTime() )
	{
		qDebug() << "backup timer" ;

		this->nextBackup = this->nextBackup.addDays(1);

		runBackup() ;

		qDebug() << "next backup : " << this->nextBackup ;
	}
}

void MinecraftServer::runBackup()
{
	QProcess backupProcess ;

	backupProcess.setWorkingDirectory("./server/") ;
	backupProcess.setProgram("./backup.sh") ;

	qDebug() << "start backup" ;

	emit minecraftSendCmd("say Debut de backup");
	emit minecraftSendCmd("save-all");
	emit minecraftSendCmd("save-off");

	backupProcess.start() ;

	backupProcess.waitForFinished(5*60*1000);

	emit minecraftSendCmd("save-on");
	emit minecraftSendCmd("say Fin de backup");

	qDebug() << "backup finich" ;

	this->makeBackup = false ;
}

void MinecraftServer::stopDelay()
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

void MinecraftServer::run()
{	

	//SendMsg * sendMsgObj = new SendMsg() ;

	while(1)
	{	
		int maxSize = 200 ;
		char command[maxSize] ;
		memset(command, '\0', maxSize);

		int i = 0 ;
		while(i < maxSize-1 )
		{

			char c = getchar() ;

			if (c != '\n')
			{
				command[i] = c ;
			}
			else
			{
				command[i] = '\0' ;
				break;
			}

			i++;
			
		}

		if (i == 0)
		{
			continue ;
		}

		QString string = command ;

		if (string.startsWith("!"))
		{
			qDebug() << "Daemon command" ;

			if (string.startsWith("!stop-daemon"))
			{
				if (!string.contains("now"))
				{
					stopDelay();
				}

				qDebug() << "STOP DAEMON" ;

				emit playerMsg("**Arrêt du serveur**") ;
			
				emit minecraftSendCmd("stop");
				reboot = false ;
			}

			else if (string.startsWith("!backup"))
			{

				qDebug() << "RUN BACKUP" ;

				runBackup() ;
			}
		}
		else
		{
			emit minecraftSendCmd(string) ;
		}

	}

}