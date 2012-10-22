/* 
 * File:   main.cpp
 * Author: Administrator
 *
 * Created on 2012年10月15日, 下午2:48
 */

#include <cstdlib>
#include <direct.h>
#include <iostream>
#include <fstream>
#include <string>
#include "DBMS.h"

using namespace std;

/*
 * insert into course values(eng, 2)
 */

int main(int argc, char** argv) {
//    boost::regex reg(".*\\..*");
//    string hi("nihao teaid nihao");
//    boost::smatch matches;
//    if(boost::regex_match(hi, reg))
//        cout << "ok!" << endl;
    DBMS idbms;
    char in[256];
    string command;
    string::size_type i;
    const string exit = "exit";
    const string createTable = "create table ";
    const string createDatabase = "create database ";
    const string showDatabases = "show databases";
    const string use = "use ";
    const string describe = "describe ";
    const string dropDatabase = "drop database ";
    const string showTables = "show tables";
    const string dropTable = "drop table ";
    const string select = "select ";
    const string insert = "insert ";
    while(true) {
        cout << "flightSQL> ";
        cin.getline(in, 256, '\n');
        command.clear();
        command.insert(0, in);
        if(i = command.find(exit, 0) != string::npos) {
            return 0;
        }
        else if(i = command.find(createTable, 0) != string::npos) {
            idbms.CreateTable(command.substr(createTable.length()));
        }
        else if(i = command.find(createDatabase, 0) != string::npos) {
            idbms.CreateDatabase(command.substr(createDatabase.length()));
        }
        else if(i = command.find(showDatabases, 0) != string::npos) {
            idbms.ShowDatabases();
        }
        else if(i = command.find(use, 0) != string::npos) {
            idbms.Use(command.substr(use.length()));
        }
        else if(i = command.find(describe, 0) != string::npos) {
            idbms.Describe(command.substr(describe.length()));
        }
        else if(i = command.find(dropDatabase, 0) != string::npos) {
            idbms.DropDatabase(command.substr(dropDatabase.length()));
        }
        else if(i = command.find(showTables, 0) != string::npos) {
            idbms.ShowTables();
        }
        else if(i = command.find(dropTable, 0) != string::npos) {
            idbms.DropTable(command.substr(dropTable.length()));
        }
        else if(i = command.find(select, 0) != string::npos) {
            idbms.Select(command);
        }
        else if(i = command.find(insert, 0) != string::npos) {
            idbms.Insert(command);
        }
        else
            cout << "error" << endl;
    }
}


