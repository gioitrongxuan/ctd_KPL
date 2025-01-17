

/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

Token *currentToken;
Token *lookAhead;

void scan(void) {
  Token* tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType) {
  if (lookAhead->tokenType == tokenType) {
    printToken(lookAhead);
    scan();
  } else missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void) {
  assert("Parsing a Program ....");
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
  assert("Program parsed!");
}

void compileBlock(void) {
  assert("Parsing a Block ....");
  if (lookAhead->tokenType == KW_CONST) {
    eat(KW_CONST);
    compileConstDecl();
    compileConstDecls();
    compileBlock2();
  }
  else compileBlock2();
  assert("Block parsed!");
}

void compileBlock2(void) {
  if (lookAhead->tokenType == KW_TYPE) {
    eat(KW_TYPE);
    compileTypeDecl();
    compileTypeDecls();
    compileBlock3();
  }
  else compileBlock3();
}

void compileBlock3(void) {
  if (lookAhead->tokenType == KW_VAR) {
    eat(KW_VAR);
    compileVarDecl();
    compileVarDecls();
    compileBlock4();
  }
  else compileBlock4();
}

void compileBlock4(void) {
  compileSubDecls();
  compileBlock5();
}

void compileBlock5(void) {
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}

void compileConstDecls(void) {
  // TODO
  if (lookAhead->tokenType==TK_IDENT)
    {
      compileConstDecl();
      compileConstDecls();
    }
//  else return;  OK
}

void compileConstDecl(void) {
  // TODO
  // Bạn nào code giúp với
  eat(TK_IDENT);
  eat(SB_EQ);
  compileConstant();
  eat(SB_SEMICOLON);
}

void compileTypeDecls(void) {
  // TODO
  if (lookAhead->tokenType == TK_IDENT) {
    compileTypeDecl();
    compileTypeDecls();
  }
}

void compileTypeDecl(void) {
  // TODO
  eat(TK_IDENT);
  eat(SB_EQ);
  compileType();
  eat(SB_SEMICOLON);
}

void compileVarDecls(void) {
  // TODO
  if (lookAhead->tokenType == TK_IDENT) {
    compileVarDecl();
    compileVarDecls();
  }
}

void compileVarDecl(void) {
  // TODO
  eat(TK_IDENT);
  eat(SB_COLON);
  compileType();
  eat(SB_SEMICOLON);
}

void compileSubDecls(void) {
  assert("Parsing subtoutines ....");
  // TODO
  if(lookAhead->tokenType == KW_FUNCTION){
    compileFuncDecl();
    compileSubDecls();
  }else if(lookAhead->tokenType == KW_PROCEDURE){
    compileProcDecl();
    compileSubDecls();
  }
  assert("Subtoutines parsed ....");
}

void compileFuncDecl(void) {
  assert("Parsing a function ....");
  // TODO
  eat(KW_FUNCTION);
  eat(TK_IDENT);
  compileParams();
  eat(SB_COLON);
  compileBasicType();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Function parsed ....");
}

void compileProcDecl(void) {
  assert("Parsing a procedure ....");
  // TODO
  eat(KW_PROCEDURE);
  eat(TK_IDENT);
  compileParams();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Procedure parsed ....");
}

void compileUnsignedConstant(void) {
  // TODO
  if (lookAhead->tokenType == TK_NUMBER || lookAhead->tokenType == TK_CHAR) {
    eat(lookAhead->tokenType);
  } else {
    error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileConstant(void) {
  // TODO
  if (lookAhead->tokenType == SB_PLUS || lookAhead->tokenType == SB_MINUS) {
    eat(lookAhead->tokenType);
    compileUnsignedConstant();
  } else {
    compileUnsignedConstant();
  }
}

void compileConstant2(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  default:
    error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileType(void) {
  // TODO
  switch (lookAhead->tokenType) {
    case KW_INTEGER:
      eat(KW_INTEGER);
      break;
    case KW_CHAR:
      eat(KW_CHAR);
      break;
    case KW_BYTES:
      eat(KW_BYTES);
      break;
    case TK_IDENT:
      eat(TK_IDENT);
      break;
    case KW_ARRAY:
      eat(KW_ARRAY);
      eat(SB_LSEL);
      eat(TK_NUMBER);
      eat(SB_RSEL);
      eat(KW_OF);
      compileType();
      break;
    default:
      error(ERR_INVALIDTYPE, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileBasicType(void) {
  // TODO
  if (lookAhead->tokenType == KW_INTEGER
      || lookAhead->tokenType == KW_CHAR
      || lookAhead->tokenType == KW_BYTES) {
    eat(lookAhead->tokenType);
  } else {
    error(ERR_INVALIDBASICTYPE, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileParams(void) {
  // TODO
  if (lookAhead->tokenType == SB_LPAR) {
    eat(SB_LPAR);
    compileParam();
    compileParams2();
    eat(SB_RPAR);
  }
}

void compileParams2(void) {
  // TODO
  if (lookAhead->tokenType == SB_SEMICOLON) {
    eat(SB_SEMICOLON);
    compileParam();
    compileParams2();
  }
}

void compileParam(void) {
  // TODO
  if (lookAhead->tokenType == TK_IDENT) {
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
  } else if (lookAhead->tokenType == KW_VAR) {
    eat(KW_VAR);
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
  } else {
    error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileStatements(void) {
  // TODO
  compileStatement();
  compileStatements2();
}

void compileStatements2(void) {
  // TODO
  switch (lookAhead->tokenType) {
    case SB_SEMICOLON:
      eat(SB_SEMICOLON);
      compileStatement();
      compileStatements2();
      break;
    case KW_END:
    case KW_UNTIL:
      break;
    default:
      eat(SB_SEMICOLON);
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileStatement(void) {
  switch (lookAhead->tokenType) {
  case TK_IDENT:
    compileAssignSt();
    break;
  case KW_CALL:
    compileCallSt();
    break;
  case KW_BEGIN:
    compileGroupSt();
    break;
  case KW_IF:
    compileIfSt();
    break;
  case KW_WHILE:
    compileWhileSt();
    break;
  case KW_FOR:
    compileForSt();
    break;
    // EmptySt needs to check FOLLOW tokens
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  case KW_UNTIL:
    break;
    // Error occurs
  default:
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileAssignSt(void) {
  assert("Parsing an assign statement ....");
  // TODO
  eat(TK_IDENT);
  compileIndexes();
  eat(SB_ASSIGN);
  compileExpression();
  assert("Assign statement parsed ....");
}

void compileCallSt(void) {
  assert("Parsing a call statement ....");
  // TODO
  eat(KW_CALL);
  eat(TK_IDENT);
  compileArguments();
  assert("Call statement parsed ....");
}

void compileGroupSt(void) {
  assert("Parsing a group statement ....");
  // TODO
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
  assert("Group statement parsed ....");
}

void compileIfSt(void) {
  assert("Parsing an if statement ....");
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE)
    compileElseSt();
  assert("If statement parsed ....");
}

void compileElseSt(void) {
  eat(KW_ELSE);
  compileStatement();
}

void compileWhileSt(void) {
  assert("Parsing a while statement ....");
  // TODO
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
  assert("While statement parsed ....");
}

void compileForSt(void) {
  assert("Parsing a for statement ....");
  // TODO
  eat(KW_FOR);
  eat(TK_IDENT);
  eat(SB_ASSIGN);
  compileExpression();
  eat(KW_TO);
  compileExpression();
  eat(KW_DO);
  compileStatement();
  assert("For statement parsed ....");
}

void compileRepeatSt(void) {
  assert("Parsing a repeat statement ....");
  compileStatements();
  eat(KW_UNTIL);
  compileCondition();
  assert("Repeat statement parsed ....");
}

void compileArguments(void) {
  // TODO
  switch (lookAhead->tokenType) {
    case SB_LPAR:
      eat(SB_LPAR);
      compileExpression();
      compileArguments2();
      eat(SB_RPAR);
      break;
    case SB_SEMICOLON:
    case KW_END:
    case KW_ELSE:
    case SB_TIMES:
    case SB_SLASH:
    case KW_TO:
    case KW_DO:
    case SB_COMMA:
    case SB_EQ:
    case SB_NEQ:
    case SB_LE:
    case SB_LT:
    case SB_GE:
    case SB_GT:
    case SB_RPAR:
    case SB_RSEL:
    case KW_THEN:
      break;
    default:
      error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileArguments2(void) {
  // TODO
  switch (lookAhead->tokenType) {
    case SB_COMMA:
      eat(SB_COMMA);
      compileExpression();
      compileArguments2();
      break;
    case SB_RPAR:
      break;
    default:
      error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileCondition(void) {
  // TODO
  compileExpression();
  compileCondition2();
}

void compileCondition2(void) {
  // TODO
  switch (lookAhead->tokenType) {
    case SB_EQ:
    case SB_NEQ:
    case SB_LT:
    case SB_LE:
    case SB_GT:
    case SB_GE:
      eat(lookAhead->tokenType);
      compileExpression();
      break;
    default:
      error(ERR_INVALIDCOMPARATOR, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileExpression(void) {
  assert("Parsing an expression");
  // TODO
  if (lookAhead->tokenType == SB_PLUS || lookAhead->tokenType == SB_MINUS) {
    eat(lookAhead->tokenType);
  }
  compileExpression2();
  assert("Expression parsed");
}

void compileExpression2(void) {
  // TODO
  compileTerm();
  compileExpression3();
}


void compileExpression3(void) {
  // TODO
  if (lookAhead->tokenType == SB_PLUS || lookAhead->tokenType == SB_MINUS) {
    eat(lookAhead->tokenType);
    compileTerm();
    compileExpression3();
  }
}

void compileTerm(void) {
  // TODO
  compileFactor();
  compileTerm2();
}

void compileTerm2(void) {
  // TODO
  if (lookAhead->tokenType == SB_TIMES
      || lookAhead->tokenType == SB_SLASH
      || lookAhead->tokenType == SB_POWER) {
    eat(lookAhead->tokenType);
    compileFactor();
    compileTerm2();
  }
}

void compileFactor(void) {
  switch (lookAhead->tokenType) {
    case TK_NUMBER:
    case TK_CHAR:
      eat(lookAhead->tokenType);
      break;
    case TK_IDENT:
      eat(TK_IDENT);
      switch(lookAhead->tokenType) {
        case SB_LSEL:
        case SB_LPAR:
          compileIndexes();
          break;
        default:
          break;
      }
      break;
    case SB_LPAR:
      eat(SB_LPAR);
      compileExpression();
      eat(SB_RPAR);
      break;
    default:
      error(ERR_INVALIDFACTOR, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileIndexes(void) {
    // Kiểm tra dấu ngoặc vuông hoặc tròn mở
    if (lookAhead->tokenType == SB_LSEL || lookAhead->tokenType == SB_LPAR) {
        if (lookAhead->tokenType == SB_LSEL) {
            eat(SB_LSEL);  // Ăn dấu ngoặc vuông mở '['
        } else if (lookAhead->tokenType == SB_LPAR) {
            eat(SB_LPAR);  // Ăn dấu ngoặc tròn mở '('
        }

        // Xử lý biểu thức bên trong ngoặc
        compileExpression();

        // Kiểm tra và xử lý dấu ngoặc đóng phù hợp
        if (lookAhead->tokenType == SB_RSEL) {
            eat(SB_RSEL);  // Ăn dấu ngoặc vuông đóng ']'
        } else if (lookAhead->tokenType == SB_RPAR) {
            eat(SB_RPAR);  // Ăn dấu ngoặc tròn đóng ')'
        }

        // Kiểm tra nếu có mảng lồng nhau (các dấu ngoặc lồng nhau)
        if (lookAhead->tokenType == SB_LSEL || lookAhead->tokenType == SB_LPAR) {
            compileIndexes();  // Tiếp tục xử lý nếu có dấu ngoặc lồng nhau
        }
    }

    // Trường hợp đặc biệt xử lý A(.I] như yêu cầu
    else if (lookAhead->tokenType == SB_LPAR) {
        // Nếu gặp dấu ngoặc tròn mở '(' và token sau đó là dấu '.'
        eat(SB_LPAR);  // Ăn dấu ngoặc tròn mở '('

        // Kiểm tra dấu '.' sau dấu ngoặc tròn mở
        if (lookAhead->tokenType == CHAR_PERIOD) {
            eat(SB_DOT);  // Ăn dấu '.'
            compileExpression();  // Xử lý trường hoặc phương thức sau dấu '.'

            // Kiểm tra dấu ngoặc vuông đóng ']'
            if (lookAhead->tokenType == SB_RSEL) {
                eat(SB_RSEL);  // Ăn dấu ngoặc vuông đóng ']'
            }
        } else {
            // Xử lý trường hợp không có dấu '.' sau '('
            compileExpression();  // Xử lý biểu thức trong ngoặc
            if (lookAhead->tokenType == SB_RPAR) {
                eat(SB_RPAR);  // Ăn dấu ngoặc tròn đóng ')'
            }
        }
    }
}


int compile(char *fileName) {
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  currentToken = NULL;
  lookAhead = getValidToken();

  compileProgram();

  free(currentToken);
  free(lookAhead);
  closeInputStream();
  return IO_SUCCESS;

}