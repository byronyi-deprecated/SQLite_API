#include "SQLite\sqlite3.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    char* file = "";
    sqlite3* db = NULL;
    int rc = 0;
    if(argc > 1)
        file = argv[1];

    sqlite3_initialize();
    rc = sqlite3_open_v2(file, &db, SQLITE_OPEN_READWRITE |
                         SQLITE_OPEN_CREATE, NULL);

    if(rc == SQLITE_NOTADB)
        cout << "Not a database!" << endl;
    if(rc != SQLITE_OK) {
        sqlite3_close(db);
        return -1;
    }

    sqlite3_stmt* stmt = NULL;

    sqlite3_prepare_v2(db, "SELECT count(*) FROM sqlite_master WHERE name = (:name)", -1, &stmt, NULL);

    int idx = sqlite3_bind_parameter_index(stmt, ":name");
    sqlite3_bind_text(stmt, idx, "peptide", -1, SQLITE_STATIC);

    sqlite3_step(stmt);
    cout << sqlite3_column_int(stmt, 0);

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

    sqlite3_bind_text(stmt, idx, "peaklist", -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    cout << sqlite3_column_int(stmt, 0);

//    while(sqlite3_step(stmt) == SQLITE_ROW) {
//        const char* data = (const char*) (sqlite3_column_text(stmt, 0));
//        cout << (data ? data : "[NULL]") << endl;
//    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);
    sqlite3_shutdown();
    return 0;
}

