#include <iostream>
#include <string>

using namespace std;

string DBHost = "localhost";
string DBName = "db";
string DBUser = "user";
string DBPass = "pass";

#include "dblink.h"
DBLink dblink;

int main()
{
    if (!dblink.connect()) //if bad connect
        return 1;
    
    int num_item = dblink.num("SELECT id FROM table"); //get and show num items in table
    cout << "num my items in table: " << num_item << endl;
    
    int id_to_get = 77; //if var int
    
    StoreQueryResult data_item = dblink.row("SELECT val1,val2 FROM table WHERE id='+to_string(id_to_get)+'"); //get data
    if (!data_item.empty())
    {
        cout << "my data val1: " << data_item[0]["val1"] << " AND my data val2: " << data_item[0]["val2"] << endl;
    }
    
    StoreQueryResult data_item_for = dblink.row("SELECT val1,val2 FROM table"); //get data in the for
    if (!data_item_for.empty())
    {
        for(int i=0;i<data_item_for.size();i++)
        {
            cout << "my data val1: " << data_item_for[i]["val1"] << " AND my data val2: " << data_item_for[i]["val2"] << endl;
        }
    }
    
    string id_to_query = "77"; //if var string
    
    if (dblink.query("UPDATE table SET val1='1' WHERE id='+id_to_query+'")) //query to DB
        cout << "write OK!" << endl;
    
    dblink.disconnect(); //disconnect DB
	return 0;
}
