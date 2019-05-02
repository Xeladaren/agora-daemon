#include "discord/SendMsg.hpp"

#include <QDebug>

void SendMsg::sendMsg(QString msg)
{

	qDebug() << "send msg" ;

	QNetworkRequest request(QUrl("https://discordapp.com/api/channels/525047514919993344/messages")) ;

	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json") ;
	request.setRawHeader("Authorization", "Bot NTI1MDQxNzA4MjU0MzYzNjcz.Dvw4tg.HdZ-BkSw__JJwcgeHFcMZNZ1ko0") ;

	QString string = QString("{\"content\": \"%1\"}").arg("Test") ;
	

	this->nam.post(request, string.toLocal8Bit());

}