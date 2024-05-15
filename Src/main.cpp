//
// 2024-5-15, jjuiddong
// - reference
//      - https://gist.github.com/larioandr/e4c77396ee2447181f809ffff1269ad3
//          - connect, insert, select
//      - https://stackoverflow.com/questions/16047986/how-to-insert-binary-data-into-a-postgresql-bytea-column-using-the-c-libpqxx-a
//          - insert bytea type (binary buffer)
//      - https://stackoverflow.com/questions/59826604/increase-the-update-speed-to-postgresql-database-via-libpqxx// 
//          - update
//
#include <iostream>
#include <pqxx/pqxx> 

using namespace std;


int main()
{
    try 
    {
        pqxx::connection c("postgres://postgres:1123@localhost/navi");

        if (c.is_open()) 
        {
            cout << "Opened database successfully: " << c.dbname() << endl;

            // insert, select
            if (0)
            {
                pqxx::work insert_trans(c);
                insert_trans.exec("INSERT INTO bound (id, level, x, y) VALUES ('2', '0', '1', '0')");
                insert_trans.commit();

                pqxx::work select_trans(c);
                pqxx::result r = select_trans.exec("SELECT * FROM bound");
                select_trans.commit();

                for (auto const& row : r) {
                    for (auto const& field : row) {
                        std::cout << field.c_str() << "\t";
                    }
                    std::cout << std::endl;
                }
            }

            // insert binary buffer
            if (1)
            {
                std::string name = "foo";
                unsigned char bin_data[128];
                for (int i = 0; i < sizeof(bin_data); ++i)
                    bin_data[i] = i;
                const size_t bin_size = sizeof(bin_data);

                // update binary buffer
                //pqxx::work update_trans(c);
                //pqxx::binarystring blob(bin_data, bin_size);                
                //pqxx::result r = 
                //    update_trans.exec_params("UPDATE bound SET data = $2 WHERE id = $1", 2, blob);
                //update_trans.commit();

                // get binary buffer
                pqxx::work select_trans(c);
                pqxx::result r2 = 
                    select_trans.exec_params("SELECT data FROM bound WHERE id = $1", 14139487057576);
                select_trans.commit();
                for (const auto& row : r2)
                {
                    pqxx::binarystring blob(row["data"]);
                    const pqxx::binarystring::value_type* ptr = blob.data();
                    size_t len = blob.size();
                    int a = 0;
                }

            }
        }
        else 
        {
            cout << "Can't open database" << endl;
            return 1;
        }

    }
    catch (const std::exception& e) 
    {
        //cerr << e.what() << std::endl;
        cout << e.what() << std::endl;
        return 1;
    }

}
