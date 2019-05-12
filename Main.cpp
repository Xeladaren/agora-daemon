#include "Main.hpp"

Main::Main()
{
	minecraftServer = new MinecraftServer() ;

	discordAPI = new DiscordAPI() ;

	minecraftServer->start();

	connect(discordAPI, SIGNAL(sendMinecraftCmd(QString)), minecraftServer, SLOT(minecraftWrite(QString))) ;
}

Main::~Main()
{
	delete(minecraftServer) ;

	delete(discordAPI) ;
}