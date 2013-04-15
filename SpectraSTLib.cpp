#include "SpectraSTLib.hpp"

using namespace std;


SpectraSTLib::SpectraSTLib(vector<string>& impFileNames,
                           SpectraSTCreateParams* para) : m_libFileName(para->outputFileName)
{
    sqlite3* db = NULL;

    sqlite3_initialize();
    int rc = 0;
    rc = sqlite3_open_v2(m_libFileName, &db,
                         SQLITE_OPEN_CREATE, NULL);

    if(rc != SQLITE_OK) {
        sqlite3_close(db);
        cerr << "Cannot create database using give name" << endl;
        return;
    }

    sqlite3_stmt* stmt = NULL;

    sqlite3_prepare_v2(db, "CREATE TABLE peptides ("
                       "id INTEGER PRIMARY KEY NOT NULL,"
                       "name varchar(255),"
                       "mw real,"
                       "charge tinyint,"
                       "precursorMZ real,"
                       "status varchar(255),"
                       "fullName varchar(255),"
                       "comment varchar(65535),"
                       "numPeaks tinyint);",
                       -1, &stmt, NULL);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    sqlite3_prepare_v2(db, "CREATE TABLE peaklists("
                       "id INTEGER NOT NULL "
                       "REFERENCES peptides(id),"
                       "mz real NOT NULL,"
                       "intensity real,"
                       "annotations varchar(255),"
                       "info varchar(255),"
                       "PRIMARY KEY(id, mz);",
                       -1, &stmt, NULL);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    import(impFileNames, db);

    sqlite3_close(db);
    sqlite3_shutdown();
}

SpectraSTLib::SpectraSTLib(string libFileName,
                           SpectraSTSearchParams* para,
                           bool loadPeptideIndex)
{

}

SpectraSTLib::~SpectraSTLib()
{

}

void SpectraSTLib::insertEntry(SpectraSTLibEntry *)
{

}

void SpectraSTLib::retrieve(vector<SpectraSTLibEntry*>& hits,
                            double lowMz,
                            double highMz)
{
    sqlite3* db = NULL;
    int rc = 0;

    sqlite3_initialize();
    rc = sqlite3_open_v2(m_libFileName, &db,
                         SQLITE_OPEN_READONLY, NULL);

    if(rc != SQLITE_OK) {
        sqlite3_close(db);
        cerr << "Cannot open the given library." << endl;
        return;
    }

    sqlite3_stmt* peptide_stmt = NULL;
    sqlite3_stmt* peak_stmt = NULL;

    sqlite3_prepare_v2(db, "SELECT id, name, charge,"
                       "precursorMZ, status, comment,"
                       "numPeaks FROM peptides;",
                       -1, &peptide_stmt, NULL);

    sqlite3_prepare_v2(db, "SELECT mz, intensity,"
                       "annotations, info FROM "
                       "peptides WHERE id = (:id) "
                       "AND mz BETWEEN (:lowMz) "
                       "AND (:highMz);",
                       -1, &peak_stmt, NULL);

    int id_idx = sqlite3_bind_parameter_index(peak_stmt, ":id");
    int lowMz_idx = sqlite3_bind_parameter_index(peak_stmt, ":lowMz");
    int highMz_idx = sqlite3_bind_parameter_index(peak_stmt, ":highMz");

    sqlite3_bind_double(peak_stmt, lowMz_idx, lowMz);
    sqlite3_bind_double(peak_stmt, highMz_idx, highMz);

    while(sqlite3_step(peptide_stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(peptide_stmt, 0);

        char* raw_name = sqlite3_column_text(peptide_stmt, 1);
        string name(raw_name);

        int parentCharge = sqlite3_column_int(peptide_stmt, 2);
        double parentMz = sqlite3_column_double(peptide_stmt, 3);

        char* raw_status = sqlite3_column_text(peptide_stmt, 4);
        string status(raw_status);

        char* raw_comments = sqlite3_column_text(peptide_stmt, 5);
        string comments(raw_comments);

        unsigned int numPeaks = sqlite3_column_int(peptide_stmt, 6);

        SpectraSTPeakList* newPeaklist = new SpectraSTPeakList(parentMz,
                                                        parentCharge,
                                                        numPeaks);

        sqlite3_bind_int(peak_stmt, id_idx, id);

        while(sqlite3_step(peak_stmt) == SQLITE_ROW)
        {
            double mz = sqlite3_column_double(peak_stmt, 0);
            float intensity = sqlite3_column_double(peak_stmt, 1);

            char* raw_annotations = sqlite3_column_text(peak_stmt, 2);
            string annotations(raw_annotations);

            char* raw_info = sqlite3_column_text(peak_stmt, 3);
            string info(raw_info);

            newPeaklist->insert(mz, intensity, annotations, info);
        }

        SpectraSTLibEntry* newLibEntry = new SpectraSTLibEntry(name, comments,
                                                               status, newPeaklist);

        hits.push_back(newLibEntry);
    }

    sqlite3_finalize(peptide_stmt);
    sqlite3_finalize(peak_stmt);

    sqlite3_close(db);
    sqlite3_shutdown();
    return;
}
