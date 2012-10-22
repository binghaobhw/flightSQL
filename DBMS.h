/* 
 * File:   DBMS.h
 * Author: Administrator
 *
 * Created on 2012年10月15日, 下午2:49
 */

#ifndef DBMS_H
#define	DBMS_H

#include <direct.h>
#include <iostream>
#include <fstream>
#include <io.h>
#include <string>
#include <vector>
#include <algorithm>
#include "linkedlist.h"
//#include <boost/regex.hpp>

#define NOTUSE 1
#define TABLEEXISTED 2
#define NOTEXIST 3
#define DBEXISTED 4
#define UNKNOWN 5
#define maxReadSize 200      //max read piece

using namespace std;

class DBMS {
public:
    DBMS();
    DBMS(const DBMS& orig);
    virtual ~DBMS();
    
    string Replace(string src, string symbol, string r);
    string GetWholeName(string suffix);
    bool IsDigit(string str);
    string Parse(string words);
    void SetDirPath(string path);
    void ParseSelect(string sentence, string &select);
    void ParseFrom(string sentence, string &from);
    void ParseInsert(string sentence, string& tableFileName, int& tableSize, 
                     paralist* list, int PrimaryKeyIndex[], int& PrimaryKeySize);
    void ParseInsert(string words, string& table, vector<string>& value);
    void GetTableInfo(string tableName);
    void ReadModel(string tableName, string& content);
    void WriteModel(string& content, int mode);
    
    int CreateDatabase(string databaseName);
    int DropDatabase(string databaseName);
    void ShowDatabases();
    void Use(string databaseName);
    int CreateTable(string sentence);
    int DropTable(string tableName);
    void Describe(string tableName);
    void ShowTables();
    void Select(string sentence);
    void Projection(char* tableFileName, int tableSize, int* positionList, 
                    int* valueSize, int list_size);
    void Insert(string words);
    void Equi_Join( char* tableName1, char* tableName2, char* Col1, char* Col2);

private:
    string dirPath;
    string currentDb;
    
    string currentTable;
    vector<string> column;
    vector<int> size;  //
    vector<int> type;
    vector<int> position;
    vector<int> key;
    int tupleLength;
    
};

#endif	/* DBMS_H */

