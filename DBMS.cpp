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
                i = str.find("\"", j);
                if(i != string::npos) {
                    i++;
                    j = str.find("\"", i);
                    if(j != string::npos) {
                        j++;
                        attribute.push_back(str.substr(i, j - i));
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
            i = str.find("\"", j);
            if(i != string::npos) {
                i++;
                j = str.find("\"", i);
                if(j != string::npos) {
                    j++;
                    table.push_back(str.substr(i, j - i));
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
            i = str.find("\"", j);
            if(i != string::npos) {
                i++;
                j = str.find("\"", i);
                if(j != string::npos) {
                    j++;
                    condition.push_back(str.substr(i, j - i));
                }
            }
            else break;
        }
    }
}


void DBMS::GetTableInfo(string tableName, int temp) 
{
    if (currentDb.length() == 0) {
        cout << "choose a database first!" << endl;
        return;
    }
    string content = "";
    string modelPath = this->GetWholeName(this->currentDb);
    if(temp == 0)
        modelPath += "\\model.md";
    else
        modelPath += "\\temp.md";
    ReadModel(modelPath, content);

    currentTable = tableName;
    column.clear();
    size.clear();
    type.clear();
    position.clear();
    key.clear();
    tupleLength = 0;

    string::size_type i, j, k;
    string tableInfo;

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

void DBMS::ReadModel(string modelPath, string& content) {
    char c[1024];
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
void DBMS::Compare(int processed, string tableName, vector<string>& col, 
                   vector<string>& op, vector<string>& value)
{
    list<char*> tuple;
    list<char*>::iterator charIt;
    fstream writeTemp;
    char** piece;
    char* yuanzu;
    char p[20];
    int i,j,x,y;
    int digitCol, digitValue;
    int type, size, position;
    int count;
    int ope;  /* 0-=  1->  2-< 3->= 4-<= */
    string filePath;
    string strCol;
    myfstream fs;
    filePath = GetWholeName(currentDb) + "\\";
    //无需读TEMP,读表文件
    if(processed == 0) {
        filePath += tableName + ".tab";
        this->GetTableInfo(tableName, 0);
    }
    //读TEMP
    else {
        filePath += "temp.tmp";
        this->GetTableInfo("TEMP", 1);
    }
    //读取
    fs.open(filePath);
    count = 0;
    while (!fs.end()) {
        piece[count] = new char[piece_length];
        fs.read(piece[count]);
        int max_tuple_num_in_piece = piece_length / this->tupleLength;
        for (i = 0; i < max_tuple_num_in_piece; i++) {
            yuanzu = piece[count] + i * (this->tupleLength + 1);
            if (yuanzu[0] == '\0')
                break;
            if (yuanzu[0] == 'y') { //yuanzu  option
                tuple.push_back(yuanzu);
            }
        }
        count++;
    }
    //在tuple链表中比较，删除不合格的
    for(i = 0; i < op.size(); i++) {
        if(op[i] == "=") 
            ope = 0;
        if(op[i] == ">")
            ope = 1;
        if(op[i] == "<")
            ope = 2;
        if(op[i] == ">=")
            ope = 3;
        if(op[i] == "<=")
            ope = 4;
        for(j = 0; j < this->column.size(); j++) {
            if(column[j] == col[i]) {
                type = this->type[j];
                if(type == 1)
                    digitValue = atoi(value[i].c_str());
                size = this->size[j];
                position = this->position[j];
                position++;
                break;
            }
        }
        for(charIt = tuple.begin(); charIt != tuple.end(); charIt++) {
            //比int型
            if(type == 1) {      
                for(x = 0; x < size; x++) {
                    *(reinterpret_cast<char* >(&digitCol) + x)= (*charIt)[position + x];
                }
                switch(ope) {
                case 0:
                    if(digitCol != digitValue) {
                        charIt = tuple.erase(charIt);
                        charIt--;
                    }
                    break;
                case 1:
                    if(!(digitCol > digitValue)) {
                        charIt = tuple.erase(charIt);
                        charIt--;
                    }
                    break;
                case 2:
                    if(!(digitCol < digitValue)) {
                        charIt = tuple.erase(charIt);
                        charIt--;
                    }
                    break;
                case 3:
                    if(!(digitCol >= digitValue)) {
                        charIt = tuple.erase(charIt);
                        charIt--;
                    }
                    break;
                case 4:
                    if(!(digitCol <= digitValue)) {
                        charIt = tuple.erase(charIt);
                        charIt--;
                    }
                    break;
                default:
                    break;
                }
            }
            //比char型
            else {
                strCol = (*charIt) + position;
                value[i] = value[i].substr(0, size);
                if(strCol.length() > size)
                    strCol = strCol.substr(0, size);                
                switch(ope) {
                    case 0:
                        if(!(strCol == value[i])) {
                            charIt = tuple.erase(charIt);
                            charIt--;
                        }
                        break;
                    case 1:
                        if(!(strCol > value[i])) {
                            charIt = tuple.erase(charIt);
                            charIt--;
                        }
                        break;
                    case 2:
                        if(!(strCol < value[i])) {
                            charIt = tuple.erase(charIt);
                            charIt--;
                        }
                        break;
                    case 3:
                        if(!(strCol >= value[i])) {
                            charIt = tuple.erase(charIt);
                            charIt--;
                        }
                        break;
                    case 4:
                        if(!(strCol <= value[i])) {
                            charIt = tuple.erase(charIt);
                            charIt--;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
    //写入temp.tmp
    writeTemp.open("temp.tmp", ios::binary | ios::out);
    for(charIt = tuple.begin(); charIt != tuple.end(); charIt++) {
        j += (tupleLength + 1);
        x = piece_length - (j % piece_length) - this->tupleLength - 1;
        if(x < 0) {
            for(y = x; y < 0; y++) {
                writeTemp.put('\0');
            }
        }   
        writeTemp.write(*charIt, this->tupleLength + 1);
    }
    writeTemp.close();
    //释放内存
    for(j = 0; j < count; j++) {
        delete[] piece[j];
    }
}

int DBMS::CreateDatabase(string databaseName) {
    int res = 0;
    string fullName = GetWholeName(databaseName);
    string modelPath = fullName + "\\model.md";
    string tempPath = fullName + "\\temp.tmp";
    string tempMdPath = fullName +"\\temp.md";
    ofstream fileIO;
    switch (_mkdir(fullName.c_str())) {
        case 0:
            fileIO.open(modelPath.c_str());
            fileIO.close();
            fileIO.open(tempPath.c_str());
            fileIO.close();
            fileIO.open(tempMdPath.c_str());
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
    string modelPath = this->GetWholeName(this->currentDb) + "\\model.md";
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
    ReadModel(modelPath, c);
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
    string modelPath = this->GetWholeName(this->currentDb) + "\\model.md";
    ReadModel(modelPath, c);
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
    GetTableInfo(tableName, 0);
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
    int processed = 0;  //  0表示还没有写入过temp.tmp
    int i,j,k,m;
    string table1, table2;
    string col1, col2;
    string filePath;
    vector<string> attribute;
    vector<string> table;
    vector<string> condition;
    vector<string> col;  //选择所需属性名
    vector<string> ope;  //选择所需比较符
    vector<string> val;  //选择所需值
    vector<string>::iterator p;
    /*  解析语句  */
    ParseSelect(sentence, attribute, table, condition);
    /*  有where条件 */
    if(condition.size() != 0) {
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
                    /*  等值连接  */
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
                    col.push_back(p->substr(i, j - i));
                    ope.push_back(p->substr(j + 1, k - j - 1));
                    val.push_back(p->substr(k + 1));
                }
            }
        }
        /*  条件选择  */
        if(col.size() != 0) {
            Compare(processed, table1, col, ope, val);
            processed = 1;
        }
    }
    /*  无需投影  */
    if(attribute.size() == 1 && attribute[0] == "*") {
        Display();
        return;
    }
    /*  从temp.tmp投影  */
    if(processed == 1) {
        filePath += "temp.tmp";
        this->GetTableInfo("TEMP", 1);
    }
    /*  从表文件投影  */
    else {
        filePath += table[0] + ".tab";
        this->GetTableInfo(table[0], 0);
    }
    /*  投影*/
    Projection(filePath, attribute);   
}

void DBMS::Projection(string filePath, vector<string>& attribute)
{
    list<char* > tuple;
    list<char* >::iterator charIt;
    vector<string> colName;
    vector<string>::iterator strIt;
    vector<int> index;
    char** piece;
    char* yuanzu;
    myfstream fs;
    int i,j,x,y;
    int count;
    int digit;
    int type, size, position;
    string result;
    char single[20];
    for(i = 0; i < attribute.size(); i++) {
        result += attribute[i];
        result += "\t";
    }
    result += "\n";
    
    /*  去表名  */
    for(strIt = attribute.begin(); strIt != attribute.end(); strIt++) {
        x = strIt->find(".", 0);
        x++;
        colName.push_back(strIt->substr(x));
    }
    /*  定位  */
    for(strIt = colName.begin(); strIt != colName.end(); strIt++) {
        for(i = 0; i < this->column.size(); i++) {
            if(this->column[i] == *strIt) {
                index.push_back(i);
                break;
            }
        }
    }
    //读取
    fs.open(filePath);
    count = 0;
    while (!fs.end()) {
        piece[count] = new char[piece_length];
        fs.read(piece[count]);
        int max_tuple_num_in_piece = piece_length / this->tupleLength;
        for (i = 0; i < max_tuple_num_in_piece; i++) {
            yuanzu = piece[count] + i * (this->tupleLength + 1);
            if (yuanzu[0] == '\0')
                break;
            if (yuanzu[0] == 'y') { //yuanzu  option
                tuple.push_back(yuanzu);
            }
        }
        count++;
    }
    for(charIt = tuple.begin(); charIt != tuple.end(); charIt++) {
        for(i = 0; i < index.size(); i++) {
            type = this->type[index[i]];
            size = this->size[index[i]];
            position = this->position[index[i]] + 1;
            if(type == 1) {
                for(x = 0; x < size; x++) {
                    *(reinterpret_cast<char* >(&digit) + x)= (*charIt)[position + x];
                }
                sprintf(single, "%d", digit);
                result.append(single);
                result += "\t";
            }
            else {
                for(x = 0; x < size; x++) {
                    result.append(1, (*charIt)[position + x]);
                }
                result += "\t";
            }
        }
        result += "\n";
    }
    //释放内存
    for(j = 0; j < count; j++) {
        delete[] piece[j];
    }
    cout << result << endl;
    
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

    GetTableInfo(tableName, 0);

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
    writeTable.open(tabFile.c_str(), ios::binary | ios::out | ios::in);
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

void DBMS::Equi_Join(char* tableName1, char* tableName2, char* Col1, char* Col2) {
    int tempgs[10] = {0};
    int tempnum = 0;
    string tempsxname[10];

    int duqu1[10] = {0}; //duqu1、duqu2记录读取规格
    int duqu2[10] = {0};

    int sumdatatype1[10];
    int sumdatatype2[10];

    int t1sx_hao = 1; //table1中包含的属性个数,t1sx_hao-1
    int t2sx_hao = 1; //table2中包含的属性个数

    int sx1index = 0; //sx1index、sx2index记录连接的属性在规格中的下标
    int sx2index = 0;
    bool judge1 = false; //用来检验是否找到两个属性
    bool judge2 = false;
    int datasize = 0;
    int datatype1 = 0; //用于连接的属性的类型，0表示int型，i>0表示char(i)
    int datatype2 = 0;

    string tname; //记录当前的表名
    string table1 = tableName1;
    string table2 = tableName2;
    string nowsxname; //记录当前属性名
    string datatype; //记录当前数据类型名
    string sx1 = Col1;
    string sx2 = Col2;

    
    duqu1[0] = 1;
    duqu2[0] = 1;

    fstream binaryio;
    binaryio.open("model.tab", ios::in | ios::binary | ios::out);


    char *tempchar = new char;
    char tmpchars[30] = {'\000'}; //临时的char型数组，用于逐位记录数据
    char sx_name[30] = {'\000'};
    int index = 0;

    binaryio.read(tempchar, 1);
    while (!binaryio.eof()) {    //找到名字匹配的表

        if (*tempchar == '~') { //说明下一个数据是表名或者为结束
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
                int duquindex = 1;
                binaryio.read(tempchar, 1);
                while (*tempchar != '~' && !binaryio.eof()) { //一直执行到遇到下一个‘\0'即到下一个表或者是结束
                    index = 0;

                    if (binaryio.eof())
                        break;

                    while (*tempchar != ' ') { //读取属性名
                        sx_name[index] = *tempchar;
                        index++;
                        binaryio.read(tempchar, 1);
                    }
                    sx_name[index] = '\0';
                    nowsxname = sx_name;

                    binaryio.read(tempchar, 1); //读数据类型
                    index = 0;
                    while (*tempchar != ' ' && !binaryio.eof()) {
                        tmpchars[index] = *tempchar;
                        index++;
                        binaryio.read(tempchar, 1);
                    }
                    tmpchars[index] = '\0';
                    datatype = tmpchars;

                    int value;

                    if (datatype == "char") { //如果是char型，接下来读取包含字符的个数-value
                
                        binaryio.read((char *) (&value), sizeof (value));
                        datasize = value;
                        tempsxname[tempnum] = sx_name;
                        tempgs[tempnum++] = datasize;
                        binaryio.read(tempchar, 1);

                    }

                    if (datatype == "int") { //int型
                        
                        value = 4;
                        datasize = 0;
                        tempsxname[tempnum] = sx_name;
                        tempgs[tempnum++] = datasize;
                     
                    }

                    duquindex += value;
                    duqu1[t1sx_hao] = duquindex;
                    sumdatatype1[t1sx_hao] = datasize;

                    binaryio.read(tempchar, 1);

                    if (*tempchar == '*' || *tempchar == '#') {
                        binaryio.read(tempchar, 1);
                        binaryio.read(tempchar, 1);
                    }

                    if (sx1 == nowsxname) { //找到用于连接的属性
                        sx1index = t1sx_hao;
                        judge1 = true;
                        datatype1 = datasize;
                    }
                    t1sx_hao++;
                    
                }
            }


            else if (tname == table2) { //如果该表正是要查找的表，读其属性    
                int duquindex = 1;
                binaryio.read(tempchar, 1);
                while (*tempchar != '~' && !binaryio.eof()) { //一直执行到遇到下一个‘\0'即到下一个表或者是结束
                   

                    if (binaryio.eof())
                        break;
                      index = 0;
                    while (*tempchar != ' ') { //读取属性名
                        sx_name[index] = *tempchar;
                        index++;
                        binaryio.read(tempchar, 1);
                    }
                    sx_name[index] = '\0';
                    nowsxname = sx_name;

                    binaryio.read(tempchar, 1); //读数据类型
                    index = 0;
                    while (*tempchar != ' ' && !binaryio.eof()) {
                        tmpchars[index] = *tempchar;
                        index++;
                        binaryio.read(tempchar, 1);
                    }
                    tmpchars[index] = '\0';
                    datatype = tmpchars;

                    int value;

                    if (datatype == "char") { //如果是char型，接下来读取包含字符的个数-value
                   
                        binaryio.read((char *) (&value), sizeof (value));
                        datasize = value;
                        binaryio.read(tempchar, 1);
                    }

                    if (datatype == "int") { //int型
                        value = 4;
                        datasize = 0;
                    }

                    duquindex += value;
                    duqu2[t2sx_hao] = duquindex;
                    sumdatatype2[t2sx_hao] = datasize;

                    binaryio.read(tempchar, 1);

                    if (*tempchar == '*' || *tempchar == '#') {
                        binaryio.read(tempchar, 1);
                        binaryio.read(tempchar, 1);
                    }

                    if (sx2 == nowsxname) { //找到用于连接的属性
                        sx2index = t2sx_hao;
                        datatype2 = datasize;
                        judge2 = true;
                    } else {
                        tempsxname[tempnum] = sx_name;
                        tempgs[tempnum++] = datasize;
                    }
                    t2sx_hao++;
                    
                }
            }
        } else {
            binaryio.read(tempchar, 1);
        }
    }
    
    if (!judge1 || !judge2) {
        cout << "无法找到所要查找的属性" << endl;
    }
    else if (datatype1 != datatype2) {
        cout << "属性类型不匹配" << endl;
    }
    else {
        int tmpint1, tmpint2;
        char* tmpchars1, tmpchars2;

        char *judge = new char; //用于判断元组是否被删除

        t1sx_hao--;
        t2sx_hao--;

        table1.append(".tab");
        table2.append(".tab");
        char *t1place = (char *) table1.c_str();

        int tolast1 = duqu1[t1sx_hao] - duqu1[sx1index]; //
        int tolast2 = duqu2[t2sx_hao] - duqu2[sx2index];
        int tofirst1 = duqu1[sx1index - 1] - 1;
        int tofirst2 = duqu2[sx2index - 1] - 1;
        int jump1 = duqu1[t1sx_hao] - 1;
        int jump2 = duqu2[t2sx_hao] - 1;
        char *t2place = (char *) table2.c_str();

        char *yuanzu = new char;
        char tempchars1[100], tempchars2[100]; //用于char*跟string的转换
        int value1, value2;

        string t1_string, t2_string;

        fstream t1, temp;

        if (datatype1 == 0) { //如果连接类型是整型
         
            t1.open(t1place, ios::in | ios::binary);
            temp.open("temp.tab", ios::out | ios::binary);


            t1.read(judge, 1);      //读取标志位
            while (!t1.eof()) {
                
                if (judge[0] == 'N') { //标志删除则跳过该元组
                    t1.seekg(jump1, ios::cur);
                    continue;
                }
                else { //未删除则读取该属性值
                    t1.seekg(tofirst1, ios::cur);
                    t1.read(reinterpret_cast<char *> (&value1), sizeof (int));
                    t1.seekg(tolast1, ios::cur);

                    fstream t2;
                    t2.open(t2place, ios::in | ios::binary);
                    t2.seekg(0L, ios::beg);
                    t2.read(judge, 1);
                    while (!t2.eof()) {

                        //  t2.read(judge,1);           //读取标志位
                        if (judge[0] == 'N') { //标志删除则跳过该元组
                            t2.seekg(jump2, ios::cur);
                            continue;
                        }
                        else { //未删除则读取该属性值
                            t2.seekg(tofirst2, ios::cur);
                            t2.read(reinterpret_cast<char *> (&value2), sizeof (int));
                            t2.seekg(tolast2, ios::cur);
                        }

                        if (value1 == value2) { //如果整型连接属性相等
                            t1.seekg(-jump1, ios::cur);
                            t1.read(yuanzu, jump1);
                            temp.write(yuanzu, jump1);

                            t2.seekg(-jump2, ios::cur);
                            t2.read(yuanzu, tofirst2);
                            temp.write(yuanzu, tofirst2);
                            t2.seekg(4, ios::cur);
                            t2.read(yuanzu, tolast2);
                            temp.write(yuanzu, tolast2);
                        }
                        t2.read(judge, 1);
                    }
                    t1.read(judge, 1); //读取标志位
                    t2.close();
                }
              
            }
            t1.close();
            temp.close();
        }
        else {


            t1.open(t1place, ios::in | ios::binary);
            temp.open("temp.tab", ios::out | ios::binary);
            temp.seekg(0, ios::beg);
        
            t1.read(judge, 1); //读取标志位
            while (!t1.eof()) {
                if (judge[0] == 'N') { //标志删除则跳过该元组
                    t1.seekg(jump1, ios::cur);
                    continue;
                } else { //未删除则读取该属性值
                    t1.seekg(tofirst1, ios::cur);
                    t1.read(tempchars1, datatype1);
                    tempchars1[datatype1] = '\0';
                    t1.seekg(tolast1, ios::cur);
                    t1_string = tempchars1;



                    fstream t2;
                    t2.open(t2place, ios::in | ios::binary);
                    t2.seekg(0L, ios::beg);
                    t2.read(judge, 1); //读取标志位

                    while (!t2.eof()) {

                        if (judge[0] == 'N') { //标志删除则跳过该元组
                            t2.seekg(jump2, ios::cur);
                            continue;
                        }
                        else if (judge[0] == 'Y') { //未删除则读取该属性值
                            t2.seekg(tofirst2, ios::cur);
                            t2.read(tempchars2, datatype1);
                            tempchars2[datatype1] = '\0';
                            t2.seekg(tolast2, ios::cur);
                            t2_string = tempchars2;
                        }

                        if (t1_string == t2_string) { //如果cahr型连接属性相等
                            t1.seekg(-jump1, ios::cur);
                            t1.read(yuanzu, jump1);
                            temp.write(yuanzu, jump1);

                            t2.seekg(-jump2, ios::cur);
                            t2.read(yuanzu, tofirst2);
                            temp.write(yuanzu, tofirst2);
                            t2.seekg(datatype1, ios::cur);
                            t2.read(yuanzu, tolast2);
                            temp.write(yuanzu, tolast2);
                        }


                        t2.read(judge, 1); //读取标志位
                    }
                    t1.read(judge, 1); //读取标志位
                    t2.close();
                }
            }
            t1.close();
            temp.close();



        }

        delete judge;
        delete yuanzu;
        delete t1place;
        delete t2place;

        fstream addtemp;
        addtemp.open("tempgs.tab", ios::in | ios::out | ios::binary);
        char *tmps = new char;

        char s[10] = {'~', 'T', 'E', 'M', 'P', ' '};
        addtemp.write(s, 6);
        for (int t = 0; t < tempnum; t++) {

            tmps = (char *) tempsxname[t].c_str();
           
            addtemp.write(tmps, tempsxname[t].length());

            if (tempgs[t] == 0) { //是int型
                tmps = " int # ";
                addtemp.write(tmps, 7);

            }

            else {
                tmps = " char ";
                addtemp.write(tmps, 6);
                int value = tempgs[t];
                addtemp.write(reinterpret_cast<char *> (&value), sizeof (value));
                tmps = " # ";
                addtemp.write(tmps, 3);
            }
        }
        delete tmps;
    }
    delete tempchar;
}

void DBMS::Display()
{
    
}
