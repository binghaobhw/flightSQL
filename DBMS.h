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
#include <list>
#include <algorithm>
#include "myh.h"

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
    void ParseSelect(string sentence, vector<string>& attribute, 
                     vector<string>& table, vector<string>& condition);
    void GetTableInfo(string tableName, int temp);
    void ReadModel(string modelPath, string& content);
    void WriteModel(string& content, int mode);
    void Compare(int processed, string tableName, vector<string>& col, 
                 vector<string>& op, vector<string>& value);
    
    int CreateDatabase(string databaseName);
    int DropDatabase(string databaseName);
    void ShowDatabases();
    void Use(string databaseName);
    int CreateTable(string sentence);
    int DropTable(string tableName);
    void Describe(string tableName);
    void ShowTables();
    void Select(string sentence);
    void Projection(string filePath, vector<string>& attribute);
    void Insert(string words);
    void Equi_Join(char* tableName1, char* tableName2, char* Col1, char* Col2);
    void Display();

private:
    string dirPath;
    string currentDb;
    
    string currentTable;
    vector<string> column;
    vector<int> size;  //
    vector<int> type;  //0-char 1-int
    vector<int> position;
    vector<int> key;
    int tupleLength;
    
};

#endif	/* DBMS_H */

