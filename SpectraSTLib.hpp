#ifndef SPECTRASTLIB_HPP
#define SPECTRASTLIB_HPP

#include "SpectraSTLibEntry.hpp"
#include "SpectraSTCreateParams.hpp"
#include "SpectraSTSearchParams.hpp"
#include "SpectraSTLibImporter.hpp"
#include "SpectraSTPeakList.hpp"
#include "SQLite/sqlite3.h"
#include <vector>
#include <string>

using namespace std;

class SpectraSTLib
{
public:
    SpectraSTLib(vector<string>& impFileNames,
                 SpectraSTCreateParams*);
    SpectraSTLib(string libFileName,
                 SpectraSTSearchParams*,
                 bool loadPeptideIndex = false);

    ~SpectraSTLib();

    string getLibFileName() {return m_libFileName;}

    void insertEntry(SpectraSTLibEntry*);

    void retrieve(vector<SpectraSTLibEntry*>& hits,
                  double lowMz,
                  double highMz);

private:
    string m_libFileName;
};

#endif // SPECTRASTLIB_HPP
