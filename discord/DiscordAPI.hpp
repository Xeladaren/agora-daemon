
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QBuffer>

#include <QTimer>

#include <QObject>

class DiscordAPI : public QObject
{

	Q_OBJECT

	public :

		DiscordAPI();

		void pachChanelTopic(QString topic) ;

	public slots :

		void postNewMsg(QString msg) ;

		void getNewMsg();

		void reciveReplyFinished(void) ;

		void sendReplyFinished(void) ;

		void pachReplyFinished(void) ;

		void updatePlayerList(QList<QString> playerList) ;

	signals :

		void sendMinecraftCmd(QString cmd) ;

	private :

		QNetworkAccessManager nam ;

		QNetworkReply * sendReply ;
		QNetworkReply * reciveReply ;

		QNetworkReply * pachReply ;

		QTimer * getNewMsgTimer ;

		QString lastMsgID ;

		QList<QString> playerList ;

		QBuffer * patchBufferData ;
	
};