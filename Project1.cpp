# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <string>
# include <iostream>
# include <sstream>
# include <vector>
# include <math.h>
# include <ctype.h>

# define NUM_INFINITY 69454559
using namespace std;

static int uTestNum = 0 ;
static float uTolerance = 0.0001; // tolerance number
static bool uFloat = false ;
static bool uBoolean = false ; // check whether has bool op

enum TokenType
{
    // define Token Type
    IDENT,
    NUM, // int or float and 3.4.5 case
    LEFT_PAREN, // (
    RIGHT_PAREN, // )
    ADD, // +
    MINUS, // -
    MULT, // *
    DIV, // /
    GREATER, // >
    LESS, // <
    GE, // >=
    LE, // <=
    EQ, // =
    NEQ, // <>
    ASSIGN, //  :=
    SEMICOLON // ;
};

struct Token {
  string tokenValue; // record data
  TokenType type; // to tell parser which type
};

struct Id {
  string id;
  float float_value;
  int int_value ;
};

class Scanner {
private:
  Token mBuf_token; // PeekToken()
public:
  Scanner() ;
  Token PeekToken() ;
  Token GetToken() ;
}; // Scanner

Scanner::Scanner() {
  // initial constructor
} // Scanner::Scanner()

Token Scanner::PeekToken() {
  // check next token type for parser
  if ( mBuf_token.tokenValue == "" ) {
    mBuf_token = GetToken();
  } // if
  
  return mBuf_token;
} // Scanner::PeekToken()

Token Scanner::GetToken() {
   // get the token and return to parser
  Token temp_Token;
  char peek_char; // peek_char for check peek char || get_char is actuaaly what you get 
  peek_char = cin.peek();
  if ( mBuf_token.tokenValue != "" ) {
    // if buffer it's not empty then use it and return for parser
    temp_Token.tokenValue = mBuf_token.tokenValue;
    temp_Token.type = mBuf_token.type;
    mBuf_token.tokenValue = ""; // empty the buffer
  } // if
  else {
    if ( peek_char == ' ' || peek_char == '\t' ) {
      cin.get(); // skip whitespace
      temp_Token = GetToken();
    } // if
    else if ( peek_char == '\n' ) {
      cin.get();
      temp_Token = GetToken();
    } // else if
    else if ( peek_char == '(' ) {
      // LP
      char get_char = cin.get();
      temp_Token.type = LEFT_PAREN;
      temp_Token.tokenValue = "(";
    } // else if
    else if ( peek_char == ')' ) {
      // RP
      char get_char = cin.get();
      temp_Token.type = RIGHT_PAREN;
      temp_Token.tokenValue = ")";
    } // else if
    else if ( peek_char == '+' ) {
      char get_char = cin.get();
      
      temp_Token.type = ADD;
      temp_Token.tokenValue = "+";
    } // else if
    else if ( peek_char == '-' ) {
      char get_char = cin.get();
      
      temp_Token.type = MINUS;
      temp_Token.tokenValue = "-";
    } // else if 
    else if ( peek_char == '*' ) {
      char get_char = cin.get();
      temp_Token.type = MULT;
      temp_Token.tokenValue = "*";

    } // else if
    else if ( peek_char == '/' ) {
      // check whether it is comment or divide
      char get_char = cin.get();
      char temp_peek = cin.peek();
      if ( temp_peek == '/' ) {
        // it's comment
        cin.get(); // read all comment
        temp_peek = cin.peek();
        while ( temp_peek != '\n' ) {
          // skip the comment
          cin.get();
          temp_peek = cin.peek();
        } // while()

        temp_Token = GetToken() ;
      } // if
      else {
        // it's divide
        temp_Token.type = DIV;
        temp_Token.tokenValue = "/";
      } // else
    } // else if 
    else if ( peek_char == '>' ) {
      char get_char = cin.get();
      char temp_peek = cin.peek();
      if ( temp_peek == '=' ) {
        // >=
        temp_Token.tokenValue = temp_Token.tokenValue + get_char;
        get_char = cin.get();
        temp_Token.tokenValue = temp_Token.tokenValue + get_char;
        temp_Token.type = GE;
      } // if
      else {
        temp_Token.type = GREATER;
        temp_Token.tokenValue = ">";
      } // else
    } // else if
    else if ( peek_char == '<' ) {
      char get_char = cin.get();
      char temp_peek = cin.peek();
      if ( temp_peek == '=' ) {
        // >=
        temp_Token.tokenValue = temp_Token.tokenValue + get_char;
        get_char = cin.get();
        temp_Token.tokenValue = temp_Token.tokenValue + get_char;
        temp_Token.type = LE;
      } // if
      else if ( temp_peek == '>' ) {
        // >>
        temp_Token.tokenValue = temp_Token.tokenValue + get_char;
        get_char = cin.get();
        temp_Token.tokenValue = temp_Token.tokenValue + get_char;
        temp_Token.type = NEQ;
      } // else if 
      else {
        temp_Token.type = LESS;
        temp_Token.tokenValue = "<";
      } // else
    } // else if
    else if ( peek_char == '=' ) {
      char get_char = cin.get();
      temp_Token.type = EQ;
      temp_Token.tokenValue = "=";
    } // else if
    else if ( peek_char == ';' ) {
      char get_char = cin.get();
      temp_Token.type = SEMICOLON;
      temp_Token.tokenValue = ";";
    } // else if 
    else if ( peek_char == ':' ) {
      char get_char = cin.get();
      char temp_peek = cin.peek();
      if ( temp_peek == '=' ) {
        temp_Token.tokenValue = temp_Token.tokenValue + get_char;
        get_char = cin.get();
        temp_Token.tokenValue = temp_Token.tokenValue + get_char;
        temp_Token.type = ASSIGN;
      } // if
      else {
        temp_Token.type = IDENT; // Unrecognized token
        temp_Token.tokenValue = ":";
      } // else
    } // else if
    else if ( peek_char >= '0' && peek_char <= '9' ) {
      // num or float
      char get_char = cin.get() ;
      temp_Token.tokenValue = temp_Token.tokenValue + get_char ;
      char temp_peek = cin.peek() ;
      while ( temp_peek == '.' || ( temp_peek >= '0' && temp_peek <= '9' ) ) {
        char get_char = cin.get();
        temp_Token.tokenValue = temp_Token.tokenValue + get_char;
        temp_peek = cin.peek();
      } // while()

      temp_Token.type = NUM ;
    } // else if
    else if ( peek_char == '.' ) {
      // .3 case  num
      char get_char = cin.get() ;
      temp_Token.tokenValue = temp_Token.tokenValue + get_char ;
      char temp_peek = cin.peek() ;
      while ( temp_peek == '.' || ( temp_peek >= '0' && temp_peek <= '9' ) ) {
        char get_char = cin.get();
        temp_Token.tokenValue = temp_Token.tokenValue + get_char;
        temp_peek = cin.peek();
      } // while()

      temp_Token.type = NUM ;
    } // else if
    else {
      // IDENT
      char get_char = cin.get();
      temp_Token.tokenValue = temp_Token.tokenValue + get_char;
      if ( ( ( get_char >= 'a' && get_char <= 'z' ) || 
             ( get_char >= 'A' && get_char <= 'Z' ) ) ) {

        char temp_peek = cin.peek();
        while ( ( ( temp_peek >= 'a' && temp_peek <= 'z' ) || 
                  ( temp_peek >= 'A' && temp_peek <= 'Z' ) || temp_peek == '_' ) ||
                ( temp_peek >= '0' && temp_peek <= '9' )  ) {

          char get_char = cin.get();
          temp_Token.tokenValue = temp_Token.tokenValue + get_char;
          temp_peek = cin.peek();
        } // while()
      } // if()
      
      temp_Token.type = IDENT;
    } // else
  } // else

  return temp_Token;
} // Scanner::GetToken()

class Parser {
private:
  Scanner mScanner;
  vector <Id> mIdList; // store id's value

public:
  bool Command() ;
  void ArithExp( bool& correct, float& value ) ;
  void Term( bool& correct, float& value ) ;
  void Factor( bool& correct, float& value ) ;

  void IDlessArithExpOrBexp( bool& correct, float& value ) ;
  void BooleanOperator( bool& correct ) ;
  void BooleanExp( Token token, float value1, float value2, bool &correct ) ;

  void NOT_IDStartArithExpOrBexp( bool& correct, float& value ) ;
  void NOT_ID_StartArithExp( bool& correct, float& value ) ;
  void NOT_ID_StartTerm( bool& correct, float& value ) ;
  void NOT_ID_StartFactor( bool& correct, float& value ) ;

  bool CheckInside( string& id ) ;
  void ChangeInsideValue( string id, float value ) ; // change vector's id value
  float GetIDvalue( string id ) ;
  void InitID( Id id ) ;

  bool IsRecognized( string& token ) ;
  bool HasDot( string& token ) ;
  void ErrorProcess() ;
  bool WrongFloat( string& token ) ;
  
  void Print_Unrecognized( string token ) ;
  void Print_Unexpected( string token ) ;
  void Print_Undefined( string token ) ;

  void Print_Float( float value ) ;
  void Print_Int( float value ) ;
}; // Parser

bool Parser::Command() {
  // interface for c interpreter
  printf( "> " ) ; // command prompt
  Token token, peek ;
  bool arithexp1Correct = false, boolean1Correct = false, booleanexp1Correct = false ;
  bool notidstartarithexporbexp1Correct = false ;
  float arithexp1Value = 0, notidstartarithexporbexp1Value = 0.0 ;
  Id id ; // store id
  // initial id value
  InitID( id ) ;
  peek = mScanner.PeekToken() ;
  if ( strcmp( peek.tokenValue.c_str(), "quit" ) == 0 ) {
    printf( "Program exits..." ) ;
    return false ;
  } // if
  else if ( peek.type == IDENT ) {
    // check the id is recognized
    token = mScanner.GetToken() ; // get the id
    if ( !IsRecognized( token.tokenValue ) ) {
      Print_Unrecognized( token.tokenValue ) ;
      ErrorProcess() ;
    } // if
    else {
      peek = mScanner.PeekToken() ; // peek the token
      id.id = token.tokenValue ; // give id
      if ( peek.type == ASSIGN ) {
        // Arithexp
        // this id can be undefined
        token = mScanner.GetToken() ; // get the assign op
        ArithExp( arithexp1Correct, arithexp1Value ) ;
        if ( arithexp1Correct ) {
          token = mScanner.GetToken() ; //  get ';'
          if ( token.type == SEMICOLON ) {
            // correct
            if ( uFloat ) {
              if ( CheckInside( id.id ) ) {
                ChangeInsideValue( id.id, arithexp1Value ) ;
                InitID( id ) ;
              } // if
              else {
                id.int_value = NUM_INFINITY ;
                id.float_value = arithexp1Value ;
                mIdList.push_back( id ) ;
                InitID( id ) ;
              } // else

              Print_Float( arithexp1Value ) ;
              uFloat = false ; // close float print
            } // if
            else {
              if ( CheckInside( id.id ) ) {
                ChangeInsideValue( id.id, arithexp1Value ) ;
                InitID( id ) ;
                Print_Int( arithexp1Value ) ;

              } // if
              else {
                id.float_value = NUM_INFINITY ;
                id.int_value = arithexp1Value ;
                mIdList.push_back( id ) ;
                InitID( id ) ;
                Print_Int( arithexp1Value ) ;

              } // else
            } // else
          } // if
          else {
            // check unrecognized or unexpected
            if ( !IsRecognized( token.tokenValue ) ) {
              Print_Unrecognized( token.tokenValue ) ;
            } // if
            else {
              Print_Unexpected( token.tokenValue ) ;
            } // else

            ErrorProcess() ;
          } // else	
        } // if
      } // if
      else if ( peek.type == SEMICOLON ) {
        // a ; case printf the value
        token = mScanner.GetToken() ; // get ';'

        if ( CheckInside( id.id ) ) {
          // has defined
          int print_int ;
          float print_float ;
          for ( int i = 0 ; i < mIdList.size() ; i++ ) {
            if ( strcmp( mIdList[i].id.c_str(), id.id.c_str() ) == 0 ) {
              // choose which type
              if ( mIdList[i].int_value != NUM_INFINITY ) {
                print_int = mIdList[i].int_value ;
                Print_Int( print_int ) ;
              } // if
              else {
                print_float = mIdList[i].float_value ;
                Print_Float( print_float ) ;
              } // else
            } // if
          } // for
        } // if
        else {
          // not defined
          Print_Undefined( id.id ) ;
          ErrorProcess() ;
        } // else
      } // else if
      else if ( peek.type == ADD || peek.type == MINUS || peek.type == MULT || peek.type == DIV ) {
        // IDlessarithexporbexp
        // this id should be defined
        bool idless1Correct = false, idHasFloat = false ;
        float id_value = 0.0 ;
        if ( CheckInside( token.tokenValue ) ) {
          // take the value
          for ( int i = 0 ; i < mIdList.size() ; i++ ) {
            if ( strcmp( token.tokenValue.c_str(), mIdList[i].id.c_str() ) == 0 ) {
              // value = mIdList[i].value;
              if ( mIdList[i].int_value != NUM_INFINITY ) {
                id_value = mIdList[i].int_value ;
              } // if
              else {
                idHasFloat = true ;
                id_value = mIdList[i].float_value ;
              } // else
            } // if
          } // for

          IDlessArithExpOrBexp( idless1Correct, id_value ) ; // give it id's value

          if ( idless1Correct ) {
            // check result : id_value is float or int
            BooleanOperator( boolean1Correct ) ;
            if ( !boolean1Correct ) {
              token = mScanner.GetToken() ; // get the ';'
              if ( token.type == SEMICOLON ) {
                // print the output
                if ( idHasFloat || uFloat ) {
                  Print_Float( id_value ) ;
                  uFloat = false ;
                } // if
                else {
                  Print_Int( id_value ) ;
                } // else
              } // if
              else {
                // check the token is unrecognized or unexpected
                if ( !IsRecognized( token.tokenValue ) ) {
                  // unrecognized
                  Print_Unrecognized( token.tokenValue ) ;
                } // if
                else {
                  Print_Unexpected( token.tokenValue ) ;
                } // else

                ErrorProcess() ; // read all garbage
              } // else
            } // if
            else {
              // id_value is IDless's value
              // do comparision and output 'true' or 'false'
              token = mScanner.GetToken() ; // get the operator '=' '<>' '>' ...
              ArithExp( arithexp1Correct, arithexp1Value ) ;
              if ( arithexp1Correct ) {
                peek = mScanner.PeekToken() ; // check ';'
                if ( peek.type == SEMICOLON ) {
                  // do comparision !
                  BooleanExp( token, id_value, arithexp1Value, booleanexp1Correct ) ;
                  // in the end get the ';' token
                  token = mScanner.GetToken() ;
                } // if
                else {
                  token = mScanner.GetToken() ; // get the wrong token
                  if ( !IsRecognized( token.tokenValue ) ) {
                    Print_Unrecognized( token.tokenValue ) ;
                  } // if
                  else {
                    Print_Unexpected( token.tokenValue ) ;
                  } // else

                  ErrorProcess() ;
                } // else
              } // if
              else {
                // arithexp wrong
                ErrorProcess() ;
              } // else
            } // else
          } // if
          else {
            // wrong idlessarithexporbexp
            ErrorProcess() ;
          } // else
        } // if
        else {
          Print_Undefined( token.tokenValue ) ;
          mScanner.GetToken() ; // get op token
          ErrorProcess() ;
        } // else
      } // else if 
      else if ( peek.type == EQ || peek.type == NEQ || peek.type == GREATER || peek.type == LESS || 
                peek.type == GE || peek.type == LE ) {
        // a > 5 ; case 
        float id_value = 0.0 ;
        if ( CheckInside( token.tokenValue ) ) {
          // if it's inside then get the value 
          for ( int i = 0 ; i < mIdList.size() ; i++ ) {
            if ( strcmp( token.tokenValue.c_str(), mIdList[i].id.c_str() ) == 0 ) {
              // value = mIdList[i].value;
              if ( mIdList[i].int_value != NUM_INFINITY ) {
                id_value = mIdList[i].int_value ;
              } // if
              else {
                id_value = mIdList[i].float_value ;
              } // else
            } // if
          } // for

          token = mScanner.GetToken() ; // get the operator
          ArithExp( arithexp1Correct, arithexp1Value ) ;
          if ( arithexp1Correct ) {
            peek = mScanner.PeekToken() ; // check ';'
            if ( peek.type == SEMICOLON ) {
              // do comparision !
              BooleanExp( token, id_value, arithexp1Value, booleanexp1Correct ) ;
              // in the end get the ';' token
              token = mScanner.GetToken() ;
            } // if
            else {
              token = mScanner.GetToken() ; // get the wrong token
              if ( !IsRecognized( token.tokenValue ) ) {
                Print_Unrecognized( token.tokenValue ) ;
              } // if
              else {
                Print_Unexpected( token.tokenValue ) ;
              } // else

              ErrorProcess() ;
            } // else
          } // if
          else {
            // arithexp wrong
            ErrorProcess() ;
          } // else
        } // if
        else {
          // this id doesn't defined 
          Print_Undefined( token.tokenValue ) ;
          mScanner.GetToken() ;
          ErrorProcess() ;
        } // else
      } // else if
      else {
        // error 
        token = mScanner.GetToken() ;
        if ( !IsRecognized( token.tokenValue ) ) {
          Print_Unrecognized( token.tokenValue ) ;
        } // if
        else {
          Print_Unexpected( token.tokenValue ) ;
        } // else

        ErrorProcess() ; // collect the garbage        
      } // else
    } // else
  } // if
  else if ( peek.type == ADD || peek.type == MINUS || peek.type == NUM || peek.type == LEFT_PAREN ) {
    // notstartarithexporbexp
    NOT_IDStartArithExpOrBexp( notidstartarithexporbexp1Correct, notidstartarithexporbexp1Value ) ;
    if ( notidstartarithexporbexp1Correct ) {
      token = mScanner.GetToken() ; // get the ';'
      if ( token.type == SEMICOLON ) {
        // choose type of print
        if ( !uBoolean ) {
          if ( uFloat ) {
            Print_Float( notidstartarithexporbexp1Value ) ;
            uFloat = false ; // close float print
          } // if
          else {
            Print_Int( notidstartarithexporbexp1Value ) ;
          } // else
        } // if

        uBoolean = false ;
        uFloat = false ;
      } // if
      else {
        if ( !IsRecognized( token.tokenValue ) ) {
          Print_Unrecognized( token.tokenValue ) ;
        } // if
        else {
          Print_Unexpected( token.tokenValue ) ;
        } // else

        ErrorProcess() ;
      } // else
    } // if
  } // else if
  else {
    // error then determine unrecognized or unexpected
    token = mScanner.GetToken() ; // get the peek token
    if ( !IsRecognized( token.tokenValue ) ) {
      // Unrecognized token with first char : ' '
      Print_Unrecognized( token.tokenValue ) ;
    } // if
    else {
      // Unexpected token : ' '
      Print_Unexpected( token.tokenValue ) ;
    } // else

    ErrorProcess() ; // collect the garbage 
  } // else

  uBoolean = false ;
  uFloat = false ;
  return true ;  
} // Parser::Command()

void Parser::ArithExp( bool& correct, float& value ) {
    // <ArithExp>              ::= <Term> { '+' <Term> | '-' <Term> }
  float term1Value = 0.0, term2Value = 0.0;
  bool term1Correct = false, term2Correct = false;
  Term( term1Correct, term1Value );
  if ( !term1Correct ) {
    correct = false;
    value = 0.0;
    return;
  } // if
  
  do
  {
    Token peek = mScanner.PeekToken();
    if ( peek.type != ADD && peek.type != MINUS ) {
      correct = true;
      value = term1Value;
      return;
    } // if

    Token token = mScanner.GetToken(); // get + or -
    Term( term2Correct, term2Value );
    if ( !term2Correct ) {
      correct = false;
      value = 0.0;
      return;
    } // if

    correct = true;
    if ( token.type == ADD ) {
      term1Value = term1Value + term2Value;
    } // if
    else if ( token.type == MINUS ) {
      term1Value = term1Value - term2Value;
    } // else if
  } while ( 1 );
} // Parser::ArithExp()

void Parser::Term( bool& correct, float& value ) {
    // <Term>                  ::= <Factor> { '*' <Factor> | '/' <Factor> }

  float factor1Value = 0.0, factor2Value = 0.0;
  bool factor1Correct = false, factor2Correct = false;
  Factor( factor1Correct, factor1Value );
  if ( !factor1Correct ) {
    correct = false;
    value = 0.0;
    return;
  } // if
  
  do
  {
    Token peek = mScanner.PeekToken();
    if ( peek.type != MULT && peek.type != DIV ) {
      correct = true;
      value = factor1Value;
      return;
    } // if
    
    Token token = mScanner.GetToken();
    Factor( factor2Correct, factor2Value );
    if ( !factor2Correct ) {
      correct = false;
      value = 0.0;
      return;
    } // if
    
    correct = true;
    if ( token.type == MULT ) {
      factor1Value = factor1Value * factor2Value;
      value = factor1Value;
    } // if
    else {
      if ( factor2Value == 0 ) {
        // divide by 0 
        string output = "Error" ;
        printf( "%s\n", output.c_str() ) ;
        ErrorProcess() ;
        correct = false ;
        return ;
      } // if
      else {
        factor1Value = factor1Value / factor2Value;
      } // else
    } // else
  } while ( 1 );
} // Parser::Term()

void Parser::Factor( bool& correct, float& value ) {
  Token token = mScanner.GetToken() ;
  bool arithexp1Correct = false ;
  float arithexp1Value = 0.0 ;
  if ( token.type != NUM && token.type != IDENT && token.type != LEFT_PAREN &&
       token.type != ADD && token.type != MINUS ) {
    if ( !IsRecognized( token.tokenValue ) ) {
      Print_Unrecognized( token.tokenValue ) ;
    } // if
    else {
      // unexpected token
      Print_Unexpected( token.tokenValue ) ;
    } // else

    ErrorProcess() ;
    correct = false;
    value = 0.0;
    return;
  } // if

  if ( token.type == IDENT ) {
    // IDENT
    // first check recognized
    if ( IsRecognized( token.tokenValue ) ) {
      // check defined or not
      if ( CheckInside( token.tokenValue ) ) {
        // has defined 
        // take value out 
        for ( int i = 0 ; i < mIdList.size() ; i++ ) {
          if ( strcmp( token.tokenValue.c_str(), mIdList[i].id.c_str() ) == 0 ) {
            // value = mIdList[i].value;
            if ( mIdList[i].int_value != NUM_INFINITY ) {
              value = mIdList[i].int_value ;
            } // if
            else {
              value = mIdList[i].float_value ;
              uFloat = true ;
            } // else

            correct = true;
            return; // if get the value then jumpout the factor 
          } // if
        } // for
      } // if
      else {
        // Undefined
        Print_Undefined( token.tokenValue ) ;
        ErrorProcess() ;
        correct = false ;
        value = 0.0 ;
        return ;
      } // else
    } // if
    else {
      // Unrecognized
      Print_Unrecognized( token.tokenValue ) ;
      ErrorProcess() ;
      correct = false ;
      value = 0.0 ;
      return ;
    } // else
  } // if
  else if ( token.type == ADD || token.type == MINUS ) {
    // [sign] NUM
    Token peek = mScanner.PeekToken() ; // peek the NUM token
    if ( peek.type == NUM ) {
      // get num -> sign num
      // first check float. If it's not a float then is a integer
      if ( HasDot( peek.tokenValue ) ) {
        // is float
        // need to check this float is correct  3.4.5 case
        if ( !WrongFloat( peek.tokenValue ) ) {
          // correct float
          uFloat = true ; // float arithexp open
          string sign_num = token.tokenValue + peek.tokenValue ;
          token = mScanner.GetToken() ; // get the NUM
          float float_value ; 
          stringstream ss ;
          ss << sign_num ;
          ss >> float_value ;
          value = float_value ;
          correct = true ;
          return ; // get the num then retrun 
        } // if
        else {
          // wrong float  ex. 3.4.5 ...
          token = mScanner.GetToken() ; // get the wrong NUM
          string wrongfloat ;
          int checkDot = 0 ; // count the dot
          for ( int i = 0 ; i < token.tokenValue.size() ; i++ ) {
            if ( token.tokenValue[i] == '.' ) {
              checkDot++ ;
            } // if
            
            if ( checkDot == 2 ) {
              // error occur. store unexpected token .5 
              wrongfloat = wrongfloat + token.tokenValue[i] ;
            } // if
          } // for

          Print_Unexpected( wrongfloat ) ;
          correct = false ;
          value = 0.0 ;
          ErrorProcess() ;
          return ;
        } // else
      } // if
      else {
        // is integer
        string sign_num = token.tokenValue + peek.tokenValue ;
        token = mScanner.GetToken() ; // get the num
        int integer_value ; 
        stringstream ss ;
        ss << sign_num ;
        ss >> integer_value ;
        value = integer_value ;
        correct = true ;
        return ; // get the num then retrun 
      } // else
    } // if
    else {
      // first check unrecognized then unexpected
      // get the wrong token
      token = mScanner.GetToken() ;
      if ( !IsRecognized( token.tokenValue ) ) {
        Print_Unrecognized( token.tokenValue ) ;
      } // if
      else {
        Print_Unexpected( token.tokenValue ) ;
      } // else

      ErrorProcess() ;
      correct = false ;
      value = 0.0 ;
      return ;
    } // else
  } // else if
  else if ( token.type == NUM ) {
    // NUM
    // check float or integer
    if ( HasDot( token.tokenValue ) ) {
      // it's float check correct or not
      if ( !WrongFloat( token.tokenValue ) ) {
        // correct float
        uFloat = true ; // float arithexp open
        float float_value ; 
        stringstream ss ;
        ss << token.tokenValue ;
        ss >> float_value ;
        value = float_value ;
        correct = true ;
        return ; // get the num then retrun 
      } // if
      else {
        // wrong float  ex. 3.4.5 ...
        string wrongfloat ;
        int checkDot = 0 ; // count the dot
        for ( int i = 0 ; i < token.tokenValue.size() ; i++ ) {
          if ( token.tokenValue[i] == '.' ) {
            checkDot++ ;
          } // if
            
          if ( checkDot == 2 ) {
            // error occur. store unexpected token .5 
            wrongfloat = wrongfloat + token.tokenValue[i] ;
          } // if
        } // for

        Print_Unexpected( wrongfloat ) ;
        correct = false ;
        value = 0.0 ;
        ErrorProcess() ;
        return ;
      } // else
    } // if
    else {
      // it's integer
      int integer_value ; 
      stringstream ss ;
      ss << token.tokenValue ;
      ss >> integer_value ;
      value = integer_value ;
      correct = true ;
      return ; // get the num then retrun 
    } // else
  } // else if
  else {
    // LEFT_PAREN
    ArithExp( arithexp1Correct, arithexp1Value ) ;
    if ( !arithexp1Correct ) {
      correct = false ;
      value = 0.0 ;
      return ;
    } // if
    else {
      // get the ')' RP
      token = mScanner.GetToken() ;
      if ( token.type == RIGHT_PAREN ) {
        // correct
        correct = true ;
        value = arithexp1Value ;
        return ;
      } // if
      else {
        // check unrecognized or unexpected
        if ( !IsRecognized( token.tokenValue ) ) {
          Print_Unrecognized( token.tokenValue ) ;
        } // if
        else {
          Print_Unexpected( token.tokenValue ) ;
        } // else

        ErrorProcess() ;
        correct = false ;
        value = 0.0 ;
        return ;
      } // else
    } // else
  } // else
} // Parser::Factor()

void Parser::BooleanOperator( bool& correct ) {
    // <BooleanOprator>        ::= '=' | '<>' | '>' | '<' | '>=' | '<='
  Token peek = mScanner.PeekToken();
  if ( peek.type != EQ && peek.type != NEQ && peek.type != GREATER
       && peek.type != LESS && peek.type != GE && peek.type != LE ) {
    correct = false;
    return;
  } // if
  else {
    correct = true;
    return;
  } // else
} // Parser::BooleanOperator()

void Parser::BooleanExp( Token token, float value1, float value2, bool& correct ) {
  if ( token.type == EQ ) {
    if ( value1 > value2 ) {
      if ( value1 - value2 <= uTolerance ) {
        printf( "true\n" ) ;
      } // if
      else {
        printf( "false\n" ) ;
      } // else
    } // if
    else if ( value1 == value2 ) {
      printf( "true\n" ) ;
    } // else if
    else {
      if ( value2 - value1 <= uTolerance ) {
        printf( "true\n" ) ;
      } // if
      else {
        printf( "false\n" ) ;
      } // else
    } // else
  } // if
  else if ( token.type == NEQ ) {
    if ( value1 > value2 ) {
      if ( value1 - value2 <= uTolerance ) {
        printf( "false\n" ) ;
      } // if
      else {
        printf( "true\n" ) ;
      } // else
    } // if
    else if ( value1 == value2 ) {
      printf( "false\n" ) ;
    } // else if
    else {
      if ( value2 - value1 <= uTolerance ) {
        printf( "false\n" ) ;
      } // if
      else {
        printf( "true\n" ) ;
      } // else
    } // else
  } // else if
  else if ( token.type == GREATER ) {
    if ( value1 - uTolerance > value2 ) {
      printf( "true\n" ) ;
    } // if
    else {
      printf( "false\n" ) ;
    } // else
  } // else if
  else if ( token.type == LESS ) {
    if ( value1 - uTolerance < value2 ) {
      printf( "true\n" ) ;
    } // if
    else {
      printf( "false\n" ) ;
    } // else
  } // else if
  else if ( token.type == GE ) {
    if ( value1 - uTolerance >= value2 ) {
      printf( "true\n" ) ;
    } // if
    else {
      printf( "false\n" ) ;
    } // else
  } // else if
  else if ( token.type == LE ) {
    if ( value1 - uTolerance <= value2 ) {
      printf( "true\n" ) ;
    } // if
    else {
      printf( "false\n" ) ;
    } // else
  } // else if
} // Parser::BooleanExp()

void Parser::IDlessArithExpOrBexp( bool& correct, float& value ) {
  bool term1Correct = false, factor1Correct = false, arithexp1Correct = false ;
  float term1Value = 0.0, factor1Value = 0.0, arithexp1Value = 0.0 ;
  bool boolean1Correct = false ;
  do
  {
    Token peek = mScanner.PeekToken() ;
    if ( peek.type != ADD && peek.type != MINUS && peek.type != MULT && peek.type != DIV ) {
      // check it's boolean or not
      BooleanOperator( boolean1Correct ) ;
      if ( boolean1Correct ) {
        // has boolean operator and don't peek any token because the next step
        correct = true ; // do boolean op
        return ; // return to command function to do bollean op
      } // if
      else {

        // error Check token
        correct = false ;
        value = 0.0 ;
        Token token = mScanner.GetToken() ; // get the wrong token
        if ( !IsRecognized( token.tokenValue ) ) {
          Print_Unrecognized( token.tokenValue ) ;
          ErrorProcess() ;
        } // if
        else {
          Print_Unexpected( token.tokenValue ) ;
          ErrorProcess() ;
        } // else
      } // else

      return ;
    } // if

    correct = true ;
    Token token = mScanner.GetToken() ; // get the op
    if ( token.type == ADD || token.type == MINUS ) {
      Term( term1Correct, term1Value ) ;
      if ( term1Correct ) {
        if ( token.type == ADD ) {
          value = value + term1Value ;
        } // if
        else {
          value = value - term1Value ;
        } // else

        peek = mScanner.PeekToken() ;
        if ( peek.type == SEMICOLON ) {
          correct = true ;
          return ;
        } // if
      } // if
      else {
        correct = false ;
        value = 0.0 ;
        return ;
      } // else
    } // if
    else if ( token.type == MULT || token.type == DIV ) {
      Factor( factor1Correct, factor1Value );
      if ( factor1Correct ) {
        if ( token.type == MULT ) {
          value = value * factor1Value ;
        } // if
        else {
          if ( factor1Value == 0 ) {
            // divide by 0 case 
            string output = "Error" ;
            printf( "%s\n", output.c_str() ) ;
            ErrorProcess() ;
            correct = false ;
            return ;
          } // if
          else {
            value = value / factor1Value ;
          } // else
        } // else

        peek = mScanner.PeekToken() ;
        if ( peek.type == SEMICOLON ) {
          correct = true ;
          return ;
        } // if
      } // if
      else {
        correct = false ;
        value = 0.0 ;
        return ;
      } // else
    } // else if
  } while ( 1 ) ;
} // Parser::IDlessArithExpOrBexp()

void Parser::NOT_IDStartArithExpOrBexp( bool& correct, float& value ) {
  // <NOT_ID_StartArithExp> 
  //    [<BooleanOperator> <ArithExp>]

  bool notIDStartArith1Correct = false, boolean1Correct = false ;
  bool arithexp1Correct = false, booleanexp1Correct = false ;
  float notIDStartArith1Value = 0.0, arithexp1Value = 0.0 ;
  NOT_ID_StartArithExp( notIDStartArith1Correct, notIDStartArith1Value ) ;

  if ( !notIDStartArith1Correct ) {
    correct = false ;
    value = 0.0 ;
    return ;
  } // if
  else {
    correct = true ;
    value = notIDStartArith1Value ;
    BooleanOperator( boolean1Correct ) ;
    if ( boolean1Correct ) {
      Token token = mScanner.GetToken() ; // get the operator
      ArithExp( arithexp1Correct, arithexp1Value ) ;
      if ( arithexp1Correct ) {
        Token peek = mScanner.PeekToken() ; // check ';'
        if ( peek.type == SEMICOLON ) {
          // do comparision !
          BooleanExp( token, notIDStartArith1Value, arithexp1Value, booleanexp1Correct ) ;
          uBoolean = true ;
          // in the end get the ';' token
        } // if
        else {
          token = mScanner.GetToken() ; // get the wrong token
          if ( !IsRecognized( token.tokenValue ) ) {
            Print_Unrecognized( token.tokenValue ) ;
          } // if
          else {
            Print_Unexpected( token.tokenValue ) ;
          } // else

          ErrorProcess() ;
          correct = false ;
          value = 0.0 ;
          return ;
        } // else
      } // if
      else {
        correct = false ;
        value = 0.0 ;
        return ;
      } // else
    } // if
  } // else
} // Parser::NOT_IDStartArithExpOrBexp()

void Parser::NOT_ID_StartArithExp( bool& correct, float& value ) {
  // <NOT_ID_StartTerm> { '+' <Term> | '-' <Term> }
  bool notIDStartTerm1Correct = false, term1Correct = false;
  float notIDStartTerm1Value = 0.0, term1Value = 0.0;
  NOT_ID_StartTerm( notIDStartTerm1Correct, notIDStartTerm1Value );
  if ( !notIDStartTerm1Correct ) {
    correct = false;
    value = 0.0;
    return;
  } // if
  
  do
  {
    Token peek = mScanner.PeekToken();
    if ( peek.type != ADD && peek.type != MINUS ) {
      correct = true;
      value = notIDStartTerm1Value;
      return;
    } // if

    Token token = mScanner.GetToken(); // get + or -
    Term( term1Correct, term1Value );
    if ( !term1Correct ) {
      correct = false;
      value = 0.0;
      return;
    } // if

    correct = true;
    if ( token.type == ADD ) {
      notIDStartTerm1Value = notIDStartTerm1Value + term1Value;
    } // if
    else if ( token.type == MINUS ) {
      notIDStartTerm1Value = notIDStartTerm1Value - term1Value;
    } // else if
  } while ( 1 );
} // Parser::NOT_ID_StartArithExp()

void Parser::NOT_ID_StartTerm( bool& correct, float& value ) {
  // <NOT_ID_StartFactor> { '*' <Factor> | '/' <Factor> }
  bool notIDStartFactor1Correct = false, factor1Correct = false;
  float notIDStartFactor1Value = 0.0, factor1Value = 0.0;
  NOT_ID_StartFactor( notIDStartFactor1Correct, notIDStartFactor1Value );
  if ( !notIDStartFactor1Correct ) {
    correct = false;
    value = 0.0;
    return;
  } // if
  
  do
  {
    Token peek = mScanner.PeekToken();
    if ( peek.type != MULT && peek.type != DIV ) {
      correct = true;
      value = notIDStartFactor1Value;
      return;
    } // if
    
    Token token = mScanner.GetToken();
    Factor( factor1Correct, factor1Value );
    if ( !factor1Correct ) {
      correct = false;
      value = 0.0;
      return;
    } // if
    
    correct = true;
    if ( token.type == MULT ) {
      notIDStartFactor1Value = notIDStartFactor1Value * factor1Value;
      value = notIDStartFactor1Value;
    } // if
    else {
      if ( factor1Value == 0 ) {
        // divide by 0 
        string output = "Error" ;
        printf( "%s\n", output.c_str() ) ;
        ErrorProcess() ;
        correct = false ;
        return ;
      } // if
      else {
        notIDStartFactor1Value = notIDStartFactor1Value / factor1Value;
        value = notIDStartFactor1Value ;
      } // else
    } // else
  } while ( 1 );
} // Parser::NOT_ID_StartTerm()

void Parser::NOT_ID_StartFactor( bool& correct, float& value ) {
  // [ SIGN ] NUM | '(' <ArithExp> ')'
  Token token = mScanner.GetToken() ;
  bool arithexp1Correct = false ;
  float arithexp1Value = 0.0 ;

  if ( token.type != NUM && token.type != LEFT_PAREN && token.type != ADD && token.type != MINUS ) {
    if ( !IsRecognized( token.tokenValue ) ) {
      Print_Unrecognized( token.tokenValue ) ;
    } // if
    else {
      Print_Unexpected( token.tokenValue ) ;
    } // else

    ErrorProcess() ;
    correct = false ;
    value = 0.0 ;
    return ;
  } // if

  if ( token.type == ADD || token.type == MINUS ) {
    // [sign] NUM
    Token peek = mScanner.PeekToken() ; // peek the NUM token
    if ( peek.type == NUM ) {
      // get num -> sign num
      // first check float. If it's not a float then is a integer
      if ( HasDot( peek.tokenValue ) ) {
        // is float
        // need to check this float is correct  3.4.5 case
        if ( !WrongFloat( peek.tokenValue ) ) {
          // correct float
          uFloat = true ; // float arithexp open
          string sign_num = token.tokenValue + peek.tokenValue ;
          token = mScanner.GetToken() ; // get the NUM
          float float_value ; 
          stringstream ss ;
          ss << sign_num ;
          ss >> float_value ;
          value = float_value ;
          correct = true ;
          return ; // get the num then retrun 
        } // if
        else {
          // wrong float  ex. 3.4.5 ...
          token = mScanner.GetToken() ; // get the wrong NUM
          string wrongfloat ;
          int checkDot = 0 ; // count the dot
          for ( int i = 0 ; i < token.tokenValue.size() ; i++ ) {
            if ( token.tokenValue[i] == '.' ) {
              checkDot++ ;
            } // if
            
            if ( checkDot == 2 ) {
              // error occur. store unexpected token .5 
              wrongfloat = wrongfloat + token.tokenValue[i] ;
            } // if
          } // for

          Print_Unexpected( wrongfloat ) ;
          correct = false ;
          value = 0.0 ;
          ErrorProcess() ;
          return ;
        } // else
      } // if
      else {
        // is integer
        string sign_num = token.tokenValue + peek.tokenValue ;
        token = mScanner.GetToken() ; // get the num
        int integer_value ; 
        stringstream ss ;
        ss << sign_num ;
        ss >> integer_value ;
        value = integer_value ;
        correct = true ;
        return ; // get the num then retrun 
      } // else
    } // if
    else {
      // first check unrecognized then unexpected
      // get the wrong token
      token = mScanner.GetToken() ;
      if ( !IsRecognized( token.tokenValue ) ) {
        Print_Unrecognized( token.tokenValue ) ;
        ErrorProcess() ; // collect garbage
      } // if
      else {
        Print_Unexpected( token.tokenValue ) ;
        ErrorProcess() ; // collect garbage
      } // else

      correct = false ;
      value = 0.0 ;
      return ;
    } // else
  } // if
  else if ( token.type == NUM ) {
    // NUM
    // check float or integer
    if ( HasDot( token.tokenValue ) ) {
      // it's float check correct or not
      if ( !WrongFloat( token.tokenValue ) ) {
        // correct float
        uFloat = true ; // float arithexp open
        float float_value ; 
        stringstream ss ;
        ss << token.tokenValue ;
        ss >> float_value ;
        value = float_value ;
        correct = true ;
        return ; // get the num then retrun 
      } // if
      else {
        // wrong float  ex. 3.4.5 ...
        string wrongfloat ;
        int checkDot = 0 ; // count the dot
        for ( int i = 0 ; i < token.tokenValue.size() ; i++ ) {
          if ( token.tokenValue[i] == '.' ) {
            checkDot++ ;
          } // if
            
          if ( checkDot == 2 ) {
            // error occur. store unexpected token .5 
            wrongfloat = wrongfloat + token.tokenValue[i] ;
          } // if
        } // for

        Print_Unexpected( wrongfloat ) ;
        correct = false ;
        value = 0.0 ;
        ErrorProcess() ;
        return ;
      } // else
    } // if
    else {
      // it's integer
      int integer_value ; 
      stringstream ss ;
      ss << token.tokenValue ;
      ss >> integer_value ;
      value = integer_value ;
      correct = true ;
      return ; // get the num then retrun 
    } // else
  } // else if
  else {
    // '(' ArithExp ')'
    ArithExp( arithexp1Correct, arithexp1Value ) ;
    if ( !arithexp1Correct ) {
      correct = false ;
      value = 0.0 ;
      return ;
    } // if
    else {
      // get the ')' RP
      token = mScanner.GetToken() ;
      if ( token.type == RIGHT_PAREN ) {
        // correct
        correct = true ;
        value = arithexp1Value ;
        return ;
      } // if
      else {
        // check unrecognized or unexpected
        if ( !IsRecognized( token.tokenValue ) ) {
          Print_Unrecognized( token.tokenValue ) ;
        } // if
        else {
          Print_Unexpected( token.tokenValue ) ;
        } // else

        ErrorProcess() ;
        correct = false ;
        value = 0.0 ;
        return ;
      } // else
    } // else
  } // else  
} // Parser::NOT_ID_StartFactor()

bool Parser::CheckInside( string& id ) {
  // check this id has already in the vector
  for ( int i = 0 ; i < mIdList.size() ; i++ ) {
    if ( strcmp( id.c_str(), mIdList[i].id.c_str() ) == 0 ) {
      return true;
    } // if
  } // for
  
  return false;
} // Parser::CheckInside()

void Parser::ChangeInsideValue( string id, float value ) {
  // renew the value in the id
  for ( int i = 0 ; i < mIdList.size() ; i++ ) {
    if ( strcmp( id.c_str(), mIdList[i].id.c_str() ) == 0 ) {
      // change inside value
      if ( uFloat ) { 
        mIdList.at( i ).float_value = value ;
        mIdList.at( i ).int_value = NUM_INFINITY ;
      } // if 
      else {
        mIdList.at( i ).int_value = value ;
        mIdList.at( i ).float_value = NUM_INFINITY ;
      } // else
    } // if
  } // for
} // Parser::ChangeInsideValue() 


void Parser::InitID( Id id ) {
  id.id = "" ;
  id.float_value = NUM_INFINITY ;
  id.int_value = NUM_INFINITY ;
} // Parser::InitID()

bool Parser::IsRecognized( string& token ) {
  // determine the token is recognized
  if ( strcmp( token.c_str(), ":=" ) == 0 ) {
    return true ;
  } // if

  if ( token[0] == '_' ) {
    return false ;
  } // if

  for ( int i = 0 ; i < token.size() ; i++ ) {
    if ( token[i] != '_' && token[i] != '*' && token[i] != '(' && token[i] != ')' && token[i] != '-' && 
         token[i] != '+' && token[i] != ';' &&  ( token[i] < '0' || token[i] > '9'  ) &&
         ( ( token[i] < 'a' || token[i] > 'z' ) && 
           ( token[i] < 'A' || token[i] > 'Z' ) && token[i] != '=' ) && token[i] != '/' &&
         token[i] != '<' && token[i] != '>' && token[i] != '.'  ) {

      return false ;
    } // if
  } // for

  return true ;
} // Parser::IsRecognized()

bool Parser::HasDot( string& token ) {
  // determine the token value is float 
  for ( int i = 0 ; i < token.size() ; i++ ) {
    if ( token[i] == '.' ) {
      return true ;
    } // if
  } // for
  
  return false ;
} // Parser::HasDot()

bool Parser::WrongFloat( string& token ) {
  // try to catch 3.4.5 wrong float case
  int checkDot = 0 ;
  for ( int i = 0 ; i < token.size() ; i++ ) {
    if ( token[i] == '.' ) {
      checkDot++ ;
    } // if

    if ( checkDot >= 2 ) {
      return true ;
    } // if
  } // for
  
  return false ;
} // Parser::WrongFloat()

void Parser::ErrorProcess() {
  // read all garbage char
  char temp_peek = cin.peek() ;
  while ( temp_peek != '\n' ) {
    cin.get() ;
    temp_peek = cin.peek() ;
  } // while()
} // Parser::ErrorProcess()

void Parser::Print_Unrecognized( string token ) {
  string output = "Unrecognized token with first char : '" + token + "'" ;
  printf( "%s\n", output.c_str() ) ; // c++ string to c string
} // Parser::Print_Unrecognized()

void Parser::Print_Unexpected( string token ) {
  string output = "Unexpected token : '" + token + "'" ;
  printf( "%s\n", output.c_str() ) ; // c++ string to c string
} // Parser::Print_Unexpected()

void Parser::Print_Undefined( string token ) {
  string output = "Undefined identifier : '" + token + "'" ;
  printf( "%s\n", output.c_str() ) ; // c++ string to c string
} // Parser::Print_Undefined()

void Parser::Print_Float( float value ) {
  printf( "%1.3f\n", value ) ;
} // Parser::Print_Float()

void Parser::Print_Int( float value ) {
  int num = value ; // float to string
  printf( "%d\n", num ) ;
} // Parser::Print_Int()

int main() {
  char ch = '\0' ; // read '\n'
  Token token ;
  bool jumpOut = false ;
  Parser parser ;
  scanf( "%d%c", &uTestNum, &ch ) ;
  printf( "Program starts...\n" ) ;
  while ( jumpOut != true ) {
    if ( !parser.Command() ) {
      jumpOut = true ;
    } // if
  } // while()
} // main()


