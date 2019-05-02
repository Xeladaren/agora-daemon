
#include "minecraft/TestCrash.hpp"

#include "discord/SendMsg.hpp"

#include <QDebug>

TestCrash::TestCrash()
{

}

TestCrash::~TestCrash()
{

}

void TestCrash::run()
{

	while(1)
	{

		SendMsg sendMsgObj ;

		sendMsgObj.sendMsg("test") ;

		qDebug() << "run" ;

		sleep(10) ;
	}

}