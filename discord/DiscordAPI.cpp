#include "discord/DiscordAPI.hpp"

#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

DiscordAPI::DiscordAPI()
{
	this->getNewMsgTimer = new QTimer() ;

	this->getNewMsgTimer->setInterval(1000) ;
	this->getNewMsgTimer->start() ;

	connect(this->getNewMsgTimer, 
			SIGNAL(timeout()),
			this,
			SLOT(getNewMsg(void)));
}

void DiscordAPI::postNewMsg(QString msg)
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
	this->sendReply = nam.get(request);

	connect(this->sendReply, SIGNAL(finished()), this, SLOT(sendReplyFinished(void)));

}

void DiscordAPI::getNewMsg()
{

	QUrl requestUrl("https://discordapp.com/api/channels/525047514919993344/messages") ;

	if (this->lastMsgID != "")
	{
		QString fragment = "after=%1" ;
		fragment = fragment.arg(this->lastMsgID) ;

		requestUrl.setQuery(fragment) ;
	}

	QNetworkRequest request(requestUrl) ;

	QString authorizationHeader = "Bot NTI1MDQxNzA4MjU0MzYzNjcz.Dvw4tg.HdZ-BkSw__JJwcgeHFcMZNZ1ko0";

	request.setRawHeader(QByteArray("Authorization"), authorizationHeader.toLocal8Bit()) ;

	this->reciveReply = nam.get(request) ;

	connect(this->reciveReply, SIGNAL(finished()), this, SLOT(reciveReplyFinished(void)));


}

void DiscordAPI::sendReplyFinished(void)
{
	qDebug() << "send Reply finish" ;
}

void DiscordAPI::reciveReplyFinished(void) 
{

	QJsonDocument data = QJsonDocument::fromJson(reciveReply->readAll()) ;

	QJsonArray msgList = data.array() ;

	for (int i = msgList.count()-1; i >= 0 ; --i)
	{
		

		if (msgList.at(i).isObject())
		{
			QJsonObject msgJObj = msgList.at(i).toObject() ;

			QString msgPseudo, msgContent ;

			if (msgJObj.contains("id"))
			{
				QJsonValue idJVal = msgJObj.value("id") ;

				if (idJVal.isString())
				{
						this->lastMsgID = idJVal.toString() ;
				}
			}

			if (msgJObj.contains("author"))
			{
				QJsonValue autorJVal = msgJObj.value("author") ;

				if (autorJVal.isObject())
				{
					QJsonObject autorJObj = autorJVal.toObject() ;

					if (autorJObj.contains("bot"))
					{
						continue ;
					}
					else if (autorJObj.contains("username"))
					{
						QJsonValue usernameJVal = autorJObj.value("username") ;

						if (usernameJVal.isString())
						{
							msgPseudo = usernameJVal.toString() ;
						}
					}
					
				}
				
			}

			if (msgJObj.contains("content"))
			{
				QJsonValue contentJVal = msgJObj.value("content") ;

				if (contentJVal.isString())
				{
					msgContent = contentJVal.toString() ;
				}
			}

			QString minecraftMsgModel = "tellraw @a \"[\u00A79Discord\u00A7r]<%1> %2\"" ;

			QString minecraftMsg = minecraftMsgModel.arg(msgPseudo).arg(msgContent) ;

			emit sendMinecraftCmd(minecraftMsg) ;

			//qDebug() << minecraftMsg ;

		}

		
	}
}