#include <QObject>

#include "minecraft/MinecraftServer.hpp"
#include "discord/DiscordAPI.hpp"

class Main : public QObject
{

	public:

		Main();

		~Main();

	private:

		MinecraftServer * minecraftServer ;

		DiscordAPI * discordAPI ;
	
};