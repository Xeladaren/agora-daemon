
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QObject>

class SendMsg : public QObject
{

	Q_OBJECT

	public :

		void sendMsg(QString msg) ;

	private :

		QNetworkAccessManager nam ;
		QNetworkReply * reply ;

	public slots :

		void replyFinished(void) ;
	
};