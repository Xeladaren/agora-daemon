
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class SendMsg
{

	public :

		void sendMsg(QString msg) ;

	private :

		QNetworkAccessManager nam ;

		QNetworkReply * reply ;

	public slots :

		void replyFinish() ;
	
};