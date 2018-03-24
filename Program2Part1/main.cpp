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

string keywords[] =  {"char","string","int","double","float","include","start", "finish", "then", "if", "repeat", "var", "while", "string", "switch", "case", "int", "float", "do", "read", "print", "void", "return", "cout", "cin","using","namespace", "program"};

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

//Best answer for Lexeme Programming
/*
1. projlex.h
2. main.cpp
3. lex.cpp
 

 * projlex.h
 *
 * CS280
 * Spring 2018


#ifndef PROJLEX_H_
#define PROJLEX_H_

#include <string>
#include <iostream>
using std::string;
using std::istream;
using std::ostream;

enum TType {
    // keywords
    SET,
    PRINT,
    VAR,
    REPEAT,
    
    // an identifier
    IDENT,
    
    // an integer and string constant
    ICONST,
    SCONST,
    
    // the operators, parens and semicolon
    PLUS,
    MINUS,
    STAR,
    COLON,
    LSQ,
    RSQ,
    LPAREN,
    RPAREN,
    SC,
    
    // any error returns this token
    ERR,
    
    // when completed (EOF), return this token
    DONE
};

class Token {
    TType    tt;
    string        lexeme;
    int            lnum;
    
public:
    Token() {
        tt = ERR;
        lnum = -1;
    }
    Token(TType tt, string lexeme, int line) {
        this->tt = tt;
        this->lexeme = lexeme;
        this->lnum = line;
    }
    
    bool operator==(const TType tt) const { return this->tt == tt; }
    bool operator!=(const TType tt) const { return this->tt != tt; }
    
    TType        GetTokenType() const { return tt; }
    string        GetLexeme() const { return lexeme; }
    int            GetLinenum() const { return lnum; }
};

extern ostream& operator<<(ostream& out, const Token& tok);

extern Token getNextToken(istream *in, int *linenum);


#endif /* PROJLEX_H_

//-------------------------------------------------------------------------------------
main

 * main.cpp
 *
 * CS280 - Fall 2017
 * Lexical Analyzer test engine
 

#include <fstream>
using std::ifstream;

#include <map>
using std::map;

#include "projlex.h"
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

int
main(int argc, char *argv[])
{
    int lineNumber = 0;
    
    Token    tok;
    
    istream *in = &cin;
    ifstream file;
    
    bool vflag = false;
    bool mciflag = false;
    bool sumflag = false;
    
    int tokCount = 0;
    int stringCount = 0;
    map<TType,int> counts;
    map<string,bool> allStrings;
    map<string,int> identCount;
    
    for( int i=1; i<argc; i++ ) {
        string arg( argv[i] );
        if( arg == "-v" )
            vflag = true;
        else if( arg == "-mci" )
            mciflag = true;
        else if( arg == "-sum" )
            sumflag = true;
        else if( arg[0] == '-' ) {
            cerr << "INVALID FLAG " << arg << endl;
            return 0;
        }
        else if( in != &cin ) {
            cerr << "TOO MANY FILE NAMES" << endl;
            return 0;
        }
        else {
            file.open(arg);
            if( file.is_open() == false ) {
                cerr << "UNABLE TO OPEN " << arg << endl;
                return 0;
            }
            
            in = &file;
        }
    }
    
    while( (tok = getNextToken(in, &lineNumber)) != DONE && tok != ERR ) {
        if( vflag )
            cout << tok << endl;
        
        ++tokCount;
        if( tok == SCONST ) {
            stringCount++;
            allStrings[ tok.GetLexeme() ] = true;
        }
        else if( tok == IDENT )
            identCount[ tok.GetLexeme() ]++;
    }
    
    if( tok == ERR ) {
        cout << "Error on line " << lineNumber << " (" << tok.GetLexeme() << ")" << endl;
        return 0;
    }
    
    if( mciflag && identCount.size() > 0 ) {
        cout << "Most Common Identifier: ";
        // what is the most common identifier?
        // first, find the largest counter in the map
        int maxcnt = 0;
        for( auto it = identCount.begin(); it != identCount.end(); it++ )
            if( it->second > maxcnt )
                maxcnt = it->second;
        
        // now, print all the identifiers with that length
        bool printed = false;
        for( auto it = identCount.begin(); it != identCount.end(); it++ ) {
            if( it->second != maxcnt )
                continue;
            if( printed )
                cout << ", ";
            printed = true;
            cout << it->first;
        }
        cout << endl;
    }
    
    if( sumflag ) {
        cout << "Total lines: " << lineNumber << endl;
        cout << "Total tokens: " << tokCount << endl;
        if( tokCount > 0 ) {
            cout << "Total strings: " << stringCount << endl;
            if( stringCount > 0 ) {
                unsigned maxSlen = 0;
                for( auto it = allStrings.begin(); it != allStrings.end(); it++ ) {
                    if( it->first.length() > maxSlen )
                        maxSlen = it->first.length();
                }
                cout << "Length of longest string: " << maxSlen << endl;
            }
        }
    }
    
    return 0;
}


//-------------------------------------------------------------------------------------
/*
 * lex.cpp
 *
 * CS280 - Fall 2017
 

#include <cctype>
#include <map>
using std::map;

#include "projlex.h"

static map<TType,string> tokenPrint = {
    { SET, "SET" },
    { PRINT, "PRINT" },
    { VAR, "VAR" },
    { REPEAT, "REPEAT" },
    
    { IDENT, "IDENT" },
    
    { ICONST, "ICONST" },
    { SCONST, "SCONST" },
    
    { PLUS, "PLUS" },
    { MINUS, "MINUS" },
    { STAR, "STAR" },
    { COLON, "COLON" },
    { LSQ, "LSQ" },
    { RSQ, "RSQ" },
    { LPAREN, "LPAREN" },
    { RPAREN, "RPAREN" },
    { SC, "SC" },
    
    { ERR, "ERR" },
    
    { DONE, "DONE" }
};


ostream& operator<<(ostream& out, const Token& tok) {
    TType tt = tok.GetTokenType();
    out << tokenPrint[ tt ];
    if( tt == IDENT || tt == ICONST || tt == SCONST || tt == ERR ) {
        out << "(" << tok.GetLexeme() << ")";
    }
    return out;
}


static map<string,TType> kwmap = {
    { "var", VAR },
    { "set", SET },
    { "print", PRINT },
    { "repeat", REPEAT },
};

Token
id_or_kw(const string& lexeme, int linenum)
{
    TType tt = IDENT;
    
    auto kIt = kwmap.find(lexeme);
    if( kIt != kwmap.end() )
        tt = kIt->second;
    
    return Token(tt, lexeme, linenum);
}


Token
getNextToken(istream *in, int *linenum)
{
    enum LexState { BEGIN, INID, INSTRING, SAWMINUS, ININT, INCOMMENT } lexstate = BEGIN;
    string lexeme;
    char ch;
    
    while(in->get(ch)) {
        
        if( ch == '\n' ) {
            (*linenum)++;
        }
        
        switch( lexstate ) {
            case BEGIN:
                if( isspace(ch) )
                    continue;
                
                lexeme = ch;
                
                if( isalpha(ch) ) {
                    lexstate = INID;
                }
                else if( ch == '"' ) {
                    lexstate = INSTRING;
                }
                else if( ch == '-' ) {
                    lexstate = SAWMINUS;
                }
                else if( isdigit(ch) ) {
                    lexstate = ININT;
                }
                else if( ch == '#' ) {
                    lexstate = INCOMMENT;
                }
                else {
                    TType tt = ERR;
                    switch( ch ) {
                        case '+':
                            tt = PLUS;
                            break;
                        case '*':
                            tt = STAR;
                            break;
                        case ':':
                            tt = COLON;
                            break;
                        case '[':
                            tt = LSQ;
                            break;
                        case ']':
                            tt = RSQ;
                            break;
                        case '(':
                            tt = LPAREN;
                            break;
                        case ')':
                            tt = RPAREN;
                            break;
                        case ';':
                            tt = SC;
                            break;
                    }
                    
                    return Token(tt, lexeme, *linenum);
                }
                break;
                
            case INID:
                if( isalpha(ch) || isdigit(ch) ) {
                    lexeme += ch;
                }
                else {
                    if( ch == '\n' )
                        (*linenum)--;
                    in->putback(ch);
                    return id_or_kw(lexeme, *linenum);
                }
                break;
                
            case INSTRING:
                lexeme += ch;
                if( ch == '\n' ) {
                    return Token(ERR, lexeme, *linenum );
                }
                if( ch == '"' ) {
                    lexeme = lexeme.substr(1, lexeme.length()-2);
                    return Token(SCONST, lexeme, *linenum );
                }
                break;
                
            case SAWMINUS:
                if( !isdigit(ch) ) {
                    if( ch == '\n' )
                        (*linenum)--;
                    in->putback(ch);
                    return Token(MINUS, lexeme, *linenum);
                }
                else {
                    lexeme += ch;
                    lexstate = ININT;
                }
                break;
                
            case ININT:
                if( isdigit(ch) ) {
                    lexeme += ch;
                }
                else if( isalpha(ch) ) {
                    lexeme += ch;
                    return Token(ERR, lexeme, *linenum);
                }
                else {
                    if( ch == '\n' )
                        (*linenum)--;
                    in->putback(ch);
                    return Token(ICONST, lexeme, *linenum);
                }
                break;
                
            case INCOMMENT:
                if( ch == '\n' ) {
                    lexstate = BEGIN;
                }
                break;
        }
        
    }
    
    if( in->eof() )
        return Token(DONE, "", *linenum);
    return Token(ERR, "some strange I/O error", *linenum);
}


*/


