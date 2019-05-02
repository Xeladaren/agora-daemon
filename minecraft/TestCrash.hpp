
#include <QThread>

class TestCrash : public QThread
{

	Q_OBJECT

	public :

		TestCrash() ;
		~TestCrash() ;

	private :

		void run() ;
	
};