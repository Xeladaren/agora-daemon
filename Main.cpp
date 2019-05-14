#include "Main.hpp"

Main::Main()
{
	#if 1

	minecraftServer = new MinecraftServer() ;
	minecraftServer->start();

	#endif

	discordAPI = new DiscordAPI() ;

	connect(minecraftServer, SIGNAL(playerMsg(QString)), 
			discordAPI, SLOT(postNewMsg(QString)));

	connect(minecraftServer, SIGNAL(updatePalyerList(QList<QString>)), 
			discordAPI, SLOT(updatePlayerList(QList<QString>)));

	connect(discordAPI, SIGNAL(sendMinecraftCmd(QString)), 
			minecraftServer, SLOT(minecraftWrite(QString))) ;
}

Main::~Main()
{
	delete(minecraftServer) ;

	delete(discordAPI) ;
}