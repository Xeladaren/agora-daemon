
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QTimer>

#include <QObject>

class DiscordAPI : public QObject
{

	Q_OBJECT

	public :

		DiscordAPI();

	public slots :

		void postNewMsg(QString msg) ;

		void getNewMsg();

		void reciveReplyFinished(void) ;

		void sendReplyFinished(void) ;

	signals :

		void sendMinecraftCmd(QString cmd) ;

	private :

		QNetworkAccessManager nam ;

		QNetworkReply * sendReply ;
		QNetworkReply * reciveReply ;

		QTimer * getNewMsgTimer ;

		QString lastMsgID ;


	
};