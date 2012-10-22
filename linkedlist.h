#ifndef LINKEDLIST_H
#define	LINKEDLIST_H
#include <iostream>
#include <fstream>
using namespace std;
#define parameterMaxNumber 20

typedef class table {
private:
    int column_num;
    int table_len;
    int value_size[parameterMaxNumber];
    int column_pos[parameterMaxNumber];
    string column_name[parameterMaxNumber];
    int type[parameterMaxNumber];
    bool primary[parameterMaxNumber];
    char* tableFilePath;
public:
    table();
    table(char* tableName);
    int getColumn_num();
    int getTabale_len();
    int getValue_size(int index);
    int getColumn_pos(int index);
    int getValueType(int index);
    string getColumn_name(int index);
    char* getTableFilePath();
} tab;

typedef class parameterNode {
public:
    char* element;
    int position;
    int size;
    parameterNode();
    parameterNode(char* value, int position, int size);

} parameter;

typedef class parameterList {
public:
    parameter par[parameterMaxNumber];
    int size;
public:
    parameterList();
    parameterList(char* value, int position, int size);
    void add(char* value, int position, int size);
    int getSize();
    char* getValue(int i);
    int getPosition(int i);
    int getValueSize(int i);
} paralist;


#endif	/* LINKEDLIST_H */

