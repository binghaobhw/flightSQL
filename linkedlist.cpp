#include "linkedlist.h"

tab::table(char* tableName) {
    this->table_len = 0;
    this->column_num = 0;

    string path = tableName;
    if (path == "temp")
        path.append(".tmp");
    else if (path == "temp1")
        path.append(".tmp");
    else {
        path.append(".tab");
        this->tableFilePath = new char[path.size()];
        path.copy(this->tableFilePath, path.size(), 0);

        fstream read_model;
        read_model.open("model.md", ios::in | ios::binary);
        read_model.seekg(0, ios::end);
        int file_len = read_model.tellg();

        read_model.seekg(0);
        char* model = new char[file_len];
        read_model.read(model, file_len);

        int pos = 1; ///next read position
        while (1) {
            if (pos >= file_len)
                break;
            string tableInfo = model + pos;
            pos += tableInfo.length() + 1;


            int next = tableInfo.find(' ', 0);
            string tname = tableInfo.substr(0, next);
            string keyTable = tableName;
            if (tname == keyTable) {

                int position = 0;
                int col_cnt = 0;
                while (1) {
                    tableInfo = tableInfo.substr(next + 1);
                    next = tableInfo.find(' ', 0);
                    this->column_name[col_cnt] = tableInfo.substr(0, next);

                    tableInfo = tableInfo.substr(next + 1);
                    next = tableInfo.find(' ', 0);
                    string value_type = tableInfo.substr(0, next);
                    if (value_type == "char") {
                        this->type[col_cnt] = 0;

                        tableInfo = tableInfo.substr(next + 1);
                        next = tableInfo.find(' ', 0);
                        string size = tableInfo.substr(0, next);
                        this->value_size[col_cnt] = atoi(size.data());
                        this->table_len += atoi(size.data());
                        this->column_pos[col_cnt] = position;
                        position += atoi(size.data());

                        tableInfo = tableInfo.substr(next + 1);
                        next = -1;
                        next = tableInfo.find(' ', 0);

                        string key_type;
                        if (next < 0) //last parameter
                            key_type = tableInfo;
                        else
                            key_type = tableInfo.substr(0, next);

                        if (key_type == "*")
                            this->primary[col_cnt] = true;
                        else
                            this->primary[col_cnt] = false;
                        col_cnt++;

                        if (next < 0)
                            break;

                    } else if (value_type == "int") {
                        this->type[col_cnt] = 1;
                        this->value_size[col_cnt] = 4;
                        this->table_len += 4;
                        this->column_pos[col_cnt] = position;
                        position += 4;

                        tableInfo = tableInfo.substr(next + 1);
                        next = -1;
                        next = tableInfo.find(' ', 0);

                        string key_type;
                        if (next < 0) //last parameter
                            key_type = tableInfo;
                        else
                            key_type = tableInfo.substr(0, next);

                        if (key_type == "*")
                            this->primary[col_cnt] = true;
                        else
                            this->primary[col_cnt] = false;
                        col_cnt++;

                        if (next < 0)
                            break;
                    }
                }
                this->column_num = col_cnt;
                break;
            }
        }
        delete[] model;
        read_model.close();
    }
}

int tab::getColumn_pos(int index) {
    return this->column_pos[index];
}

int tab::getValue_size(int index) {
    return this->value_size[index];
}

int tab::getValueType(int index) {
    return this->type[index];
}

string tab::getColumn_name(int index) {
    return this->column_name[index];
}

int tab::getColumn_num() {
    return this->column_num;
}

int tab::getTabale_len() {
    return this->table_len;
}

char* tab::getTableFilePath() {
    return this->tableFilePath;
}

parameterNode::parameterNode() {
}

parameterNode::parameterNode(char* value, int position, int size) {
    this->element = value;
    this->position = position;
    this->size = size;
}

paralist::parameterList() {
    this->size = 0;
}

paralist::parameterList(char* value, int position, int size) {
    this->size = 1;
    par[0].element = value;
    par[0].position = position;
    par[0].size = size;
}

void paralist::add(char* value, int position, int size) {
    par[this->size].element = value;
    par[this->size].position = position;
    par[this->size].size = size;
    this->size++;
}

int paralist::getSize() {
    return size;
}

char* paralist::getValue(int i) {
    return par[i].element;
}

int paralist::getPosition(int i) {
    return par[i].position;
}

int paralist::getValueSize(int i) {
    return par[i].size;
}

