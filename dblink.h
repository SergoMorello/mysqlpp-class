#include <mysql++.h>

using namespace std;
using namespace mysqlpp;

mutex mtxDblink;

class DBLink
{
	private:
		Connection conn;
        string Host;
        string Name;
        string User;
        string Pass;
        void OutError(string text)
        {
            cerr << text << endl;
        }
	public:
        DBLink(string DBHost,string DBName,string DBUser,string DBPass)
        {
            Host = DBHost;
            Name = DBName;
            User = DBUser;
            Pass = DBPass;
        }
		//Connect
		int connect()
		{
			try {
				conn.connect(Name.c_str(), Host.c_str(), User.c_str(), Pass.c_str());
			}
			catch (ConnectionFailed err)
			{
				string str_err(err.what());
				OutError("Error connect to mysql: "+str_err);
				return 0;
			}
			catch (BadQuery err)
			{
				string str_err(err.what());
				OutError("Error connect to mysql: "+str_err);
				return 0;
			}
			catch (std::exception & err) {
				string str_err(err.what());
				OutError("Error connect to mysql: "+str_err);
				return 0;
			}
			return 1;
		}
		
		//Disconnect
		void disconnect()
		{
			conn.disconnect();
		}
		
		//Stat connect
		int stat()
		{
			try {
				conn.query("SELECT VERSION();").store();
			}
			catch (BadQuery err) {
				return 0;
			}
			catch (std::exception & err) {
				return 0;
			}
			return 1;
		}
		
		//Get num
		int num(string query_str)
		{	
			unique_lock<mutex> ul(mtxDblink);
			StoreQueryResult queryres;
			if (!stat())
			{
				OutError("Lost connect to DB, reconnect");
				connect();
			}
			if(conn.connected())
			{
				try {
					queryres = conn.query(query_str.c_str()).store();
				}
				catch (BadQuery err) {
					string str_err(err.what());
					OutError("Error: "+str_err);
					return -1;
				}
				catch (std::exception & err) {
					string str_err(err.what());
					OutError("Error: "+str_err);
					return 0;
				}
				if(!queryres.empty())
				{
					return queryres.num_rows();		
				} 
				else
				{
					return 0;
				}
			}		
			else
			{
				return -1;
			}
			return 0;
		}
		
		//Get arr
		StoreQueryResult row(string query_str)
		{
			unique_lock<mutex> ul(mtxDblink);
			StoreQueryResult queryres;
			if (!stat())
			{
				OutError("Lost connect to DB, reconnect");
				connect();
			}
			if(conn.connected())
			{
				try {
					queryres = conn.query(query_str.c_str()).store();
				}
				catch (BadQuery err) {
					string str_err(err.what());
					OutError("Error: "+str_err);
				}
				catch (std::exception & err) {
					string str_err(err.what());
					OutError("Error: "+str_err);
				}
				if(!queryres.empty())
				{
					return queryres;
				} 	
			}	
			return queryres;
		}
		
		//Query
		int query(string query_str)
		{
			unique_lock<mutex> ul(mtxDblink);
			if (!stat())
			{
				OutError("Lost connect to DB, reconnect");
				connect();
			}
			if(conn.connected())
			{
				try {
					conn.query(query_str.c_str()).store();
				}
				catch (BadQuery err) {
					string str_err(err.what());
					OutError("Error: "+str_err);
					return 0;
				}
				catch (std::exception & err) {
					string str_err(err.what());
					OutError("Error connect to mysql: "+str_err);
					return 0;
				}
			}		
			else
			{
				return 0;
			}
			return 1;
		}
};
