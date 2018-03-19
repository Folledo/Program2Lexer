#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <map>
#include <vector>
#include <cctype>
#include <iterator>
#include <utility>
//#include <projlex.h>
using namespace std;

string keywords[] = {"char","string","int","double","float","include","start", "finish", "then", "if", "repeat", "var", "while", "string", "switch", "case", "int", "float", "do", "read", "print", "void", "return", "cout", "cin","using","namespace", "program"};

enum TType{
    UNKNOWN,
    
    //keywords
    SET, PRINT, VAR, REPEAT,
    //an identifier
    IDENT,
    //an integer and string constant
    ICONST, SCONST,
    //the operators
    PLUS, MINUS, STAR, COLON, LSQ, RSQ, LPAREN, RPAREN, SC,
    //any error returns this token
    ERR,
    //when comploeted (EOF), return this token
    DONE
};

class Token {
    TType tt;
    string lexeme;
    int lnum;
public:
    TType SetTokenType() const { return tt; }
    string SetLexeme() const { return lexeme; }
    int SetLinenum() const { return lnum; }
    
    Token() {//default constructor doesnt accept parameters
        tt = ERR;
        lnum = -1;
    }
    Token(TType tt, string lexeme, int line) { //contructor that has parameters
        this->tt = tt;
        this->lexeme = lexeme;
        this->lnum = line;
    }
    bool operator==(const TType tt) const
        { return this->tt == tt; }
    bool operator!=(const TType tt) const
        { return this->tt != tt; }
    TType GetTokenType() const { return tt; }
    string GetLexeme() const { return lexeme; }
    int GetLinenum() const { return lnum; }
};

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

bool isV=false, isMCI=false, isSUM=false;

int main( int argc, char *argv[] )
{
    
    multimap<int, string> tokenMap;
    
    for( int n=1; n<argc; n++ )
    {
        if( argv[n][0] != '-' ) continue;
        string arg( argv[n] );
        if(     arg == "-v" )   {cout<<"Tokens are printed normally\n";isV=true;}
        else if(arg == "-mci")  {cout<<"The identifier that appears the most often is...\n";isMCI=true;}
        else if(arg == "-sum" ) {cout<<"The summary information:\n";isSUM=true;}
        else { cout << arg << " INVALID FLAG" << endl; return 1; }
    }
    
    bool foundfile=false;
    ifstream file;
    for( int n=1; n<argc; n++ )
    {
        if( argv[n][0] == '-' ) continue;
        if( foundfile ) { cout << "TOO MANY FILE NAMES" << endl; return 1; }
        foundfile=true;
        file.open(argv[n]);
        if( ! file.is_open() ) { cout << argv[n] << " UNABLE TO OPEN" << endl; return 1; }
    }
    
    
    int nCharsIn = 0, nLinesIn = 0, nToken = 0;
    string line;
    while(getline(file,line))
    {
        nCharsIn += line.size();
        nLinesIn++;
        //I need to give linestream an actual token by itself################
        stringstream lineStream(line);
        int tokenLine = nLinesIn;
        char ch;
        //while(lineStream.get(ch)){nCharsIn++;}//get the number of chars
        while(lineStream.get(ch))
        {
            string currentToken = "";
            
//keyword or identifiers
            if (isalpha(ch)){
                char c = lineStream.peek();
                currentToken+=ch;
                while (isalnum(c)) {
                    lineStream.get(c);
                    currentToken+=c;
                    c = lineStream.peek();
                }//now we have a token
                
                bool isKeyword = false;
                for (int i = 0; i < 28; i++){//less than keywords array
                    string currentString = keywords[i];
                    if (currentToken == currentString){
                        isKeyword = true;
                        currentToken.insert(0,"Keyword:               ");
                    }
                }
                
                if (isKeyword == false){ currentToken.insert(0,"Identifier:            ");}
                
                nToken++;
                tokenMap.insert(make_pair(tokenLine,currentToken));
                currentToken = "";
            }

//operators, relations, and delimiters
            if (ispunct(ch)){
                currentToken+=ch;
                char c = lineStream.peek();
                char dh = '\0';
                string toInsert = "";

                if (ch == '+'){
                    toInsert = "PLUS Operator:         ";
                    if (c == '+'){ lineStream.get(c); currentToken+=c; toInsert = "PlusPlus Operator:     ";}
                } else if (ch == '-'){
                    toInsert = "MINUS Operator:        ";
                    if (c == '-'){ lineStream.get(c); currentToken+=c; toInsert = "MinusMinus Operator:   ";}
                } else if (ch == '*'){ toInsert = "MULTIPLY Operator:     ";
                } else if (ch == '/'){
                    toInsert = "DIVIDE Operator:       ";
                    if (c == '/'){ toInsert = "COMMENT:               ";}
                } else if (ch == '='){
                    toInsert = "EQUAL Operator:        ";
                    if (c == '='){lineStream.get(c); currentToken+=c; toInsert = "EqualEqual Operator:   ";}
                } else if (ch == '"') {
                    toInsert = "String:                ";
                    while (dh != '"') { lineStream.get(dh); currentToken+=dh; }
                } else if (ch == '<') {
                    toInsert = "LessThan Operator:     ";
                    if (c == '<'){
                        toInsert = "Relational Operator:   ";
                        currentToken+=c;
                        lineStream.get(c);//moves the pointer to ignore c
                    }
                } else if (ch == '#'){ toInsert = "COMMENT:               ";
                } else if (ch == ';'){ toInsert = "Semicolon Delimiter:   ";
                } else if (ch == ','){ toInsert = "Comma Delimiter:       ";
                } else if (ch == ':'){ toInsert = "Colon Delimiter:       ";
                } else if (ch == '['){ toInsert = "LSQ:                   ";
                } else if (ch == ']'){ toInsert = "RSQ:                   ";
                } else if (ch == '('){ toInsert = "LPAREN:                ";
                } else if (ch == ')'){ toInsert = "RPAREN:                ";
                } else if (ch == '{'){ toInsert = "LCurly Delimiter:      ";
                } else if (ch == '}'){ toInsert = "RCurly Delimiter:      ";
                } else { toInsert = "Symbol:                ";
                }
                
                currentToken.insert(0,toInsert);
                nToken++;
                tokenMap.insert(make_pair(tokenLine,currentToken));
            }//end of ispunct
            
            
            
//digit tokens
            if (isdigit(ch)){
                currentToken+="Digit:                 ";
                char c = lineStream.peek();
                currentToken+=ch;
                while (isdigit(c)) {
                    lineStream.get(c);
                    currentToken+=c;
                    c = lineStream.peek();
                }
                nToken++;
                tokenMap.insert(make_pair(tokenLine,currentToken));
                currentToken = "";
            }
            
            
        }//end of while getChar
    }//end of while getline
    
    cout<<"========RESULTS========\n";
    cout<<"There are "<<nCharsIn<<" characters"<<endl;
    cout<<"There are "<<nLinesIn<<" lines"<<endl;
    cout<<"There are "<<nToken<<" tokens"<<endl;
    cout<<"\n========TOKEN RESULTS========\n";
    for (auto it = tokenMap.begin(); it != tokenMap.end(); it++)
    {
        cout<<it->second<<" in line# "<<it->first<<endl;
    }
    
    return 0;
}
