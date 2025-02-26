//This is a parser program



﻿#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <iomanip>
using namespace std;
#define MAX_CHAR 100


enum lex_type
{
    LEX_NULL,       // 0
    // КЛЮЧЕВЫЕ СЛОВА
    LEX_FOR,        // 1
    LEX_DO,         // 2
    LEX_ELSE,       // 3
    LEX_PROGRAM,    // 4
    LEX_VAR,        // 5
    LEX_BEGIN,      // 6
    LEX_END,        // 7
    LEX_INT,        // 8
    LEX_FLOAT,      // 9
    LEX_BOOL,       // 10
    LEX_IF,         // 11
    LEX_THEN,       // 12
    LEX_TO,         // 13
    LEX_WHILE,      // 14
    LEX_READ,       // 15
    LEX_WRITE,      // 16
    LEX_TRUE,       // 17
    LEX_FALSE,      // 18
    // МАРКЕР КОНЦА ТЕКСТА ПРОГРАММЫ
    LEX_FIN,        // 19
    // РАЗДЕЛИТЕЛИ      
    LEX_COMMA,      // 20
    LEX_OPN_BRK,    // 21
    LEX_CLS_BRK,    // 22
    LEX_COLON,      // 23
    LEX_OPN,        // 24
    LEX_CLS,        // 25
    LEX_UNAR,       // 26
    LEX_OPN_FGR_BRK,// 27
    LEX_CLS_FGR_BRK,// 28
    LEX_SEMICOLON,
    LEX_SLASH,
    LEX_STAR,
    // ОПЕРАТОРЫ
    LEX_PLUS,       // 29
    LEX_MIN,        // 30
    LEX_OR,         // 31
    LEX_MULT,       // 32
    LEX_DIV,        // 33
    LEX_AND,        // 34
    // ЧИСЛОВАЯ КОНСТАНТА
    LEX_NUM,        // 35
    // ИДЕНТИФИКАТОР
    LEX_ID,         // 36
    //СРАВНЕНИЕ
    LEX_AS,         //37
    LEX_EQ,         //38
    LEX_NE,         //39
    LEX_LT,         //40
    LEX_LE,         //41
    LEX_GT,         //42
    LEX_GE          //43
};

class Lex{ //класс - лесема
public:
    lex_type type; //тип лексемы
    int value; //значение лексемы

    Lex(lex_type t = LEX_NULL, int v = 0){
        type = t;
        value = v;
    }
    lex_type getType() { return type; } //геттер типа
    int getValue() { return value; } //геттер значения
    
    friend ostream& operator << (ostream& s, Lex l){ //вывод лексемы в виде (тип, номер в таблице)
        s << "(" << l.type << ',' << l.value << ");";
        return s;
    }
};

class TableId{ //таблица идентификаторов
    string identify[MAX_CHAR]; //таблица
    int top; //номер верхнего элемента

public:
    TableId(){
        top = 0;
    }
    int put_ident(string name) { //добавление элемента в таблицу
        identify[top] = name;
        top++;
        return top - 1;
    }
};
class TableNum{ //таблица чисел
    string numbers[MAX_CHAR];
    int top;

public:
    TableNum() {
        top = 0;
    }
    int put_number(string name) { //добавление элемента в таблицу
        numbers[top] = name;
        top++;
        return top - 1;
    }
};
TableId ident; //создание таблицы идентификаторов
TableNum numb; //создание таблицы чисел

class Lexer
{
    enum state { //возможные состояния
        H, //пробелы, переводы строк
        ID, //идентификатор
        NM, //число
        RTO, //сравнение
        DLM, //разделитель
        OPR, //операция
        ERR //ошибка
    };
    state CS; //начальное состояние
    static lex_type kwords[]; //типы ключевых слов
    static lex_type rat[]; //типы операций сравнения
    static lex_type dlms[]; //типы разделителей
    static lex_type oper[]; //типы арифметических операций
    char c; //текущий символ
    string keywords[MAX_CHAR]{ "for","do","else","program","var","begin","end","int","float","bool","if","then","to","while","read","write","true","false"}; //таблица ключевых слов
    string ratio[MAX_CHAR]{ "as","EQ","NE","LT","LE","GT","GE"}; //таблица операций отношения
    string operations[MAX_CHAR]{ "plus","min","or","mult","div","and" }; //таблица арифметических операций
    string delims[MAX_CHAR]{ ",","[","]",":","(",")","~","{","}",";","/","*"}; //таблица разделителей
    string* arrays[4] = { keywords,delims,operations,ratio };

public:
    FILE* fp; //файл на разбор

    int look(string sample,string table[]){ //найти лексему в соответствующей таблице
        for (int i = 0; i < MAX_CHAR; i++) {
            if (sample == table[i]) return i; //возвращает номер в таблице
        }
        return -1;
    }

    bool is_ident(string& id) { //проверка на идентификатор
        regex pattern(R"([a-zA-Z](\d|[a-zA-Z]|_)*)");
        return regex_match(id, pattern);
    }

    bool is_number(string& num) { //проверка на число
        regex pattern(R"(([\d]+[D|d])|([1-7]+[O|o])|([1|0]+[B|b])|((\d(\d|[a-f|A-F])*)[H|h])|([+|-]?\d+((\.+\d+)?|\.\d+)([eE][+-]?\d+)?)|(\d+))");
        return regex_match(num, pattern);
    }

    void gc(){ //считывание очередного символа
        c = (char)fgetc(fp);
    }

    Lexer(const char* filename) { //конструктор лексера
        fopen_s(&fp,filename, "r");
        CS = H; 
        gc();
    }

    Lex getLex();
};

lex_type Lexer::kwords[] = {
    LEX_FOR, //0
    LEX_DO, //1
    LEX_ELSE, //2
    LEX_PROGRAM, //3
    LEX_VAR, //4
    LEX_BEGIN, //5
    LEX_END, //6
    LEX_INT, //7
    LEX_FLOAT, //8
    LEX_BOOL, //9
    LEX_IF, //10
    LEX_THEN, //11
    LEX_TO, //12
    LEX_WHILE, //13
    LEX_READ, //14
    LEX_WRITE //15
};

lex_type Lexer::dlms[] = {
    LEX_COMMA,
    LEX_OPN_BRK,
    LEX_CLS_BRK,
    LEX_COLON,
    LEX_OPN,
    LEX_CLS,
    LEX_UNAR,
    LEX_OPN_FGR_BRK,
    LEX_CLS_FGR_BRK,
    LEX_SEMICOLON,
    LEX_SLASH,
    LEX_STAR
};

lex_type Lexer::rat[] = {
    LEX_AS,
    LEX_EQ,
    LEX_NE,
    LEX_LT,
    LEX_LE,
    LEX_GT,
    LEX_GE
};

lex_type Lexer::oper[] = {
    LEX_PLUS,
    LEX_MIN,
    LEX_OR,
    LEX_MULT,
    LEX_DIV,
    LEX_AND
};

Lex Lexer::getLex() {
    int j;
    CS = H;
    string result = ""; //результирующая лексема
    do {
        switch (CS) { 
        case H: {
            while (isspace(c)) { //нет значащих символов
               gc();
            }
            if (isalpha(c)) { //буква
                CS = ID;
            }
            else if (c == '+' || c == '-' || isdigit(c)) { //знак или цифра
                CS = NM;
            }
            else if (c == '@') return Lex(LEX_FIN); //знак окончания
            else {
                CS = DLM;
            }
            break;
        }
        case DLM: { //разделители
            while (!isspace(c) && c != '\n') { 
                result += c;
                gc();
            }
            cout << result << " ";
            j = look(result, delims); //поиск лексемы в таблице разделителей
            if (j!=-1) { 
                return Lex(dlms[j], j);
            }
            else {
                CS = ERR;
            }
            break;
        }
        case ERR: { //ошибка
            throw result; //исключение
        }
        case ID: { //идентификатор
            while (!isspace(c)) { 
                result += c;
                gc();
            }
            cout << result << " ";
            j = look(result, keywords);  //поиск лексемы в таблице ключевых слов
            if (j!=-1) return Lex(kwords[j], j);
            j = look(result, ratio); //поиск лексемы в таблице операций отношения
            if (j!=-1) return Lex(rat[j], j);
            j = look(result, operations); //поиск лексемы в таблице арифметических операций
            if (j!=-1) return Lex(oper[j], j);
            else if (is_ident(result)) { //проверка на идентификатор
                j = ident.put_ident(result); //занесение идентификатора в таблицу
                return Lex(LEX_ID, j); 
            }
            else {
                CS = ERR;
                break;
            }
        }
        case NM: { //число
            while (!isspace(c)) {
                result += c;
                gc();
            }
            cout << result << " ";
            if (is_number(result)) { //проверка на число
                j = numb.put_number(result);//занесение числа в таблицу
                return Lex(LEX_NUM, j);
            }
            else {
                CS = ERR;
                break;
            }
        }
        }
    } while (true);
}


class Parser
{
    Lex curr_lex;       // текущая лексема
    lex_type c_type;    // её тип
    int c_val;          // её значение
    Lexer lexer;
    bool in_comment;

public:
    //void S(); //start
    //void D(); //despcription
    //void B(); //begin
    //void I(); //ident
    //void E(); //end
    //void O(); //operator
    //void CO(); //compound operator
    //void A(); //assignment
    //void C(); //conditional
    //void FC(); //fixed cycle
    //void CC(); //conditional cycle
    //void INP_OUT(); //input or ouput
    //void OUT(); //output
    //void EXP(); //expression
    //void OP(); //operand
    //void ADD(); //addendum
    //void MLT(); //multiplier
    //void COM(); //comment

    void gl()
    {
        curr_lex = lexer.getLex();
        c_type = curr_lex.getType();
        c_val = curr_lex.getValue();
        if (c_type == LEX_SLASH&&!in_comment) {
            COM();
            gl();
        }
    }

    void analyze()
    {
        gl();
        S();
    }

    Parser(const char* filename) : lexer(filename) {}

    void S() { //старт
        if (c_type != LEX_PROGRAM)  throw curr_lex;
        else gl();
        if (c_type != LEX_VAR) throw curr_lex;
        else gl();
        D();
        if (c_type != LEX_BEGIN) throw curr_lex;
        else gl();
        O();
        while (c_type == LEX_SEMICOLON) {
            gl();
            O();
        }
        if (c_type != LEX_END) throw curr_lex;
        return;
    }

    void D() { //описание
        if (c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_FLOAT) gl();
        else throw curr_lex;
        if (c_type != LEX_ID) throw curr_lex;
        else gl();
        while (c_type == LEX_COMMA) { //запятая
            gl();
            if (c_type != LEX_ID) throw curr_lex;
            gl();
        }
    }

    void O() { //оператор
        if (c_type == LEX_OPN_BRK) {
            gl();
            CO(); //составной
        }
        else if (c_type == LEX_ID) {
            gl();
            A(); //присваивания
        }
        else if (c_type == LEX_IF) {
            gl();
            C(); //условный
        }
        else if (c_type == LEX_FOR) {
            gl();
            FC(); //фикс цикла
        }
        else if (c_type == LEX_WHILE) {
            gl();
            CC(); //условного цикла
        }
        else if (c_type == LEX_READ || c_type == LEX_WRITE) {
            gl();
            INP_OUT(); //ввода или вывода
        }
        else throw curr_lex;
    }

    void CO() {
        O();
       // gl();
        while (c_type == LEX_COLON) {
            gl();
            O();
        }
        if (c_type != LEX_CLS_BRK) throw curr_lex;
        gl();
    }

    void A() {
        if (c_type != LEX_AS) throw curr_lex;
        gl();
        EXP();
    }

    void C() {
        EXP();
        if (c_type != LEX_THEN) throw curr_lex;
        gl();
        O();
        if (c_type == LEX_ELSE) {
            gl();
            O();
        }
    }

    void FC() {
        O();
        if (c_type != LEX_TO) throw curr_lex;
        gl();
        EXP();
        if (c_type != LEX_DO) throw curr_lex;
        gl();
        O();
    }

    void CC() {
        EXP();
        if (c_type != LEX_DO) throw curr_lex;
        gl();
        O();
    }

    void INP_OUT() {
        if (c_type != LEX_OPN) throw curr_lex;
        gl();
        if (c_type != LEX_ID) throw curr_lex;
        gl();
        while (c_type == LEX_COMMA) {
            if (c_type != LEX_ID) throw curr_lex;
            gl();
        }
        if (c_type != LEX_CLS) throw curr_lex;
        gl();
    }

    void EXP() { //выражение
        OP(); //операнд
        while (c_type == LEX_EQ || c_type == LEX_LT || c_type == LEX_LE) {
            gl();
            OP();
        }
   
    }

    void OP() {
        ADD(); //слагаемое
        while (c_type == LEX_PLUS || c_type == LEX_MIN || c_type == LEX_OR) {
            gl();
            ADD();
        }
    }

    void ADD() {
        MLT(); //множитель
        while (c_type == LEX_MULT || c_type == LEX_AND || c_type == LEX_DIV) {
            gl();
            MLT();
        }
    }

    void MLT() {
        if (c_type == LEX_OPN) {
            gl();
            EXP();
            if (c_type != LEX_CLS) throw curr_lex;
            gl();
        }
        else if (c_type != LEX_ID && c_type != LEX_NUM && c_type != LEX_TRUE && c_type != LEX_FALSE && c_type != LEX_UNAR) {
            throw curr_lex;
        }
        else gl();

    }
    void COM() {
        in_comment = true;
        gl();
        if (c_type != LEX_STAR) throw curr_lex;
        do {
            gl();
        } while (c_type != LEX_STAR);
        gl();
        if (c_type != LEX_SLASH) throw curr_lex;
        in_comment = false;
    }
};




int main() {
    const char* name = "example.txt"; //анализируемый файл
    Lexer lexer(name);
    while (!feof(lexer.fp)) {
        try { 
            Lex lex = lexer.getLex(); //запуск анализатора
            cout << lex << endl;
            if (lex.type == LEX_END) { //символ окончания программы
                break;
            }
        }
        catch (string result) { //перехват ошибки
            cerr << " <= LEXER ERROR " << endl;
            system("pause");
            return 0;
        }
    }

    Parser parser(name);
    try {
        parser.analyze();
        cout << "\nOk" << endl;
    }
    catch (Lex lex) {
        cout << "\nSYNTAX ERROR => " << lex << endl;
    }
    catch (string s) {
        cout << s << endl;
    }

}



