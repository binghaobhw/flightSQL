
#ifndef MYH_H
#define	MYH_H
#include <string>
#include <fstream>

using namespace std;


#define piece_length 10

typedef class mystream {
public:
    int byte_point;
    int file_length;
    string file_path;

    mystream();
    void open(string filePath);
    void read(char* piece);
    int get_byte_point();
    bool end();



} myfstream;

myfstream::mystream() {
    this->byte_point = 0;
    this->file_length = 0;

}

void myfstream::open(string file_path) {
    /*文件路径，指针初始化*/
    this->file_path = file_path;
    this->byte_point = 0;
    /*打开文件，获得文件字节长度*/
    fstream get_len;
    get_len.open(file_path.c_str(), ios::in | ios::binary);
    get_len.seekg(0, ios::end);
    this->file_length = get_len.tellg();
    get_len.close();
}

void myfstream::read(char* piece) {
    /*打开文件，读指针移动到读取位置，读取piece数据，记录下次读取的指针位置*/
    fstream readfile;
    readfile.open(this->file_path.c_str(), ios::in | ios::binary);
    readfile.seekg(this->byte_point);
    readfile.read(piece, piece_length);
    this->byte_point += piece_length;
    readfile.close();
    int over_length = this->byte_point - this->file_length;
    if (over_length > 0)
        for (int i = piece_length - over_length; i < piece_length; i++)
            *(piece + i) = '\0';
}

int myfstream::get_byte_point() {
    return this->byte_point;
}

bool myfstream::end() {
    if (this->byte_point >= this->file_length)
        return true;
    else
        return false;
}

#endif	/* MYH_H */

