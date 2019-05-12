
#include <QThread>
#include <stdio.h>
#include <QProcess>
#include <QTextStream>
#include <QSocketNotifier>
#include <QTimer>
#include <QDateTime>

class MinecraftServer : public QThread
{

	Q_OBJECT

	public :

		MinecraftServer() ;
		~MinecraftServer() ;

	public slots :

		void minecraftStateChange(QProcess::ProcessState newState) ;
		void minecraftErrorOccurred(QProcess::ProcessError error) ;
		void minecraftReadyRead();
		void minecraftWrite(QString cmd);

		void checkBackup() ;

	signals :

		void minecraftSendCmd(QString cmd);

	private :

		void run() ;

		void startMinecraft() ;
		void runBackup() ;

		void stopDelay() ;

		QProcess * minecraftProcess ;

		bool reboot = true ;

		bool makeBackup = false ;

		bool minecraftStarted = false ;

		QTimer * backupTimer ;

		QDateTime nextBackup ;
	
};