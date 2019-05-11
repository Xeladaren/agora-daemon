#include "discord/SendMsg.hpp"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

void SendMsg::sendMsg(QString msg)
{

	qDebug() << "MSG : " << msg ;

	QJsonDocument postData ;
	QJsonObject root ;

	root.insert("content", QJsonValue(msg)) ;
	postData.setObject(root);

	QUrl requestUrl("https://google.com") ;

	//requestUrl.setPath("/api/1.0/data/z_data_5c938a5e40687c16523675c1/") ;

	qDebug() << "URL : " << msg ;

	QNetworkRequest request(requestUrl) ;

	QString authorizationHeader = "Bot NTI1MDQxNzA4MjU0MzYzNjcz.Dvw4tg.HdZ-BkSw__JJwcgeHFcMZNZ1ko0";

	//request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	//request.setRawHeader(QByteArray("Authorization"), authorizationHeader.toLocal8Bit()) ;

	

	qDebug() << postData.toJson() ;

	//this->reply = this->nam.post(request, string.toLocal8Bit());
	reply = nam.get(request);

	connect(this->reply, SIGNAL(finished()), this, SLOT(replyFinished(void)));

}

void SendMsg::replyFinished()
{
	qDebug() << "Reply finish" ;
}