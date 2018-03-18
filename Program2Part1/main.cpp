#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <map>
#include <vector>
#include <cctype>
using namespace std;

vector<string>operatorsVec {":", "+", "-", "*", "/", "%" , "="};

char *keywords[] = {"start", "finish", "then", "if", "repeat", "var",
    "int", "float", "do", "read", "print", "void", "return", "dummy", "program"};
char *identifiers[100]; //name of variables etc.
char comments[] = {"\n"};
char *relationalOperators[] = {"==", "<", ">", "=!=", "=>", "=<"};

char otherOperators[] = {':', '+', '-', '*', '/', '%' , '='};

char delimiters[] = {'.', '(', ')', ',', '{', '}', ';', '[', ']'};

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

typedef enum { unknown, wordish, spaceish } wordmode_t;

int main( int argc, char *argv[] )
{
    
    TType type;
    
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
    //char c='\0';
    TType tokenType = UNKNOWN;
    string line;
    
    string wordstuff="", spacestuff="";
    bool isKeyword = true, isIdentifier = true, isConstant = true, isOperator = true, isError = true, isDone = true;
    int nKeyword = 0, nIdentifier = 0, nConstant = 0, nOperator = 0, nError = 0, nDone = 0;
    
    //string   line;
    while(getline(file,line))
    {
        nLinesIn++;
        
        //I need to give linestream an actual token by itself################
        stringstream lineStream(line);
        string sToken = line;
        int tokenLine = nLinesIn;
        bool inOperators = false;
        
        int tokenLength = sToken.size();
        string theToken = "";
        char ch;
        //for (int i = 0; i < tokenLength; i++)
        while(lineStream.get(ch))
        {
            nCharsIn++;
            //char c = sToken[i], c1, c2;
            //c1 = line.peek();
            if (ch == '<')
            {
                char c = lineStream.peek();
                
                switch(c)
                {
                    case '<':
                        cout<<"There is 2 << in line: "<<nLinesIn<<endl;
                        theToken+=ch;
                        theToken+=c;
                        nToken++;
                        lineStream.get(c);//moves the pointer to ignore c
                        
                        break;
                    case ' ':
                        cout<<"There is only one < in line: "<<nLinesIn<<endl;
                        nToken++;
                        lineStream.get(c);
                    default:
                        nToken++;
                        //cout<<"default\n";
                        break;
                }
                
            }
            //cout<<c;
          // while(sToken.peek()=='<'){sToken.ignore(1,'<');}
        }
        cout<<theToken<<" in line#"<<nLinesIn<<endl;
        
        /*
        while(lineStream >> sToken)//grabs each tokens separated by lines
        {
            nToken++;
            
            string currentToken = sToken;
            if (find(operatorsVec.begin(), operatorsVec.end(), currentToken) != operatorsVec.end()) //if currentToken is find from operatorsVec
            {
                inOperators = true;
            }
            
            
            
            if (inOperators == true){cout<<"An operator token = ";}
            cout << "Token( " << currentToken<< " )\n";
            //cout<<" in line "<<tokenLine<<"\n";
            
            
        }//end of while(lineStream >> sToken)
        cout << "New Line Detected\n";
         */
        
    }//end of while getline
    
    cout<<"========RESULTS========\n";
    cout<<"There are "<<nCharsIn<<" characters"<<endl;
    cout<<"There are "<<nLinesIn<<" lines"<<endl;
    cout<<"There are "<<nToken<<" tokens"<<endl;

    return 0;
}
