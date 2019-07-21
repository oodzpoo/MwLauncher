/*
 * Example to connect to MariaDB(MySQL)
 */
#include <iostream>
#include <mysql/mysql.h>  // require libmysqlclient-dev
//#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>		// getopt
#include <sys/stat.h>	// umask
#include "TaskScheduler.h"

using namespace std;

/*
 * [CLASS] Process
 */
class Proc
{
	const char* MY_HOSTNAME;
	const char* MY_DATABASE;
	const char* MY_USERNAME;
	const char* MY_PASSWORD;
	const char* MY_SOCKET;
	enum {
		MY_PORT_NO = 3306,
		MY_OPT = 0
	};
	MYSQL     *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;

public:
	Proc();           // Constructor
	bool execMain();  // Main Process
};

/*
 * Proc - Constructor
 */
Proc::Proc()
{
	// Initialize constants
	MY_HOSTNAME = "192.168.0.11";
	MY_DATABASE = "adms_api";
	MY_USERNAME = "adms_api";
	MY_PASSWORD = "adms_api1!";
	MY_SOCKET = NULL;

}

/*
 * Main Process
 */
bool Proc::execMain()
{
	try {
		// Format a MySQL object
		conn = mysql_init(NULL);

		// Establish a MySQL connection
		if (!mysql_real_connect(
			conn,
			MY_HOSTNAME, MY_USERNAME,
			MY_PASSWORD, MY_DATABASE,
			MY_PORT_NO, MY_SOCKET, MY_OPT)) {
			cerr << mysql_error(conn) << endl;
			return false;
		}

		// Execute a sql statement
		if (mysql_query(conn, "SHOW TABLES")) {
			cerr << mysql_error(conn) << endl;
			return false;
		}

		// Get a result set
		res = mysql_use_result(conn);

		// Fetch a result set
		cout << "* MySQL - SHOW TABLES in `"
			<< MY_DATABASE << "`" << endl;
		while ((row = mysql_fetch_row(res)) != NULL)
			cout << row[0] << endl;

		// Release memories
		mysql_free_result(res);

		// Close a MySQL connection
		mysql_close(conn);
	}
	catch (char *e) {
		cerr << "[EXCEPTION] " << e << endl;
		return false;
	}
	return true;
}

int SetDaemonProc() {
	pid_t iPid; 
	int iRet = 1;
	char szCwd[256];


	iPid = fork();
	switch (iPid) {
	case -1: 
		perror("The fork failed!");
		iRet = 0;
		break;

	case 0: 
		printf("Child Proc Start!!\n");
		setsid(); 
		getcwd(szCwd, sizeof(szCwd));
		chdir(szCwd); 
		umask(0); 
		iRet = 1;
		break;

	default: 
		iRet = 0;
		break;
	}

	return iRet;
}

TaskScheduler tasksc;

/*
 * Execution
 */
int main(int argc, char** argv) {
	int iCh;
	int iMode = 1;

	while ((iCh = getopt(argc, argv, "hdsv")) != EOF) {
		switch (iCh) {
		case 'h': //
			return -1;
		case 'd':
			iMode = 0;
			break;
		case 's':
			return -1;
		case 'v': // 
			printf("[VERSION %d.%d] BUILD [%s %s].\n", 1,
				0, __DATE__, __TIME__);
			return -1;
		default:
			break;
		}
	}

	// Debug Mode? Deamon Mode?
	if (iMode == 1) {
		if (0 == SetDaemonProc()) {
			exit(0);
		}
	}

	tasksc.TaskChecking();

	try {
		Proc objMain;
		bool bRet = objMain.execMain();
		if (!bRet) cout << "ERROR!" << endl;
	}
	catch (char *e) {
		cerr << "[EXCEPTION] " << e << endl;
		return 1;
	}
	return 0;
}