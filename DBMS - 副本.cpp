/* 
 * File:   DBMS.cpp
 * Author: Administrator
 * 
 * Created on 2012年10月15日, 下午2:49
 */


#include <bits/basic_string.h>

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
void DBMS::SetDirPath(string path) 
{
    this->dirPath = path;
}

string DBMS::Replace(string src, string symbol, string r)
{
    string::size_type i;
    int count;
    i = 0;
    while(true) {
        i = src.find(symbol, i);
        if(i != string::npos) {
            i++;
            count = 0;
            while(src[i + count] == symbol[0]) count++;
            if(count > 0) {
                src.replace(i, count, r);
            }
        }
        else {
            break;
        }
    }
    if(src[0] == symbol[0]) {
        src.erase(0, 1);
    }
    if(src[src.length() - 1] == symbol[0]) {
        src.erase(src.length() - 1, 1);
    }
    return src;
    
}

string DBMS::GetWholeName(string suffix) 
{
    return (this->dirPath + "\\" + suffix);    
}

bool DBMS::IsDigit(string str)
{
    for(int i = 0; i < str.length(); i++) {
        if(str[i] < '0' || str[i] > '9')
            return false;
    }
    return true;
}

string DBMS::Parse(string words)
{
    string::size_type i = 0;
    int count = 0;
    while(true) {
        i = words.find('(', i);
        if(i != string::npos) {
            words.replace(i, 1, 1, ' ');
        }
        else {
            break;
        }
    }
    i = 0;
    while(true) {
        i = words.find(')', i);
        if(i != string::npos) {
            words.replace(i, 1, 1, ' ');
        }
        else {
            break;
        }
    }
    i = 0;
    while(true) {
        i = words.find(',', i);
        if(i != string::npos) {
            words.replace(i, 1, 1, ' ');
        }
        else {
            break;
        }
    }
    i = 0;
    while(true) {
        i = words.find(' ', i);
        
        if(i != string::npos) {
            i++;
            count = 0;
            while(words[i + count] == ' ') {
                count++;
            }
            if(count > 0) {
                words.erase(i, count);
            }
        }
        else {
            if(words[0] == ' ') {
                words.erase(0, 1);
            }
            if(words[words.length() - 1] == ' ') {
                words.erase(words.length() - 1, 1);
            }
            break;
        }
    }
    
    return words;
    
}

void DBMS::ParseSelect(string sentence, string& select)
{
    //select stu.name, tea.name from stu, tea where stu.name = 'w' and tea.id > 10
    string::size_type i = 0, j = 0, k = 0;
    i = sentence.find("select", 0);
    if(i != string::npos) {
        j = sentence.find("from", 0);
        if(j != string::npos) {
            select = sentence.substr(i + 6, j - i - 6);
        }
    }
    i = 0;
    while(true) {
        i = select.find(" ", i);
        if(i != string::npos) {
            select.erase(i, 1);
        }
        else {
            break;
        }
    }
    
//    boost::regex selectExpr(".+from");
//    boost::smatch m;
//    if(boost::regex_match(sentence, m, selectExpr) == true) {
//        
//        cout << m[0].str() << endl;
//        cout << m.str(0) << endl;
//        cout << m.str(1) << endl;
//    }
    
    
//    string::size_type i = 0, j = 0, k = 0;
//    vector<string> vTable;
//    vector<string> vColumn;
//    vector<string>::iterator it;
//    
//    i = sentence.find("from ", i);
//    if(i != string::npos) {
//        select = sentence.substr(0, i);
//        
//        j = sentence.find("where ", i);
//        if(j != string::npos) {
//            i += 4;
//            from = sentence.substr(i, j - i);
//
//            j += 5;
//            where = sentence.substr(j);
//        }
//    }
//    else {
//        //  句子不对
//        return;
//    }
//    /*  select内容 "table1.colunm1 table1.column2 table2.column1"
//     *  from内容 "table1 table2"
//     *  where内容 "(table1.column3 >= table2.column1) and (table2.column2 = 10)"
//     */
//    select = Replace(select, " ", "");
//    from = Replace(from, " ", "");
//    where = Replace(where, " ", "");
//    //  where条件去括号
//    i = 0;
//    while(true) {
//        i = where.find("(", i);
//        if(i != string::npos) {
//            where.replace(i, 1, " ");
//        }
//        else {
//            break;
//        }
//    }
//    i = 0;
//    while(true) {
//        i = where.find(")", i);
//        if(i != string::npos) {
//            where.replace(i, 1, " ");
//        }
//        else {
//            break;
//        }
//    }
//    //  所需列入vColumn
//    i = 0;
//    j = 0;
//    while(true) {
//        j = select.find(",", i);
//        if(j != string::npos) {
//            vColumn.push_back(select.substr(i, j - i));
//            i = j + 1;
//        }
//        
//        else {
//            vColumn.push_back(select.substr(i));
//            break;
//        }
//    }
//    
//    i = 0;
//    j = 0;
//    k = 0;
//    while(true) {
//        j = where.find(".", i);
//        if(j != string::npos) {
//           
//            k = where.rfind(" ", j);
//            j++;
//            if(k != string::npos) {
//                i = where.find(" ", j);
//                if(i != string::npos) {
//                    vColumn.push_back(where.substr(k, i - k - 1));
//                    i++;
//                }
//                
//            }
//            else {
//                k = i;
//                i = where.find(" ", j);
//                if(i != string::npos) {
//                    vColumn.push_back(where.substr(k, i - k));
//                    i++;
//                }
//            }
//            
//        }
//        else {
//            break;
//        }
//    }
//    //  遍历vColumn去空格
//    it = vColumn.begin();
//    while(it != vColumn.end()) {
//        i = 0;
//        while(true) {
//            i = it->find(" ", i);
//            if(i != string::npos) {
//                it->erase(i, 1);
//            }
//            else {
//                break;
//            }
//        }
//    }
//    //  vColumn去重
//    sort(vColumn.begin(), vColumn.end());
//    it = unique(vColumn.begin(), vColumn.end());
//    vColumn.erase(it, vColumn.end());
//    //////
//    it = vColumn.begin();
//    while(it != vColumn.end()) {
//        cout << *it << " ";
//    }
    
    
    
    
    
}

void DBMS::ParseFrom(string sentence, string& from)
{
    string::size_type i = 0, j = 0, k = 0;
    i = sentence.find("from", 0);
    if(i != string::npos) {
        j = sentence.find("where", 0);
        if(j != string::npos) {
            from = sentence.substr(i + 4, j - i - 4);
        }
        else {
            from = sentence.substr(i + 4);
        }
    }    
    i = 0;
    while(true) {
        i = from.find(" ", i);
        if(i != string::npos) {
            from.erase(i, 1);
        }
        else {
            break;
        }
    }
}

void DBMS::ParseInsert(string sentence, string& tableFileName, int& tableSize, 
                       paralist* list, int primaryKeyIndex[], int& primaryKeySize)
{
    string::size_type i = 0, j = 0, k = 0;
    string modelPath = GetWholeName(currentDb) + "\\model.md";
    fstream fileIO;
    char content[1024];
    int fileSize = 0;
    int typeSize = 0;
    int listSize = 0;
    string values = "";
    int positionList[10];
    int valueSize[10];
    int columns = 0;
    primaryKeySize = 0;
    string container = "";
    string table = "";
    string temp = "";
    sentence = Replace(sentence, " ", "");
    
    i = sentence.find("insert into", 0);
    if(i != string::npos) {
        i = 10;
        j = sentence.find(" ", i);
        if(j != string::npos) {
            j++;
            k = sentence.find(" ", j);
            if(k != string::npos) {
                tableFileName = sentence.substr(j, k - j);
                fileIO.open(modelPath.c_str(), ios::binary | ios::in);
                fileIO.seekg(0, ios::end);
                fileSize = fileIO.tellg();

                fileIO.seekg(0, ios::beg);
                fileIO.read(content, fileSize);
                fileIO.close();

                container.insert(0, content, fileSize);
                int m = 0, n = 0;
                while(true) {
                    m = container.find('\0', m);
                    if(m != string::npos) {
                        m++;
                        n = container.find('\0', m);
                        if(n != string::npos) {
                            table = container.substr(m, n - m);
                        }
                        else {
                            table = container.substr(m);
                        }
                        if(table.substr(0, tableFileName.length()) == tableFileName) {
                            /*  插入位置,字节大小*/
                            i = 0;
                            while(true) {
                                i = table.find(" ", i);
                                if(i != string::npos) {
                                    i++;
                                    j = table.find(" ", i);
                                    if(j != string::npos) {
                                        temp = table.substr(i, j - i);
                                    }
                                    else {
                                        temp = table.substr(i);
                                    }
                                    if(IsDigit(temp)) {

                                        typeSize = atoi(temp.c_str());
                                        tableSize += typeSize;
                                        listSize++;
                                        positionList[listSize] = tableSize;
                                        valueSize[listSize - 1] = positionList[listSize] - positionList[listSize - 1];

                                        continue;
                                    }
                                    else if(temp == "char") {
                                        columns++;
                                        continue;
                                    }
                                    else if(temp == "*") {
                                        primaryKeyIndex[primaryKeySize++] = columns - 1;
                                    }
                                    else if(temp == "int") {
                                        columns++;
                                        typeSize = 1;
                                        valueSize[listSize] = typeSize;
                                        tableSize += typeSize;
                                        listSize++;
                                        positionList[listSize] = tableSize;
                                        valueSize[listSize - 1] = positionList[listSize] - positionList[listSize - 1];
                                        listSize++;
                                        continue;
                                    }

                                }
                                else {
                                    break;
                                }
                            }
                            break;
                        }
                        else {
                            continue;
                        }
                        
                    }
                    else {
                        return;
                    }
                }
    
                
            }
        }
        
    }
    //  插入内容
    i = sentence.find("(", 0);
    if(i != string::npos) {
        i++;
        j = sentence.find(")", i);
        if(j != string::npos) {
            values = sentence.substr(i, j - i);
        }
        else {
            return;
        }
    }
    i = 0;
    while(true) {
        i = values.find(",", i);
        if(i != string::npos) {
            values.replace(i, 1, " ");
        }
        else {
            break;
        }
    }
    values = Replace(values, " ", "");
    i = 0;
    int count = 0;
    while(true) {
        j = values.find(" ", i);
        if(j != string::npos) {
            list->add((char*)(values.substr(i, j - i).c_str()), positionList[count], valueSize[count]);
            i = j + 1;
            count++;
        }
        else {
            list->add(((char*)values.substr(i).c_str()), positionList[count], valueSize[count]);
        }
    }
    
    tableFileName.insert(0, GetWholeName(currentDb).append("\\"));
    tableFileName.append(".tab");
    
}

void DBMS::GetTableInfo(string tableName)
{
    if(currentDb.length() == 0) {
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
    
    string::size_type i,j,k;
    string tableInfo;
    string temp;
   
    i = 0;
    while(1) {
        i = content.find('\0', i);
        if(i != string::npos) {
            i++;
            j = content.find(' ', i);
            if(j != string::npos) {
                tableInfo = content.substr(i, j - i);
                if(tableInfo == currentTable) {
                    k = content.find('\0', i);
                    if(k != string::npos) {
                        tableInfo = content.substr(i, k - i);
                        tableInfo.erase(0, currentTable.length());
                        tableInfo.append(' ');
                    }
                    else
                        tableInfo = content.substr(i);
                }
                else continue;
            }
        }
        else break;
    }
    
    i = 0;
    while(1) {
        i = tableInfo.find(' ', i);
        if(i != string::npos) {
            i++;
            j = tableInfo.find(' ', i);
            if(j != string::npos) {
                temp = tableInfo.substr(i, j - i);
                if(temp == "char") {
                    
                }
                else if(temp == "int") {
                    
                }
                else if()
            }
        }
        else break;
    }
    
    
    
}

void DBMS::ReadModel(string tableName, string& content)
{
    char c[1024];
    string modelPath = GetWholeName(currentDb) + "\\model.md";
    fstream fileIO;
    fileIO.open(modelPath.c_str(), ios::binary | ios::in);
    int fileSize = 0;  //model.md的大小
    fileIO.seekg(0, ios::end);
    fileSize = fileIO.tellg();

    fileIO.seekg(0, ios::beg);
    fileIO.read(c, fileSize);
    fileIO.close();
    content.append(c, fileSize);
}

int DBMS::CreateDatabase(string databaseName)
{
    int res = 0;
    string fullName = GetWholeName(databaseName);
    string modelPath = fullName + "\\model.md";
    string tempPath = fullName + "\\temp.tmp";
    ofstream fileIO;
    switch(_mkdir(fullName.c_str())) {
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

int DBMS::DropDatabase(string databaseName)
{
    string str = "rmdir /s /q ";
    str.append(GetWholeName(databaseName));
    system(str.c_str());
    return 0;
//    intptr_t handle;
//    struct _finddata_t fileInfo;
//    string fullName = GetWholeName(databaseName);
//    handle = _findfirst(fullName.c_str(), &fileInfo);
//    _findclose(handle);
//    if(handle == -1L) {
//        return 1;
//    }
    
    
}

void DBMS::ShowDatabases()
{
    string dbnames;
    intptr_t handle;
    struct _finddata_t fileInfo;
    string fullName = GetWholeName("*");
    handle = _findfirst(fullName.c_str(), &fileInfo);
    if(handle != -1L) {
        do {
            if(fileInfo.attrib & _A_SUBDIR) {
                if( (strcmp(fileInfo.name,".") != 0 ) 
                        && (strcmp(fileInfo.name,"..") != 0)) {
                    dbnames = fileInfo.name;
                    cout << dbnames << endl;        
                }
            }    
        } while(_findnext(handle, &fileInfo) == 0);
        _findclose(handle);
        
    }
    
}

void DBMS::Use(string databaseName)
{
    intptr_t handle;
    struct _finddata_t fileInfo;
    string fullName = GetWholeName("*");
    handle = _findfirst(fullName.c_str(), &fileInfo);
    if(handle != -1L) {
        do {
            if((strcmp(fileInfo.name, databaseName.c_str()) == 0)) {
                currentDb = databaseName;
                
                ///////////////////
                cout << "chosen database: " << currentDb << endl;
            }
                
        } while(_findnext(handle, &fileInfo) == 0);
        _findclose(handle);
        
    }
}

int DBMS::CreateTable(string sentence)
{
    string tableFile;
    char content[1024];
    int res = 0;  
    int fileSize = 0;  //model.md的大小
    bool exist = false;  //表是否存在
    const char zero = 0;  //'\0'
    string::size_type i = 0;
    const string primaryKey = "primary key";
    const char star = '*';
    
    
    if(currentDb.length() == 0) {
        cout << "choose a database first!" << endl;
        res = 1;
        return res;
    }
    
    sentence = Parse(sentence);
    int length = sentence.length();
    string modelPath = GetWholeName(currentDb) + "\\model.md";
    fstream fileIO;
    fileIO.open(modelPath.c_str(), ios::binary | ios::in | ios::out);
    
    int nameLength = 0;
    while(sentence[nameLength] != ' ') nameLength++;
    
    
    fileIO.seekg(0, ios::end);
    fileSize = fileIO.tellg();

    fileIO.seekg(0, ios::beg);
    fileIO.read(content, fileSize);
    for(int j = 0; j < fileSize; j++) {
        if(content[j] == zero) {
            j++;
            int k = 0;
            while(k < nameLength && content[j + k] == sentence[k]) k++;
            if(k == nameLength) {
                exist = true;
                break;
            }
           
        }
    }
    if(fileSize == 0 || exist == false) {
        /*  查找关键字primary key 然后替换为星号*  */
        i = 0;
        while(true) {
            i = sentence.find(primaryKey, i);
            if(i != string::npos) {
                sentence.replace(i, primaryKey.length(), 1, star);
                i++;
            }
            else {
                break;
            } 
        }
        /*  写入model.md  */
        sentence = Parse(sentence);
        length = sentence.length();
        fileIO.seekp(0, ios::end);
        fileIO.put(zero);
        fileIO.write(sentence.c_str(), length);
        fileIO.close();
        
        /*  创建tab文件供元组信息存放  */
        tableFile = GetWholeName(currentDb);
        tableFile += "\\";
        tableFile.append(sentence.substr(0, nameLength));
        tableFile += ".tab";
        fileIO.open(tableFile.c_str(), ios::binary | ios::out);
        fileIO.close();
        
        res = 0;
    
    }
    else {
        res = TABLEEXISTED;
    }
    
    return res;

}

int DBMS::DropTable(string tableName) 
{
    string tableFile = "";
    string modelPath = "";
    string container = "";
    fstream fileIO;
    char content[1024];
    int res = 0;  
    int i = 0;
    int j = 0;
    int index = 0;
    const char zero = 0;
    int fileSize = 0;  //  model.md的大小
    
    if(currentDb.length() == 0) {
        res = NOTUSE;  
        return res;
    }
    
    modelPath = GetWholeName(currentDb) + "\\model.md";
    
    fileIO.open(modelPath.c_str(), ios::binary | ios::in);
    fileIO.seekg(0, ios::end);
    fileSize = fileIO.tellg();
    fileIO.seekg(0, ios::beg);
    fileIO.read(content, fileSize);
    fileIO.close();
    
    container.insert(0, content, fileSize);
    i = 0;
    while(true) {
        i = container.find(zero, i);
        index = i;
        if(i != string::npos) {
            i++;
            i = container.find(tableName, i);
            if(i != string::npos) {
                j = container.find(zero, i);
                if(j != string::npos) {
                    container.erase(index, j - index);
                }
                else {
                    container.erase(index);
                }
                
                fileIO.open(modelPath.c_str(), ios::binary | ios::out);
                fileIO.write(container.c_str(), container.length());
                fileIO.close();
                res = 0;
                break;
            }
            else {
                continue;
            }
        }
        else {
            res = NOTEXIST;
            break;
        }   
    }
    
    tableFile = GetWholeName(currentDb + "\\" + tableName + ".tab");
    tableFile.insert(0, "del ");
    system(tableFile.c_str());  
    
    return res;
    
    
}

void DBMS::Describe(string tableName)
{
    if(currentDb.length() == 0) {
        cout << "choose a database first!" << endl;
        return;
    }
    int j = 0, k = 0, m = 0;
    int nameLength = tableName.length();
    char zero = 0;
    char content[1024];
    int fileSize = 0;
    string sub = "";
    string description = tableName;
    string modelPath = GetWholeName(currentDb) + "\\model.md";
    fstream modelReader;
    modelReader.open(modelPath.c_str(), ios::binary | ios::in);
    modelReader.seekg(0, ios::end);
    fileSize = modelReader.tellg();
    
    modelReader.seekg(0, ios::beg);
    modelReader.read(content, fileSize);
    modelReader.close();
    fileSize--;
    for(j = 0; j < fileSize; j++) {
         if(content[j] == zero) {
             j++;
             k = 0;
             while(k < nameLength && content[j + k] == tableName[k]) k++;
             
             
             if(k == nameLength) {
                 m = j + k;
                 while(content[m] != '\0' && m <= fileSize) {
                     description.append(1, content[m]);
                     m++;
                 }
                 
                 break;
            }
         }
    }
    if(description.length() == nameLength) {
        description.append(" is not existed");
    }
    else {
        j = 0;
        k = 0;
//        while(true) {
//            j = description.find("*", 0);
//            if(j != string::npos) {
//                description.replace(j, 1, "primary key");
//            }
//        }
        m = description.length();
        while(j < m) {
            k = description.find(' ', j);
            if(k > 0 && k < m) {
                sub = description.substr(j, k - j);
                if(sub == "char") {
                    description.replace(k, 1, "(");
                }
                else if(IsDigit(sub) == true) {
                    description.replace(k, 1, ")");
                }
                else if(sub == "*") {
                    description.replace(k - 1, 1, "primary key");
                }
                j = k + 1;
            }
            else {
                sub = description.substr(j);
                if(IsDigit(sub) == true) {
                    description.insert(j, "(");
                    description.append(")");
                }
                break;
            }
                
        } 
    }
    
    cout << description << endl;
   
}

void DBMS::ShowTables()
{
    if(currentDb.length() == 0) {
        cout << "choose a database first!" << endl;
        return;
    }
    
    const char end = 0;
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
    
    if(fileSize != 0) {
        fileIO.seekg(0, ios::beg);
        fileIO.read(content, fileSize);
        fileIO.close();

        container.insert(0, content, fileSize);

        int i = 0, j = 0;
        while(true) {
            i = container.find(end, i);
            if(i != string::npos) {
                i++;
                j = container.find(space, i);
                if(j != string::npos) {
                    cout << container.substr(i, j - i) << endl;
                }                
            }
            else {
                return;
            }
        }
    }
    else {
        cout << "no any table!" << endl;
    }   
}

void DBMS::Select(string sentence)
{
    if(currentDb.length() == 0) {
        cout << "choose a database first!" << endl;
        return;
    }
    char content[1024];
    string container = "";
    int fileSize;
    string table = "";
    string temp = "";
    int typeSize = 0;
    int tableSize = 0;
    int positionList[10];
    int valueSize[10];
    int listSize = 0;
    string modelPath = GetWholeName(currentDb) + "\\model.md";
    string filePath = GetWholeName(currentDb);
    fstream fileIO;
    string select = "";
    string from = "";
    string where = "";
    string::size_type i, j;
    vector<string> vTable;
    vector<string> vColumn;
    vector<int> vSize;
    /*  select内容 "table1.colunm1,table1.column2,table2.column1"
     *  from内容 "table1,table2"
     *  where内容 "(table1.column3 >= table2.column1) and (table2.column2 = 10)"
     */
    
    ParseSelect(sentence, select);
    ParseFrom(sentence, from);
    i = 0;
    while(true) {
        j = from.find(",", i);
        if(j != string::npos) {
            vTable.push_back(from.substr(i, j - i));
            i = j + 1;
        }
        else {
            vTable.push_back(from.substr(i));
            break;
        }
    }
    i = 0;
    while(true) {
        j = select.find(",", i);
        if(j != string::npos) {
            vColumn.push_back(select.substr(i, j - i));
            i = j + 1;
        }
        else {
            vColumn.push_back(select.substr(i));
            break;
        }
    }
    if(vTable.size() == 1) {
        if(vColumn[0] == "*") {
            vColumn.clear();
            fileIO.open(modelPath.c_str(), ios::binary | ios::in);
            fileIO.seekg(0, ios::end);
            fileSize = fileIO.tellg();

            fileIO.seekg(0, ios::beg);
            fileIO.read(content, fileSize);
            fileIO.close();
            
            container.insert(0, content, fileSize);
            i = 0;
            i = container.find("\0" + vTable[0], 0);
            if(i != string::npos) {
                i++;
                j = container.find("\0", i);
                if(j != string::npos) {
                    table = container.substr(i, j - i);
                }
                i = 0;
                positionList[listSize] = 0;
                while(true) {
                    i = table.find(" ", i);
                    if(i != string::npos) {
                        i++;
                        j = table.find(" ", i);
                        if(j != string::npos) {
                            temp = table.substr(i, j - i);
                        }
                        else {
                            temp = table.substr(i);
                        }
                        if(IsDigit(temp)) {
                            
                            typeSize = atoi(temp.c_str());
                            tableSize += typeSize;
                            listSize++;
                            positionList[listSize] = tableSize;
                            valueSize[listSize - 1] = positionList[listSize] - positionList[listSize - 1];
                            vSize.push_back(typeSize);
                            
                            continue;
                        }
                        else if(temp == "char" || temp == "*") {
                            continue;
                        }
                        else if(temp == "int") {
                            typeSize = 1;
                            valueSize[listSize] = typeSize;
                            tableSize += typeSize;
                            listSize++;
                            positionList[listSize] = tableSize;
                            valueSize[listSize - 1] = positionList[listSize] - positionList[listSize - 1];
                            vSize.push_back(typeSize);
                            listSize++;
                            continue;
                        }
                        else vColumn.push_back(temp);
                    }
                    else {
                        break;
                    }
                }
            }
            filePath += ("\\" + vTable[0] + ".tab");
            Projection((char*)filePath.c_str(), tableSize, positionList, valueSize, listSize);
            
        }
            
    }
    
    
}

void DBMS::Projection(char* tableFileName, int tableSize, int* positionList, 
                      int* valueSize, int list_size) 
{
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

void DBMS::Insert(string words) 
{
    string tableFileName; 
    int tableSize;
    paralist* list; 
    int primaryKeyIndex[10]; 
    int primaryKeySize;
    
    ParseInsert(words, tableFileName, tableSize, list, primaryKeyIndex, primaryKeySize);
    
    fstream writeTable;
    writeTable.open(tableFileName.c_str(), ios::in | ios::out | ios::binary);

    writeTable.seekp(0, ios::end);
    int FileLength = writeTable.tellp();

    int Num_Of_Yuanzu = maxReadSize / (tableSize + 1);
    int readSize = Num_Of_Yuanzu * (tableSize + 1);

    int accumulate = 0;
    writeTable.seekg(0);
    while (1) {
        int distance = FileLength - writeTable.tellg();

        if (distance <= readSize) {
            char* tempPiece = new char[readSize + 1];
            char* yuanzu = new char[tableSize + 1];
            writeTable.read(tempPiece, distance); //get pieces
            //option
            //  get i yuanzu
            int num_of_yuanzu = distance / (tableSize + 1);
            for (int i = 0; i < num_of_yuanzu; i++) {
                yuanzu = tempPiece + i * (tableSize + 1);

                bool same = true;
                if (yuanzu[0] == 'y') { //yuanzu  option
                    for (int j = 0; j < primaryKeySize; j++) { //check  primaryKey
                        int index = primaryKeyIndex[j];
                        int position = list->getPosition(index);
                        string parameterValue = list->getValue(index);
                        string key = yuanzu + position + 1;
                        if (key != parameterValue) {
                            same = false;
                            break;
                        }
                    }
                }
                if (same) { //if same  ,quit
                    delete[] yuanzu;
                    delete[] tempPiece;
                    return;
                }
            }
            delete[] yuanzu;
            delete[] tempPiece;
            break;
        } else {
            char* tempPiece = new char[readSize + 1];
            char* yuanzu = new char[tableSize + 1];
            writeTable.read(tempPiece, readSize);
            //  piece  option
            for (int i = 0; i < Num_Of_Yuanzu; i++) {
                yuanzu = tempPiece + i * (tableSize + 1);
                //yuanzu  option
                bool same = true;
                if (yuanzu[0] == 'y') { //yuanzu  option
                    for (int j = 0; j < primaryKeySize; j++) { //check  primaryKey
                        int index = primaryKeyIndex[j];
                        int position = list->getPosition(index);
                        string parameterValue = list->getValue(index);
                        string key = yuanzu + position + 1;
                        if (key != parameterValue) {
                            same = false;
                            break;
                        }
                    }
                }
                if (same) { //if same  ,quit
                    delete[] yuanzu;
                    delete[] tempPiece;
                    return;
                }
            }
            delete[] yuanzu;
            delete[] tempPiece;
            accumulate++;
            writeTable.seekg(accumulate * readSize);
        }
    }

    writeTable.seekp(0, ios::end);
    writeTable.put('y');
    for (int m = 0; m < list->getSize(); m++) {
        writeTable.write(list->getValue(m), list->getValueSize(m));
    }

    writeTable.close();



}

void DBMS::Equi_Join(char* tableName1, char* tableName2, char* Col1, char* Col2)
{
    int duqu1[6]={0};           //duqu1、duqu2记录读取规格
    int duqu2[6]={0};

    int sumdatatype1[10];
    int sumdatatype2[10];

    int t1sx_hao=1;         //table1中包含的属性个数,t1sx_hao-1
    int t2sx_hao=1;          //table2中包含的属性个数
    //int colnum=0;
    //int cols[6]={0};
    int sx1index=0;    //sx1index、sx2index记录连接的属性在规格中的下标
    int sx2index=0;
    bool judge1=false;   //用来检验是否找到两个属性
    bool judge2=false;
    int datatypes=0;
    int datatype1=0;             //用于连接的属性的类型，0表示int型，i>0表示char(i)
    int datatype2=0;
    //int sx1num;                 //table1中包含的属性个数
    //int sx2num;                 //table2中包含的属性个数

    string tname;            //记录当前的表名
    string table1=tableName1;
    string table2=tableName2;
    string nowsxname;     //记录当前属性名
    string datatype;        //记录当前数据类型名
    string sx1=Col1;
    string sx2=Col2;
   // int duquindex=1;
    duqu1[0]=1;
    duqu2[0]=1;

 fstream binaryio;
  binaryio.open("E:/temp.dat", ios::in | ios::binary );


  char *tempchar=new char;
  char tmpchars[30]={'\000'};   //临时的char型数组，用于逐位记录数据
  int index=0;

  binaryio.read(tempchar,1);
  while(!binaryio.eof()){   //找到名字匹配的表

      if(*tempchar=='\0'){        //说明下一个数据是表名或者为结束
         binaryio.read(tempchar,1);
          if(binaryio.eof()){
              break;
          }
          index=0;
      while(*tempchar!=' '){          //读表名

          tmpchars[index]=*tempchar;
          index++;
          binaryio.read(tempchar,1);
           }
          tmpchars[index]='\0';
          tname=tmpchars;


     if(tname==table1){     //如果该表正是要查找的表，读其属性
      //binaryio.read(tempchar,1);
     // binaryio.read(tempchar,1);
         int duquindex=1;
      while (*tempchar!='\0'){         //一直执行到遇到下一个‘\0'即到下一个表或者是结束
      index=0;

     // while (tempchar!='\0'){
      binaryio.read(tempchar,1);
      while(*tempchar!=' '){           //读取属性名
          tmpchars[index]=*tempchar;
          index++;
          binaryio.read(tempchar,1);
           }
      tmpchars[index]='\0';
      nowsxname=tmpchars;


      binaryio.read(tempchar,1);   //读数据类型
      index=0;
      while(*tempchar!=' '&&*tempchar!='\0'){
          tmpchars[index]=*tempchar;
          index++;
          binaryio.read(tempchar,1);
           }
       tmpchars[index]='\0';
       datatype=tmpchars;
       
       int value;

       if(datatype=="char"){      //如果是char型，接下来读取包含字符的个数-value
           cout <<"zai char li"<<endl;
           //binaryio.read(tempchar,1);
           binaryio.read((char *)(&value), sizeof(value));
           datatypes=value;
           binaryio.read(tempchar,1);

       }

       if(datatype=="int"){     //int型
           cout<<"zai int li"<<endl;
           value=4;
           datatypes=0;

       //    binaryio.read(tempchar,1);
       }
       duquindex+=value;
       duqu1[t1sx_hao]=duquindex;
       sumdatatype1[t1sx_hao]=datatypes;

       if(sx1==nowsxname){     //找到用于连接的属性
           sx1index=t1sx_hao;
           judge1=true;
           datatype1=datatypes;
       }
       t1sx_hao++;
      //binaryio.read(tempchar,1);
      }
     }


     else if(tname==table2){     //如果该表正是要查找的表，读其属性
      //binaryio.read(tempchar,1);
     // binaryio.read(tempchar,1);
         int duquindex=1;
      while (*tempchar!='\0'){         //一直执行到遇到下一个‘\0'即到下一个表或者是结束
      index=0;

     // while (tempchar!='\0'){
      binaryio.read(tempchar,1);
      while(*tempchar!=' '){           //读取属性名
          tmpchars[index]=*tempchar;
          index++;
          binaryio.read(tempchar,1);
           }
      tmpchars[index]='\0';
      nowsxname=tmpchars;


      binaryio.read(tempchar,1);   //读数据类型
      index=0;
      while(*tempchar!=' '&&*tempchar!='\0'){
          tmpchars[index]=*tempchar;
          index++;
          binaryio.read(tempchar,1);
           }
       tmpchars[index]='\0';
       datatype=tmpchars;
       
       int value;

       if(datatype=="char"){      //如果是char型，接下来读取包含字符的个数-value
           cout <<"zai char li"<<endl;
           //binaryio.read(tempchar,1);
           binaryio.read((char *)(&value), sizeof(value));
           datatypes=value;

         binaryio.read(tempchar,1);
       }

       if(datatype=="int"){     //int型
           cout<<"zai int li"<<endl;
           value=4;

           datatypes=0;
       //    binaryio.read(tempchar,1);
       }
       duquindex+=value;
       duqu2[t2sx_hao]=duquindex;
       sumdatatype2[t2sx_hao]=datatypes;

       if(sx2==nowsxname){     //找到用于连接的属性
           sx2index=t2sx_hao;
           datatype2=datatypes;
           judge2=true;
       }
       t2sx_hao++;
      //binaryio.read(tempchar,1);
      }
     }
      }
      else{
          binaryio.read(tempchar,1);
      }
  }
 // cout<<"axyz"<<endl;

  if(!judge1||!judge2){
      cout<<"无法找到所要查找的属性"<<endl;
  }
  else if(datatype1!=datatype2){
      cout<<"属性类型不匹配"<<endl;
  }

  else{
      int tmpint1,tmpint2;
      char* tmpchars1,tmpchars2;
      int table1num=0;             //table元组个数
      int table2num=0;

      t1sx_hao--;
      t2sx_hao--;

      int tolast1=duqu1[t1sx_hao]-duqu1[sx1index];
      int tolast2=duqu2[t2sx_hao]-duqu2[sx2index];
      int tofirst1=duqu1[sx1index-1];
      int tofirst2=duqu2[sx2index-1];


      char s0[100];
      char s1[100];
      char s2[100];
      table1.append(".tab");
      table2.append(".tab");
      char *t1place=(char *)table1.c_str();
      char *t2place=(char *)table2.c_str();

      char linshi[100];//用于char*跟string的转换

      fstream t1,t2;
      
      if(datatype1==0){   //如果连接类型是整型
           int t1_int[200]={0};          //把table1里用于连接的属性都记录下来
           int t2_int[200]={0};


          t1.open(t1place,ios::in | ios::binary);
              while(!t1.eof()){
                  t1.seekg(tofirst1,ios::cur);
      t1.read(reinterpret_cast<char *>(&t1_int[table1num]),sizeof(int));
      t1.seekg(tolast1,ios::cur);
      table1num++;
      
              }
              t1.close();
              table1num--;



              t2.open(t2place,ios::in | ios::binary);
              while(!t2.eof()){
                  t2.seekg(tofirst2,ios::cur);
      t2.read(reinterpret_cast<char *>(&t2_int[table2num]),sizeof(int));
      t2.seekg(tolast2,ios::cur);
      table2num++;
              }
              t2.close();
              table2num--;

              
              fstream temp,t1,t2;
              temp.open("temp.tab",ios::out|ios::binary);
              t1.open(t1place,ios::in|ios::binary);
              //t2.open(t2place,ios::in|ios::binary);
         for(int i=0;i<table1num;i++){
             t1.read(s0,duqu1[t1sx_hao]*sizeof(char));    //把table1的元组逐次读出
                 s0[duqu1[t1sx_hao]]='\0';

                 t2.open(t2place,ios::in|ios::binary);
            for(int t=0;t<table2num;t++){

       
                t2.read(s1,tofirst2*sizeof(char));
                s1[tofirst2]='\0';
                t2.seekg(4L,ios::cur);
                t2.read(s2,tolast2*sizeof(char));
                     s2[tofirst2]='\0';
                 if(t1_int[i]==t2_int[t]){          //如果连接的属性值相等，把t1的元组写入temp
                     temp.write(s0,duqu1[t1sx_hao]*sizeof(char));
                    //t2.read(s,tofirst2*sizeof(char));
                    //s[tofirst2]='\0';
                     temp.write(s1,tofirst2*sizeof(char));
                     //t2.seekg(4L,ios::cur);
                     //t2.read(s,tolast2*sizeof(char));
                    // s[tofirst2]='\0';
                     temp.write(s2,tolast2*sizeof(char));
             }
             }
                  t2.close();
         }
              temp.close();
              t1.close();
      }


      else {
                                              //如果连接类型是char型
           string t1_strings[200]={""};                 //把table1里用于连接的属性都记录下来
           string t2_strings[200]={""};


          t1.open(t1place,ios::in | ios::binary);

              while(!t1.eof()){
           t1.seekg(tofirst1,ios::cur);
           t1.read(linshi,datatype1);
           linshi[datatype1]='\0';
           t1_strings[table1num]=linshi;
           t1.seekg(tolast1,ios::cur);
           table1num++;

              }
              t1.close();
              table1num--;



              t2.open(t2place,ios::in | ios::binary);
              while(!t2.eof()){
                  t2.seekg(tofirst2,ios::cur);
      t2.read(linshi,datatype1);
      linshi[datatype1]='\0';
      t2_strings[table2num]=linshi;
      t2.seekg(tolast2,ios::cur);
      table2num++;
              }
              t2.close();
              table2num--;





              fstream temp,t1,t2;
              temp.open("temp.tab",ios::out|ios::binary);
              t1.open(t1place,ios::in|ios::binary);
              //t2.open(t2place,ios::in|ios::binary);
         for(int i=0;i<table1num;i++){
             t1.read(s0,duqu1[t1sx_hao]*sizeof(char));    //把table1的元组逐次读出
                 s0[duqu1[t1sx_hao]]='\0';


                 t2.open(t2place,ios::in|ios::binary);
            for(int t=0;t<table2num;t++){


                t2.read(s1,tofirst2*sizeof(char));
                s1[tofirst2]='\0';
                t2.seekg(datatype1,ios::cur);
                t2.read(s2,tolast2*sizeof(char));
                s2[tofirst2]='\0';
                 if(t1_strings[i]==t2_strings[t]){          //如果连接的属性值相等，把t1的元组写入temp
                     temp.write(s0,duqu1[t1sx_hao]*sizeof(char));
                    //t2.read(s,tofirst2*sizeof(char));
                    // s[tofirst2]='\0';
                     temp.write(s1,tofirst2*sizeof(char));
                     //t2.seekg(4L,ios::cur);
                     //t2.read(s,tolast2*sizeof(char));
                    // s[tofirst2]='\0';
                     temp.write(s2,tolast2*sizeof(char));
             }
             }
                  t2.close();
         }
              temp.close();
              t1.close();


             
      }
      
  }

}
