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

void DiscordAPI::pachChanelTopic(QString topic)
{
	QJsonDocument data ;
	QJsonObject root ;

	topic = topic + " | conseil: mettez ce salon en muet " ;

	root.insert("topic", QJsonValue(topic)) ;
	data.setObject(root);

	qDebug() << data ;

	QUrl requestUrl("https://discordapp.com/api/channels/525047514919993344") ;

	QNetworkRequest request(requestUrl) ;

	QString authorizationHeader = "Bot NTI1MDQxNzA4MjU0MzYzNjcz.Dvw4tg.HdZ-BkSw__JJwcgeHFcMZNZ1ko0";

	request.setRawHeader(QByteArray("Authorization"), authorizationHeader.toLocal8Bit()) ;
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	this->pachReply = nam.sendCustomRequest(request, "PATCH", data.toJson()) ;

	connect(this->pachReply, SIGNAL(finished()), this, SLOT(sendReplyFinished(void)));
}

void DiscordAPI::postNewMsg(QString msg)
{

	QJsonDocument data ;
	QJsonObject root ;

	root.insert("content", QJsonValue(msg)) ;
	data.setObject(root);

	QUrl requestUrl("https://discordapp.com/api/channels/525047514919993344/messages") ;

	QNetworkRequest request(requestUrl) ;

	QString authorizationHeader = "Bot NTI1MDQxNzA4MjU0MzYzNjcz.Dvw4tg.HdZ-BkSw__JJwcgeHFcMZNZ1ko0";

	request.setRawHeader(QByteArray("Authorization"), authorizationHeader.toLocal8Bit()) ;
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	this->sendReply = nam.post(request, data.toJson()) ;

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
	if (this->sendReply->error())
	{
		qDebug() << this->sendReply->error() ;
	}
}

void DiscordAPI::updatePlayerList(QList<QString> playerList)
{
	this->playerList = playerList ;

	QString topic ;

	if (this->playerList.count() < 2)
	{
		topic = QString("Joueur en ligne : %1 / 10").arg(this->playerList.count()) ;
	}
	else
	{
		topic = QString("Joueurs en ligne : %1 / 10").arg(this->playerList.count()) ;
	}

	this->pachChanelTopic(topic) ;
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