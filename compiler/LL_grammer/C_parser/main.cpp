/*************************************************************
C 子集的词法分析器
*/

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

class SyntaxError;

class CParser
{
    enum ATTRIBUTES{NUMBER_SYMBOL,RELATION_SYMBOL,COMPUTE_SYMBOL,VARIABLE_SYMBOL,RESERVE_SYMBOL,COMMENT_SYMBOL,CHAR_SYMBOL};
    const string NUMBER_ALPHA_ERROR="number can't append an alpha in a variable name !!";
    const string MULTI_CHAR="multiple chars in single quotation.";
    const string BAD_CHAR="Bad Char ! This char can't be parsed.";
    // ; << >>
public:
    CParser();

    bool readFile(const string& file);

    // main analyzing process
    bool parsing(const char* file);

    //judge isReserve
    bool isReserve(const string& newToken);

    //helper in judging /**/ (comment)
    void commentHelper();

    //some states for atumation in judging number
    void digit_state2();
    void digit_state3();
    void digit_state5();
    void digit_state7();

    //print function
    void printSymbolTable();
    void printStatics();
    void printErrors();

    //adjust line
    void jumpCurrentLine();
    void getNewLine();

    //add a symbol to symbol_table
    void addSymbol(const string& value,ATTRIBUTES attr);

    //add error to allError
    void addError(const string& errorInfo);

    //add a char into Token
    bool putCharInToken();
private:
    ifstream readSource;    //open file
    string charBuffer;      //read buffer
    string token;
    map<string,string> symbol_table;
    char currentChar;
    vector<SyntaxError> allErrors;

    int bufferLen;
    int currentPosPtr;
    int lines;
    int allChars;

    int numberCounter,variableCounter,relationCounter,computeCounter,reserveCounter,commentCounter;

    static vector<string> RESERVE_TABLE;
};

vector<string>  CParser::RESERVE_TABLE={
        "and","and_eq","asm","auto",
        "bitand","bitor","bool","break",
        "case","catch","char","class",
        "compl","const","const_cast","continue",
        "default","delete","do","double",
        "dynamic_cast","else","enum","explicit",
        "export","extern","false","float",
        "for","friend","goto","if",
        "inline","int","long","mutable",
        "namespace","new","not","not_eq",
        "operator","or","or_eq","private",
        "protected","public","register","reinterpret_cast",
        "return","short","signed","struct",
        "sizeof","static","static_cast","throw",
        "switch","template","this","typeid",
        "true","try","typedef","using",
        "typename","union","unsigned","virtual",
        "void","volatile","wchar_t","while",
        "xor","xor_eq",
};

class SyntaxError{
public:
    SyntaxError(int line,int column,const string& error);
   // ~SyntaxError();

    int getPosition_column() const {
        return position_column;
    }

    void setPosition_column(int position_column) {
        SyntaxError::position_column = position_column;
    }

    const string &getInfo() const {
        return info;
    }

    void setInfo(const string &info) {
        SyntaxError::info = info;
    }

    int getPosition_line() const {
        return position_line;
    }

    void setPosition_line(int position_line) {
        SyntaxError::position_line = position_line;
    }

private:
    int position_line;
    int position_column;
    string info;
};

SyntaxError::SyntaxError(int line, int column, const string &error)
:position_line(line),position_column(column),info(error)
{
}

CParser::CParser()
        :lines(0),allChars(0),charBuffer("")
{

    numberCounter=0;
    variableCounter=0;
    relationCounter=0;
    computeCounter=0;
    reserveCounter=0;
    commentCounter=0;
}

bool CParser::readFile(const string& file)
{
    readSource.open(file,ios::in);
    if (!readSource.is_open())
    {
        cout<<"Bad read source file!!!\n";
        return false;
    }
    return true;
}


void CParser::jumpCurrentLine()
{
    currentPosPtr=charBuffer.length();
}


void CParser::getNewLine() {
    getline(readSource,charBuffer);
    bufferLen=charBuffer.length();
    currentPosPtr=0;
    currentChar=charBuffer.at(currentPosPtr);
}

//attr for add different string
void CParser::addSymbol(const string &value, ATTRIBUTES attr)
{
    switch (attr)
    {
        case NUMBER_SYMBOL:
            symbol_table.insert(pair<string,string>(value,"number"));\
            numberCounter++;
            break;
        case RELATION_SYMBOL:
            symbol_table.insert(pair<string,string>(value,"relationship"));
            relationCounter++;
            break;
        case VARIABLE_SYMBOL:
            symbol_table.insert(pair<string,string>(value,"variable"));
            variableCounter++;
            break;
        case RESERVE_SYMBOL:
            symbol_table.insert(pair<string,string>(value,"reserve"));
            reserveCounter++;
            break;
        case COMPUTE_SYMBOL:
            symbol_table.insert(pair<string,string>(value,"compute"));
            computeCounter++;
            break;
        case COMMENT_SYMBOL:
            symbol_table.insert(pair<string,string>(value,"comment"));
            commentCounter++;
            break;
        case CHAR_SYMBOL:
            symbol_table.insert(pair<string,string>(value,"char"));
    }
    token.clear();

}

// add a error to allError
void CParser::addError(const string &errorInfo)
{
    allErrors.push_back(SyntaxError(lines,currentPosPtr,errorInfo));
    jumpCurrentLine();
}

void CParser::digit_state2()
{
    putCharInToken();
    switch(currentChar)
    {
        case '0'...'9':
            if(currentPosPtr==charBuffer.length())
                break;
            digit_state2();
            break;
        case 'E':case 'e':
            digit_state5();
            break;
        case 'a'...'d':case 'f'...'z':case 'A'...'D':case 'F'...'Z':
            addError(NUMBER_ALPHA_ERROR);
            break;
        case '.':
            digit_state3();
            break;
        default:
            addSymbol(token,NUMBER_SYMBOL);
            break;
    }

}
void CParser::digit_state3()
{
    putCharInToken();
    if (currentChar>='0' && currentChar<='9') {
        putCharInToken();
        while(currentChar>='0' && currentChar<='9')
        {
            putCharInToken();
        }
        if(currentChar=='E' || currentChar=='e')
        {
            digit_state5();
        }
        else
        {
            addSymbol(token,NUMBER_SYMBOL);
            if(isalpha(currentChar))
            addError(NUMBER_ALPHA_ERROR);
         }
    }
    else
    {
        //error
        addError(NUMBER_ALPHA_ERROR);
    }
}

void CParser::digit_state5()
{
    putCharInToken();
    switch (currentChar) {
        case '+':case '-':
            putCharInToken();
            if (currentChar<='9' && currentChar>='0') {
                digit_state7();
            }
            else
            {
                //error
                addError(NUMBER_ALPHA_ERROR);
            }
            break;
        case '0'...'9':
            digit_state7();
            break;
        default:
            //error
            addError(NUMBER_ALPHA_ERROR);
            break;
    }
}

void CParser::digit_state7()
{
    putCharInToken();
    while(currentChar<='9' && currentChar>='0')
    {
        putCharInToken();
    }
    if (isalpha(currentChar))
    {
        addError(NUMBER_ALPHA_ERROR);
    }
    else
    {
        addSymbol(token,NUMBER_SYMBOL);
    }
}


void CParser::printSymbolTable()
{
    cout<<"print symbol table: \n";
    for(auto m:symbol_table)
    {
        cout<<m.first<<"  "<<m.second<<"  \n";
    }
}

void CParser::printStatics()
{
    cout<<"----------------------------------------------------------------"<<endl;
    cout<<"all lines: "+to_string(lines)<<"  all chars: "+to_string(allChars)<<endl;
    cout<<"each class token: \n";
    cout<<"number: "<<numberCounter<<"    variable: "<<variableCounter<<endl;
    cout<<"relation: "<<relationCounter<<"    compute: "<<computeCounter<<endl;
    cout<<"reserve: "<<reserveCounter<<"     comment: "<<commentCounter<<endl;
    cout<<"----------------------------------------------------------------"<<endl;
}

void CParser::printErrors()
{
    for(auto m:allErrors)
    {
        cout<<"Error:"<<"position: line"<<m.getPosition_line()<<" column"<<m.getPosition_column()<<"  "+m.getInfo()<<"\n";
    }
}
bool CParser::putCharInToken()
{

    token+=currentChar;
    allChars++;
    currentPosPtr++;
    if (currentPosPtr<charBuffer.length())
    {
        currentChar=charBuffer.at(currentPosPtr);
        return true;
    }
    else
    {
        return false;
    }

}

bool CParser::isReserve(const string& newToken)
{
    auto flag=find(RESERVE_TABLE.begin(),RESERVE_TABLE.end(),newToken);
    return flag!=RESERVE_TABLE.end();

}

void CParser::commentHelper()
{
    while(currentChar!='*')
    {

        if (!putCharInToken())
        {
            getNewLine();
        }
    }
    putCharInToken();
    if (currentChar=='/')
    {
        putCharInToken();
        addSymbol(token,COMMENT_SYMBOL);
    }
    else
    {
        commentHelper();
    }
}

bool CParser::parsing(const char* file)
{

    if(!this->readFile(file))
    {
        cout<<"Sorry for bad reading file.\n";
        return false;
    }
    else
    {
        while (!readSource.eof())
        {
            while(getline(readSource,charBuffer))
            {
                if (charBuffer[charBuffer.length()-1]=='\r')
                {
                    charBuffer.erase(charBuffer.end()-1);
                }
                lines++;
                if (charBuffer!="")
                {
                    break;
                }
             }

            currentPosPtr=0;
            bufferLen=charBuffer.length();
            while (currentPosPtr<bufferLen)
            {
                currentChar=charBuffer.at(currentPosPtr);

                switch (currentChar)
                {
                    case '+':
                        putCharInToken();
                        if (currentChar=='='||currentChar=='+')
                        {
                            putCharInToken();
                            addSymbol(token,COMPUTE_SYMBOL);
                        }
                        break;

                    case '\'':
                        putCharInToken();
                        if (isalpha(currentChar))
                        {
                            putCharInToken();
                            if (currentChar!='\'')
                            {
                                addError(MULTI_CHAR);
                                token.clear();
                                jumpCurrentLine();
                            }
                            else
                            {
                                putCharInToken();
                                addSymbol(token,CHAR_SYMBOL);
                            }
                        }
                        else if(currentChar=='\\')
                        {
                            putCharInToken();
                            putCharInToken();
                            if(currentChar=='\'')
                            {
                                putCharInToken();
                            }
                            else
                            {
                                addError(MULTI_CHAR);
                                token.clear();
                            }
                        }
                    case '/':
                        putCharInToken();
                        if (currentChar=='/')
                        {
                            token=charBuffer;
                            charBuffer.erase(charBuffer.begin(),charBuffer.begin()+currentPosPtr-1);
                            currentPosPtr=bufferLen;
                            addSymbol(token,COMMENT_SYMBOL);
                            //jumpCurrentLine();
                        }
                        else if(currentChar=='*')
                        {
                            putCharInToken();
                            commentHelper();
                        }
                        break;
                    case '-':
                        putCharInToken();
                        if (currentChar=='='||currentChar=='-')
                        {
                            putCharInToken();
                            addSymbol(token,COMPUTE_SYMBOL);
                        }
                        break;
                    case '=':
                        putCharInToken();
                        if (currentChar=='=')
                        {
                            putCharInToken();
                            addSymbol(token,RELATION_SYMBOL);
                        }
                        else
                        {
                            addSymbol(token,COMPUTE_SYMBOL);
                        }
                        break;
                    case '!':
                        putCharInToken();
                        if (currentChar=='=')
                        {
                            putCharInToken();
                            addSymbol(token,RELATION_SYMBOL);
                        }
                        else{
                            addSymbol(token,RELATION_SYMBOL);
                        }
                        break;
                    case '<':
                        putCharInToken();
                        if (currentChar=='<')
                        {
                            putCharInToken();
                            addSymbol(token,COMPUTE_SYMBOL);
                        }
                        else{
                            addSymbol(token,RELATION_SYMBOL);
                        }
                        break;
                    case '>':
                        putCharInToken();
                        if (currentChar=='>')
                        {
                            putCharInToken();
                            addSymbol(token,COMPUTE_SYMBOL);
                        }
                        else{
                            addSymbol(token,RELATION_SYMBOL);
                        }
                        break;
                    case ';': case '(': case ')':case '|': case '&': case '^':case '{':case '}':case '"':case '.':
                    case '[':case ']':case ':':case '#':case '*':case ',':case '%':case '\\':
                        // and token!= "<"
                        token+=currentChar;
                        addSymbol(token,RELATION_SYMBOL);
                        currentPosPtr++;
                        break;
                    case '0'...'9':
                        putCharInToken();
                        switch(currentChar)
                        {
                            case '0'...'9':
                                if(currentPosPtr==charBuffer.length())
                                    break;
                                digit_state2();
                                break;
                            case 'E':case 'e':
                                digit_state5();
                                break;
                            case 'a'...'d':case 'f'...'z':case 'A'...'D':case 'F'...'Z':
                                addError(NUMBER_ALPHA_ERROR);
                                break;
                            case '.':
                                digit_state3();
                                break;
                            default:
                                addSymbol(token,NUMBER_SYMBOL);
                                break;
                        }
                        break;

                    case 'a'...'z': case 'A'...'Z': case '_':
                        putCharInToken();
                        while(((currentChar>='a' && currentChar<='z')||(currentChar>='A' && currentChar<= 'Z')||
                               (currentChar>='0' && currentChar<='9')||currentChar=='_') && currentPosPtr<bufferLen)
                        {
                            putCharInToken();
                        }
                        if (isReserve(token))
                        {
                            addSymbol(token,RESERVE_SYMBOL);
                        }
                        else
                        {
                            addSymbol(token,VARIABLE_SYMBOL);
                        }

                        break;

                    case ' ':case '\t':
                        currentPosPtr++;
                        break;
                    default:
                        addError(BAD_CHAR);
                        break;

                }
            }
        }
    }
    readSource.close();
    return true;
}


int main()
{
    CParser* test=new CParser();
    string fileName;
    cout<<"Please input a name for parsing: \n";
    bool isParsed=false;
    while(!isParsed)
    {
        cin>>fileName;
        if (test->parsing(fileName.c_str()))
        {
            isParsed=true;
            test->printSymbolTable();
            test->printStatics();
            test->printErrors();
        }
        else
        {
            cout<<"Please input correct file name: \n";
        }
    }
    return 0;
}
