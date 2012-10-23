/* 
 * File:   DBMS.cpp
 * Author: Administrator
 * 
 * Created on 2012年10月15日, 下午2:49
 */
#include "DBMS.h"

DBMS::DBMS() {
    this->dirPath = "data";
    _mkdir(dirPath.c_str());
    this->currentDb = "";

}

DBMS::DBMS(const DBMS& orig) {
}

DBMS::~DBMS() {
}

void DBMS::SetDirPath(string path) {
    this->dirPath = path;
}

string DBMS::Replace(string src, string symbol, string r) {
    string::size_type i;
    int count;
    i = 0;
    while (true) {
        i = src.find(symbol, i);
        if (i != string::npos) {
            i++;
            count = 0;
            while (src[i + count] == symbol[0]) count++;
            if (count > 0) {
                src.replace(i, count, r);
            }
        } else {
            break;
        }
    }
    if (src[0] == symbol[0]) {
        src.erase(0, 1);
    }
    if (src[src.length() - 1] == symbol[0]) {
        src.erase(src.length() - 1, 1);
    }
    return src;

}

string DBMS::GetWholeName(string suffix) {
    return (this->dirPath + "\\" + suffix);
}

bool DBMS::IsDigit(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] < '0' || str[i] > '9')
            return false;
    }
    return true;
}

string DBMS::Parse(string words) {
    string::size_type i = 0;
    int count = 0;
    while (true) {
        i = words.find('(', 0);
        if (i != string::npos) {
            words.replace(i, 1, 1, ' ');
        } else {
            break;
        }
    }
    i = 0;
    while (true) {
        i = words.find(')', 0);
        if (i != string::npos) {
            words.replace(i, 1, 1, ' ');
        } else {
            break;
        }
    }
    i = 0;
    while (true) {
        i = words.find(',', 0);
        if (i != string::npos) {
            words.replace(i, 1, 1, ' ');
        } else {
            break;
        }
    }
    i = 0;
    while (true) {
        i = words.find("  ", 0);

        if (i != string::npos) {
            words.erase(i, 1);
        } else {
            if (words[0] == ' ') {
                words.erase(0, 1);
            }
            if (words[words.length() - 1] == ' ') {
                words.erase(words.length() - 1, 1);
            }
            break;
        }
    }

    return words;

}

void DBMS::ParseSelect(string sentence, vector<string>& attribute, 
                       vector<string>& table, vector<string>& condition)
{
    int i,j;
    string str;
    //得到列名
    i = sentence.find("select ", 0);
    if(i != string::npos) {
        j = sentence.find("from ", 0);
        if(j != string::npos) {
            str = sentence.substr(i, j - i);
            i = 0;
            j = 0;
            while(1) {
                i = sentence.find("\"", j);
                if(i != string::npos) {
                    i++;
                    j = sentence.find("\"", i);
                    if(j != string::npos) {
                        j++;
                        attribute.push_back(sentence.substr(i, j - i));
                    }
                }
                else break;
            }
        }
    }
    //得到表名
    i = sentence.find("from ", 0);
    if(i != string::npos) {
        j = sentence.find("where ", 0);
        if(j != string::npos) 
            str = sentence.substr(i, j - i);
        else 
            str = sentence.substr(i);
        i = 0;
        j = 0;
        while(1) {
            i = sentence.find("\"", j);
            if(i != string::npos) {
                i++;
                j = sentence.find("\"", i);
                if(j != string::npos) {
                    j++;
                    table.push_back(sentence.substr(i, j - i));
                }
            }
            else break;
        }
    }
    //得到条件
    i = sentence.find("where ", 0);
    if(i != string::npos) {
        str = sentence.substr(i);
        i = 0;
        j = 0;
        while(1) {
            i = sentence.find("\"", j);
            if(i != string::npos) {
                i++;
                j = sentence.find("\"", i);
                if(j != string::npos) {
                    j++;
                    condition.push_back(sentence.substr(i, j - i));
                }
            }
            else break;
        }
    }
}


void DBMS::GetTableInfo(string tableName) {
    if (currentDb.length() == 0) {
        cout << "choose a database first!" << endl;
        return;
    }
    string content = "";
    ReadModel(tableName, content);

    currentTable = tableName;
    column.clear();
    size.clear();
    type.clear();
    position.clear();
    key.clear();
    tupleLength = 0;

    string::size_type i, j, k;
    string tableInfo;
    string temp;

    i = content.find("~" + currentTable, 0);
    if (i != string::npos) {
        i = content.find(" ", i);
        i++;
        j = content.find("~", i);
        if (j != string::npos) {
            tableInfo = content.substr(i, j - i);
        } else {
            tableInfo = content.substr(i);
        }
    }


    int position = 0;
    int col_cnt = 0;
    int next;
    while (1) {
        next = tableInfo.find(' ', 0);
        this->column.push_back(tableInfo.substr(0, next));

        tableInfo = tableInfo.substr(next + 1);
        next = tableInfo.find(' ', 0);
        string value_type = tableInfo.substr(0, next);
        if (value_type == "0") {
            this->type.push_back(0);

            tableInfo = tableInfo.substr(next + 1);
            next = tableInfo.find(' ', 0);
            string size = tableInfo.substr(0, next);
            this->size.push_back(atoi(size.data()));
            this->tupleLength += atoi(size.data());
            this->position.push_back(position);
            position += atoi(size.data());

            tableInfo = tableInfo.substr(next + 1);
            next = -1;
            next = tableInfo.find(' ', 0);

            string key_type;
            if (next < 0) //last parameter
                key_type = tableInfo;
            else {
                key_type = tableInfo.substr(0, next);
                tableInfo = tableInfo.substr(next + 1);
            }
            if (key_type == "*")
                this->key.push_back(col_cnt);
            col_cnt++;

            if (next < 0)
                break;
        } else if (value_type == "1") {
            this->type.push_back(1);
            this->size.push_back(4);
            this->tupleLength += 4;
            this->position.push_back(position);
            position += 4;

            tableInfo = tableInfo.substr(next + 1);
            next = -1;
            next = tableInfo.find(' ', 0);

            string key_type;
            if (next < 0) //last parameter
                key_type = tableInfo;
            else {
                key_type = tableInfo.substr(0, next);
                tableInfo = tableInfo.substr(next + 1);
            }
            if (key_type == "*")
                this->key.push_back(col_cnt);
            col_cnt++;

            if (next < 0)
                break;
        }
    }



}

void DBMS::ReadModel(string tableName, string& content) {
    char c[1024];
    string modelPath = GetWholeName(currentDb) + "\\model.md";
    fstream fileIO;
    fileIO.open(modelPath.c_str(), ios::binary | ios::in);
    int fileSize = 0; //model.md的大小
    fileIO.seekg(0, ios::end);
    fileSize = fileIO.tellg();

    fileIO.seekg(0, ios::beg);
    fileIO.read(c, fileSize);
    fileIO.close();
    content.append(c, fileSize);


}

void DBMS::WriteModel(string& content, int mode) {
    string modelPath = GetWholeName(currentDb) + "\\model.md";
    fstream fileIO;
    if (mode == 1) {
        fileIO.open(modelPath.c_str(), ios::binary | ios::out | ios::in);
        fileIO.seekp(0, ios::end);
        fileIO.write(content.c_str(), content.length());
        fileIO.close();
    } else if (mode == 0) {
        fileIO.open(modelPath.c_str(), ios::binary | ios::out);
        fileIO.write(content.c_str(), content.length());
        fileIO.close();
    }
}
void DBMS::Compare(int processed, string tableName, 
                   string col, string op, string value)
{
    list<char*> tuple;
    string filePath;
    myfstream fs;
    filePath = GetWholeName(currentDb) + "\\";
    //无需读temp
    if(processed == 0) {
        filePath += tableName + ".tab";
    }
    //读temp
    else {
        filePath += "temp.tab";
    }
    fs.open(filePath);
    while (!readfile.end()) {
        char* piece = new char[piece_length];
        readfile.read(piece);
        int max_tuple_num_in_piece = piece_length / this->tupleLength;
        for (int i = 0; i < max_tuple_num_in_piece; i++) {
            char* yuanzu = piece + i * (this->tupleLength + 1);
            if (yuanzu[0] == '\0')
                break;
            bool same = true;
            if (yuanzu[0] == 'y') { //yuanzu  option
                for (int j = 0; j < this->key.size(); j++) { //check  primaryKey
                    int index = this->key[j];
                    int position = this->position[index];
                    string parameterValue = value[index];
                    string key = yuanzu + position + 1;
                    if (key != parameterValue) {
                        same = false;
                        break;
                    }
                }
            }
            if (same) {//if same  ,quit
                delete[] piece;
                return;
            }
        }
        delete[] piece;
    }
}

int DBMS::CreateDatabase(string databaseName) {
    int res = 0;
    string fullName = GetWholeName(databaseName);
    string modelPath = fullName + "\\model.md";
    string tempPath = fullName + "\\temp.tmp";
    ofstream fileIO;
    switch (_mkdir(fullName.c_str())) {
        case 0:
            fileIO.open(modelPath.c_str());
            fileIO.close();
            fileIO.open(tempPath.c_str());
            fileIO.close();

            res = 0;
            break;
        case EEXIST:
            res = DBEXISTED;
            break;
        default:
            res = UNKNOWN;
            break;
    }
    return res;

}

int DBMS::DropDatabase(string databaseName) {
    string str = "rmdir /s /q ";
    str.append(GetWholeName(databaseName));
    system(str.c_str());
    return 0;

}

void DBMS::ShowDatabases() {
    string dbnames;
    intptr_t handle;
    struct _finddata_t fileInfo;
    string fullName = GetWholeName("*");
    handle = _findfirst(fullName.c_str(), &fileInfo);
    if (handle != -1L) {
        do {
            if (fileInfo.attrib & _A_SUBDIR) {
                if ((strcmp(fileInfo.name, ".") != 0)
                        && (strcmp(fileInfo.name, "..") != 0)) {
                    dbnames = fileInfo.name;
                    cout << dbnames << endl;
                }
            }
        } while (_findnext(handle, &fileInfo) == 0);
        _findclose(handle);

    }

}

void DBMS::Use(string databaseName) {
    intptr_t handle;
    struct _finddata_t fileInfo;
    string fullName = GetWholeName("*");
    handle = _findfirst(fullName.c_str(), &fileInfo);
    if (handle != -1L) {
        do {
            if ((strcmp(fileInfo.name, databaseName.c_str()) == 0)) {
                currentDb = databaseName;

                ///////////////////
                cout << "chosen database: " << currentDb << endl;
            }

        } while (_findnext(handle, &fileInfo) == 0);
        _findclose(handle);

    }
}

int DBMS::CreateTable(string sentence) {
    const string primaryKey = "primary key";
    const char star = '*';
    int res = 0;
    string tableName = "";
    string::size_type i, j, k;
    sentence = sentence.erase(0, 12);
    i = sentence.find(' ', 0);
    if (i != string::npos) {
        i++;
        j = sentence.find('(', i);
        if (j != string::npos) {
            tableName = sentence.substr(i, j - i);
        }
    }
    if (tableName.length() == 0)
        return -1;

    string c;
    ReadModel(tableName, c);
    i = c.find("~" + tableName, 0);
    if (i != string::npos) {
        res = TABLEEXISTED;
        return res;
    }
    /*  查找关键字primary key 然后替换为星号*  */
    i = 0;
    while (true) {
        i = sentence.find(primaryKey, 0);
        if (i != string::npos) {
            sentence.replace(i, primaryKey.length(), 1, star);
        } else {
            break;
        }
    }
    /*  查找关键字char 然后替换为0  */
    i = 0;
    while (true) {
        i = sentence.find(" char", 0);
        if (i != string::npos) {
            j = sentence.find(')', i);
            if (j != string::npos) {
                k = sentence.find('*', j);
                if (k - j > 5 || k == string::npos) {
                    sentence.insert(j, " # ");
                }
            }
            sentence.replace(i, 5, " 0 ");
        } else {
            break;
        }
    }
    /*  查找关键字int 然后替换为1 */
    i = 0;
    while (true) {
        i = sentence.find(" int", 0);
        if (i != string::npos) {

            k = sentence.find('*', i + 3);
            if (k - i > 5 || k == string::npos) {
                sentence.insert(i + 4, " #");
            }


            sentence.replace(i, 5, " 1 ");
        } else {
            break;
        }
    }
    sentence = Parse(sentence);
    sentence.insert(0, 1, '~');

    WriteModel(sentence, 1);

    /*  创建tab文件供元组信息存放  */
    string tableFile;
    fstream fileIO;
    tableFile = GetWholeName(currentDb);
    tableFile += "\\";
    tableFile.append(tableName);
    tableFile += ".tab";
    fileIO.open(tableFile.c_str(), ios::binary | ios::out);
    fileIO.close();

    res = 0;

    return res;


}

int DBMS::DropTable(string words) {
    string tableName = "";
    string::size_type i, j, k;
    i = words.find("drop table", 0);
    if (i != string::npos) {
        tableName = words.substr(11);
    }
    string c;
    ReadModel(tableName, c);
    i = c.find("~" + tableName, 0);
    if (i != string::npos) {
        j = c.find("~", i + 1);
        if (j != string::npos) {
            c.erase(i, j - i);
        } else {
            c.erase(i);
        }
        WriteModel(c, 0);

        tableName = GetWholeName(currentDb + "\\" + tableName + ".tab");
        tableName.insert(0, "del ");
        system(tableName.c_str());

        return 0;
    } 
    else return -1;

}

void DBMS::Describe(string tableName) {
    if (currentDb.length() == 0) {
        cout << "choose a database first!" << endl;
        return;
    }
    GetTableInfo(tableName);
    char s[10];
    string description = currentTable;
    description += "(";
    int i, j;
    for(i = 0; i < column.size(); i++) {
        description += column[i];
        if(type[i] == 0) {
            description += " char(";
            sprintf(s, "%d", size[i]);
            description.append(s, size[i] / 10 + 1);
            description += ")";
        }
        else if(type[i] == 1) {
            description += " int";
        }
        for(j = 0; j < key.size(); j++) {
            if(key[j] == i) {
                description += " primary key";
                break;
            }
        }
        if(column.size() - i == 1) {
            description += ")";
        }
        else {
            description += ", ";
        }
    }
    cout << description << endl;
    
    

}

void DBMS::ShowTables() {
    if (currentDb.length() == 0) {
        cout << "choose a database first!" << endl;
        return;
    }

    const char space = ' ';
    char content[1024];
    string container = "";
    int fileSize = 0;
    fstream fileIO;
    string filePath = "";
    /*  打开model.md文件并读取*/
    filePath = GetWholeName(currentDb) + "\\model.md";
    fileIO.open(filePath.c_str(), ios::in | ios::binary);
    fileIO.seekg(0, ios::end);
    fileSize = fileIO.tellg();

    if (fileSize != 0) {
        fileIO.seekg(0, ios::beg);
        fileIO.read(content, fileSize);
        fileIO.close();

        container.insert(0, content, fileSize);

        int i = 0, j = 0;
        while (true) {
            i = container.find("~", i);
            if (i != string::npos) {
                i++;
                j = container.find(space, i);
                if (j != string::npos) {
                    cout << container.substr(i, j - i) << endl;
                }
            } else {
                return;
            }
        }
    } else {
        cout << "no any table!" << endl;
    }
}

void DBMS::Select(string sentence) {
    if (currentDb.length() == 0) {
        cout << "choose a database first!" << endl;
        return;
    }
    int processed = 0;
    int i,j,k,m;
    string table1, table2;
    string col1, col2;
    string op;
    string value;
    vector<string> attribute;
    vector<string> table;
    vector<string> condition;
    ParseSelect(sentence, attribute, table, condition);
    vector<string>::iterator p;
    //找等值连接所需参数
    for(p = condition.begin(); p != condition.end(); p++) {
        i = p->find(".", 0);
        if(i != string::npos) {
            i++;
            j = p->find(".", i);
            if(j != string::npos) {
                k = p->find(" ", i);
                col1 = p->substr(i, k - i);
                table1 = p->substr(0, i - 1);
                j++;
                m = p->rfind(" ", j);
                m++;
                col2 = p->substr(j);
                table2 = p->substr(m, j - m - 1); 
                Equi_Join((char*)table1.c_str(), (char*)table2.c_str(), (char*)col1.c_str(), (char*)col2.c_str());
                processed = 1;
                break;
            }
            else break;
        }
    }
    //得到选择所需参数
    for(p = condition.begin(); p != condition.end(); p++) {
        i = p->find(".", 0);
        if(i != string::npos) {
            i++;
            j = p->find(".", i);
            if(j == string::npos) {
                j = p->find(" ", 0);
                k = p->find(" ", j + 1);
                table1 = p->substr(0, i - 1);
                col1 = p->substr(i, j - i);
                op = p->substr(j + 1, k - j - 1);
                value = p->substr(k + 1);
                Compare(processed, table1, col1, op, value);
                processed = 1;
            }
        }
    }
    
    
    
    
    
    


}

void DBMS::Projection(char* tableFileName, int tableSize, int* positionList,
        int* valueSize, int list_size) {
    fstream selectTable;
    fstream writeTemp;
    writeTemp.open("temp.tmp", ios::out | ios::binary);
    selectTable.open(tableFileName, ios::in | ios::binary);
    selectTable.seekg(0, ios::end);
    int fileSize = selectTable.tellg();
    int Num_Of_Yuanzu = maxReadSize / tableSize;
    int readSize = Num_Of_Yuanzu * tableSize;
    int accumulate = 0;

    selectTable.seekg(0);

    while (1) {
        int distance = fileSize - selectTable.tellg();
        if (distance <= readSize) {
            char* tempPiece = new char[readSize + 1];
            char* yuanzu = new char[tableSize + 1];
            selectTable.read(tempPiece, distance); //get pieces
            //option

            //  get i yuanzu
            int num_of_yuanzu = distance / tableSize;
            for (int i = 0; i < num_of_yuanzu; i++) {
                yuanzu = tempPiece + i*tableSize;
                //yuanzu  option
                if (yuanzu[0] == 'y') { //yuanzu  option
                    for (int k = 0; k < list_size; k++) {
                        char* tempValue = yuanzu + positionList[k] + 1;
                        writeTemp.write(tempValue, valueSize[k]);
                    }
                }
            }
            delete[] yuanzu;
            delete[] tempPiece;
            break;
        } else {
            char* tempPiece = new char[readSize + 1];
            char* yuanzu = new char[tableSize + 1];
            selectTable.read(tempPiece, readSize);
            //  piece  option
            for (int i = 0; i < Num_Of_Yuanzu; i++) {
                yuanzu = tempPiece + i*tableSize;
                //yuanzu  option
                if (yuanzu[0] == 'y') { //yuanzu  option
                    for (int k = 0; k < list_size; k++) {

                        char* tempValue = yuanzu + positionList[k] + 1;
                        writeTemp.write(tempValue, valueSize[k]);
                    }
                }
            }
            delete[] yuanzu;
            delete[] tempPiece;
            accumulate++;
            selectTable.seekg(accumulate * readSize);
        }
    }


    writeTemp.close();
    selectTable.close();
}

void DBMS::Insert(string words) {
    string tableName;
    string::size_type x, y;
    x = words.find("insert into", 0);
    if (x != string::npos) {
        x += 12;
        y = words.find(" ", x);
        if (y != string::npos) {
            tableName = words.substr(x, y - x);
        }
    } else return;

    GetTableInfo(tableName);

    vector<string> value;
    int first = 0;
    int second = 0;
    for (int k = 0; k<this->column.size(); k++) {
        first = words.find("\"", first);
        second = words.find("\"", first + 1);
        value.push_back(words.substr(first + 1, second - first - 1));
        first = second + 1;
    }
    string tabFile = "";
    tabFile = GetWholeName(this->currentDb) + "\\" + this->currentTable + ".tab";

    myfstream readfile; /*get file piece*/
    readfile.open(tabFile);
    while (!readfile.end()) {
        char* piece = new char[piece_length];
        readfile.read(piece);
        int max_tuple_num_in_piece = piece_length / this->tupleLength;
        for (int i = 0; i < max_tuple_num_in_piece; i++) {
            char* yuanzu = piece + i * (this->tupleLength + 1);
            if (yuanzu[0] == '\0')
                break;
            bool same = true;
            if (yuanzu[0] == 'y') { //yuanzu  option
                for (int j = 0; j < this->key.size(); j++) { //check  primaryKey
                    int index = this->key[j];
                    int position = this->position[index];
                    string parameterValue = value[index];
                    string key = yuanzu + position + 1;
                    if (key != parameterValue) {
                        same = false;
                        break;
                    }
                }
            }
            if (same) {//if same  ,quit
                delete[] piece;
                return;
            }
        }
        delete[] piece;
    }
    
    fstream writeTable;
    writeTable.seekp(0, ios::end);
    int filelength = writeTable.tellp();
    //补全piece不足一个元组的文件位置
    int next_file_end = filelength + this->tupleLength + 1;
    int piece_eage = next_file_end / piece_length *piece_length;
    if ((piece_eage > filelength) && (piece_eage < next_file_end))
        while (writeTable.tellp() < piece_eage)
            writeTable.put('\0');

    writeTable.put('y');
    for (int m = 0; m < this->column.size(); m++) {
        int contentSize = this->size[m];
        string content = value[m];
        int valueType = this->type[m];
        if (valueType == 0) {
            content.append(contentSize, '\0');
            writeTable.write(content.data(), contentSize);
        } else if (valueType == 1) {
            int number = atoi(content.data());
            writeTable.write(reinterpret_cast<char*> (&number), contentSize);
        }
    }
    writeTable.close();


}
//void DBMS::Insert(string words) {
//    string tableName;
//    string::size_type x, y;
//    x = words.find("insert into", 0);
//    if (x != string::npos) {
//        x += 12;
//        y = words.find(" ", x);
//        if (y != string::npos) {
//            tableName = words.substr(x, y - x);
//        }
//    } else return;
//
//    GetTableInfo(tableName);
//
//    vector<string> value;
//    int first = 0;
//    int second = 0;
//    for (int k = 0; k<this->column.size(); k++) {
//        first = words.find("\"", first);
//        second = words.find("\"", first + 1);
//        value.push_back(words.substr(first + 1, second - first - 1));
//        first = second + 1;
//    }
//    string tabFile = "";
//    tabFile = GetWholeName(this->currentDb) + "\\" + this->currentTable + ".tab";
//    fstream writeTable;
//    writeTable.open(tabFile.c_str(), ios::in | ios::out | ios::binary);
//
//    writeTable.seekp(0, ios::end);
//    int FileLength = writeTable.tellp();
//    int tableLength = this->tupleLength;
//    int Num_Of_Yuanzu = maxReadSize / (tableLength + 1);
//    int readSize = Num_Of_Yuanzu * (tableLength + 1);
//
//    int accumulate = 0;
//    writeTable.seekg(0);
//    while (1) {
//        int distance = FileLength - writeTable.tellg();
//
//        if (distance <= readSize) {
//            char* tempPiece = new char[readSize + 1];
//            char* yuanzu = new char[tableLength + 1];
//            writeTable.read(tempPiece, distance); //get pieces
//            //option
//            //  get i yuanzu
//            int num_of_yuanzu = distance / (tableLength + 1);
//            for (int i = 0; i < num_of_yuanzu; i++) {
//                yuanzu = tempPiece + i * (tableLength + 1);
//
//                bool same = true;
//                if (yuanzu[0] == 'y') { //yuanzu  option
//                    for (int j = 0; j < this->key.size(); j++) { //check  primaryKey
//                        int index = this->key[j];
//                        int position = this->position[index];
//                        string parameterValue = value[index];
//                        string key = yuanzu + position + 1;
//                        if (key != parameterValue) {
//                            same = false;
//                            break;
//                        }
//                    }
//                }
//                if (same) { //if same  ,quit
//                    delete[] yuanzu;
//                    delete[] tempPiece;
//                    return;
//                }
//            }
//            delete[] yuanzu;
//            delete[] tempPiece;
//            break;
//        } else {
//            char* tempPiece = new char[readSize + 1];
//            char* yuanzu = new char[tableLength + 1];
//            writeTable.read(tempPiece, readSize);
//            //  piece  option
//            for (int i = 0; i < Num_Of_Yuanzu; i++) {
//                yuanzu = tempPiece + i * (tableLength + 1);
//                //yuanzu  option
//                bool same = true;
//                if (yuanzu[0] == 'y') { //yuanzu  option
//                    for (int j = 0; j < this->key.size(); j++) { //check  primaryKey
//                        int index = this->key[j];
//                        int position = this->position[index];
//                        string parameterValue = value[index];
//                        string key = yuanzu + position + 1;
//                        if (key != parameterValue) {
//                            same = false;
//                            break;
//                        }
//                    }
//                }
//                if (same) { //if same  ,quit
//                    delete[] yuanzu;
//                    delete[] tempPiece;
//                    return;
//                }
//            }
//            delete[] yuanzu;
//            delete[] tempPiece;
//            accumulate++;
//            writeTable.seekg(accumulate * readSize);
//        }
//    }
//
//    writeTable.seekp(0, ios::end);
//    writeTable.put('y');
//
//    for (int m = 0; m < this->column.size(); m++) {
//        int contentSize = this->size[m];
//        string content = value[m];
//        int valueType = this->type[m];
//        if (valueType == 0) {
//            content.append(contentSize, '\0');
//            writeTable.write(content.data(), contentSize);
//        } else if (valueType == 1) {
//            int number = atoi(content.data());
//            writeTable.write(reinterpret_cast<char*> (&number), contentSize);
//        }
//    }
//    writeTable.close();
//
//
//}

void DBMS::Equi_Join(char* tableName1, char* tableName2, char* Col1, char* Col2) {
    int duqu1[6] = {0}; //duqu1、duqu2记录读取规格
    int duqu2[6] = {0};

    int sumdatatype1[10];
    int sumdatatype2[10];

    int t1sx_hao = 1; //table1中包含的属性个数,t1sx_hao-1
    int t2sx_hao = 1; //table2中包含的属性个数
    //int colnum=0;
    //int cols[6]={0};
    int sx1index = 0; //sx1index、sx2index记录连接的属性在规格中的下标
    int sx2index = 0;
    bool judge1 = false; //用来检验是否找到两个属性
    bool judge2 = false;
    int datatypes = 0;
    int datatype1 = 0; //用于连接的属性的类型，0表示int型，i>0表示char(i)
    int datatype2 = 0;
    //int sx1num;                 //table1中包含的属性个数
    //int sx2num;                 //table2中包含的属性个数

    string tname; //记录当前的表名
    string table1 = tableName1;
    string table2 = tableName2;
    string nowsxname; //记录当前属性名
    string datatype; //记录当前数据类型名
    string sx1 = Col1;
    string sx2 = Col2;
    // int duquindex=1;
    duqu1[0] = 1;
    duqu2[0] = 1;

    fstream binaryio;
    binaryio.open("E:/temp.dat", ios::in | ios::binary);


    char *tempchar = new char;
    char tmpchars[30] = {'\000'}; //临时的char型数组，用于逐位记录数据
    int index = 0;

    binaryio.read(tempchar, 1);
    while (!binaryio.eof()) { //找到名字匹配的表

        if (*tempchar == '\0') { //说明下一个数据是表名或者为结束
            binaryio.read(tempchar, 1);
            if (binaryio.eof()) {
                break;
            }
            index = 0;
            while (*tempchar != ' ') { //读表名

                tmpchars[index] = *tempchar;
                index++;
                binaryio.read(tempchar, 1);
            }
            tmpchars[index] = '\0';
            tname = tmpchars;


            if (tname == table1) { //如果该表正是要查找的表，读其属性
                //binaryio.read(tempchar,1);
                // binaryio.read(tempchar,1);
                int duquindex = 1;
                while (*tempchar != '\0') { //一直执行到遇到下一个‘\0'即到下一个表或者是结束
                    index = 0;

                    // while (tempchar!='\0'){
                    binaryio.read(tempchar, 1);
                    while (*tempchar != ' ') { //读取属性名
                        tmpchars[index] = *tempchar;
                        index++;
                        binaryio.read(tempchar, 1);
                    }
                    tmpchars[index] = '\0';
                    nowsxname = tmpchars;


                    binaryio.read(tempchar, 1); //读数据类型
                    index = 0;
                    while (*tempchar != ' ' && *tempchar != '\0') {
                        tmpchars[index] = *tempchar;
                        index++;
                        binaryio.read(tempchar, 1);
                    }
                    tmpchars[index] = '\0';
                    datatype = tmpchars;

                    int value;

                    if (datatype == "char") { //如果是char型，接下来读取包含字符的个数-value
                        cout << "zai char li" << endl;
                        //binaryio.read(tempchar,1);
                        binaryio.read((char *) (&value), sizeof (value));
                        datatypes = value;
                        binaryio.read(tempchar, 1);

                    }

                    if (datatype == "int") { //int型
                        cout << "zai int li" << endl;
                        value = 4;
                        datatypes = 0;

                        //    binaryio.read(tempchar,1);
                    }
                    duquindex += value;
                    duqu1[t1sx_hao] = duquindex;
                    sumdatatype1[t1sx_hao] = datatypes;

                    if (sx1 == nowsxname) { //找到用于连接的属性
                        sx1index = t1sx_hao;
                        judge1 = true;
                        datatype1 = datatypes;
                    }
                    t1sx_hao++;
                    //binaryio.read(tempchar,1);
                }
            }
            else if (tname == table2) { //如果该表正是要查找的表，读其属性
                //binaryio.read(tempchar,1);
                // binaryio.read(tempchar,1);
                int duquindex = 1;
                while (*tempchar != '\0') { //一直执行到遇到下一个‘\0'即到下一个表或者是结束
                    index = 0;

                    // while (tempchar!='\0'){
                    binaryio.read(tempchar, 1);
                    while (*tempchar != ' ') { //读取属性名
                        tmpchars[index] = *tempchar;
                        index++;
                        binaryio.read(tempchar, 1);
                    }
                    tmpchars[index] = '\0';
                    nowsxname = tmpchars;


                    binaryio.read(tempchar, 1); //读数据类型
                    index = 0;
                    while (*tempchar != ' ' && *tempchar != '\0') {
                        tmpchars[index] = *tempchar;
                        index++;
                        binaryio.read(tempchar, 1);
                    }
                    tmpchars[index] = '\0';
                    datatype = tmpchars;

                    int value;

                    if (datatype == "char") { //如果是char型，接下来读取包含字符的个数-value
                        cout << "zai char li" << endl;
                        //binaryio.read(tempchar,1);
                        binaryio.read((char *) (&value), sizeof (value));
                        datatypes = value;

                        binaryio.read(tempchar, 1);
                    }

                    if (datatype == "int") { //int型
                        cout << "zai int li" << endl;
                        value = 4;

                        datatypes = 0;
                        //    binaryio.read(tempchar,1);
                    }
                    duquindex += value;
                    duqu2[t2sx_hao] = duquindex;
                    sumdatatype2[t2sx_hao] = datatypes;

                    if (sx2 == nowsxname) { //找到用于连接的属性
                        sx2index = t2sx_hao;
                        datatype2 = datatypes;
                        judge2 = true;
                    }
                    t2sx_hao++;
                    //binaryio.read(tempchar,1);
                }
            }
        } else {
            binaryio.read(tempchar, 1);
        }
    }
    // cout<<"axyz"<<endl;

    if (!judge1 || !judge2) {
        cout << "无法找到所要查找的属性" << endl;
    } else if (datatype1 != datatype2) {
        cout << "属性类型不匹配" << endl;
    } else {
        int tmpint1, tmpint2;
        char* tmpchars1, tmpchars2;
        int table1num = 0; //table元组个数
        int table2num = 0;

        t1sx_hao--;
        t2sx_hao--;

        int tolast1 = duqu1[t1sx_hao] - duqu1[sx1index];
        int tolast2 = duqu2[t2sx_hao] - duqu2[sx2index];
        int tofirst1 = duqu1[sx1index - 1];
        int tofirst2 = duqu2[sx2index - 1];


        char s0[100];
        char s1[100];
        char s2[100];
        table1.append(".tab");
        table2.append(".tab");
        char *t1place = (char *) table1.c_str();
        char *t2place = (char *) table2.c_str();

        char linshi[100]; //用于char*跟string的转换

        fstream t1, t2;

        if (datatype1 == 0) { //如果连接类型是整型
            int t1_int[200] = {0}; //把table1里用于连接的属性都记录下来
            int t2_int[200] = {0};


            t1.open(t1place, ios::in | ios::binary);
            while (!t1.eof()) {
                t1.seekg(tofirst1, ios::cur);
                t1.read(reinterpret_cast<char *> (&t1_int[table1num]), sizeof (int));
                t1.seekg(tolast1, ios::cur);
                table1num++;

            }
            t1.close();
            table1num--;



            t2.open(t2place, ios::in | ios::binary);
            while (!t2.eof()) {
                t2.seekg(tofirst2, ios::cur);
                t2.read(reinterpret_cast<char *> (&t2_int[table2num]), sizeof (int));
                t2.seekg(tolast2, ios::cur);
                table2num++;
            }
            t2.close();
            table2num--;


            fstream temp, t1, t2;
            temp.open("temp.tab", ios::out | ios::binary);
            t1.open(t1place, ios::in | ios::binary);
            //t2.open(t2place,ios::in|ios::binary);
            for (int i = 0; i < table1num; i++) {
                t1.read(s0, duqu1[t1sx_hao] * sizeof (char)); //把table1的元组逐次读出
                s0[duqu1[t1sx_hao]] = '\0';

                t2.open(t2place, ios::in | ios::binary);
                for (int t = 0; t < table2num; t++) {


                    t2.read(s1, tofirst2 * sizeof (char));
                    s1[tofirst2] = '\0';
                    t2.seekg(4L, ios::cur);
                    t2.read(s2, tolast2 * sizeof (char));
                    s2[tofirst2] = '\0';
                    if (t1_int[i] == t2_int[t]) { //如果连接的属性值相等，把t1的元组写入temp
                        temp.write(s0, duqu1[t1sx_hao] * sizeof (char));
                        //t2.read(s,tofirst2*sizeof(char));
                        //s[tofirst2]='\0';
                        temp.write(s1, tofirst2 * sizeof (char));
                        //t2.seekg(4L,ios::cur);
                        //t2.read(s,tolast2*sizeof(char));
                        // s[tofirst2]='\0';
                        temp.write(s2, tolast2 * sizeof (char));
                    }
                }
                t2.close();
            }
            temp.close();
            t1.close();
        }
        else {
            //如果连接类型是char型
            string t1_strings[200] = {""}; //把table1里用于连接的属性都记录下来
            string t2_strings[200] = {""};


            t1.open(t1place, ios::in | ios::binary);

            while (!t1.eof()) {
                t1.seekg(tofirst1, ios::cur);
                t1.read(linshi, datatype1);
                linshi[datatype1] = '\0';
                t1_strings[table1num] = linshi;
                t1.seekg(tolast1, ios::cur);
                table1num++;

            }
            t1.close();
            table1num--;



            t2.open(t2place, ios::in | ios::binary);
            while (!t2.eof()) {
                t2.seekg(tofirst2, ios::cur);
                t2.read(linshi, datatype1);
                linshi[datatype1] = '\0';
                t2_strings[table2num] = linshi;
                t2.seekg(tolast2, ios::cur);
                table2num++;
            }
            t2.close();
            table2num--;





            fstream temp, t1, t2;
            temp.open("temp.tab", ios::out | ios::binary);
            t1.open(t1place, ios::in | ios::binary);
            //t2.open(t2place,ios::in|ios::binary);
            for (int i = 0; i < table1num; i++) {
                t1.read(s0, duqu1[t1sx_hao] * sizeof (char)); //把table1的元组逐次读出
                s0[duqu1[t1sx_hao]] = '\0';


                t2.open(t2place, ios::in | ios::binary);
                for (int t = 0; t < table2num; t++) {


                    t2.read(s1, tofirst2 * sizeof (char));
                    s1[tofirst2] = '\0';
                    t2.seekg(datatype1, ios::cur);
                    t2.read(s2, tolast2 * sizeof (char));
                    s2[tofirst2] = '\0';
                    if (t1_strings[i] == t2_strings[t]) { //如果连接的属性值相等，把t1的元组写入temp
                        temp.write(s0, duqu1[t1sx_hao] * sizeof (char));
                        //t2.read(s,tofirst2*sizeof(char));
                        // s[tofirst2]='\0';
                        temp.write(s1, tofirst2 * sizeof (char));
                        //t2.seekg(4L,ios::cur);
                        //t2.read(s,tolast2*sizeof(char));
                        // s[tofirst2]='\0';
                        temp.write(s2, tolast2 * sizeof (char));
                    }
                }
                t2.close();
            }
            temp.close();
            t1.close();



        }

    }

}
