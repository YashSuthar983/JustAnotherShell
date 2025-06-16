#pragma once
#include <string>

enum TokenType {
    WORD,
    NUMBER,
    ASSIGNMENT_WORD,
    TOKEN_NAME,
    TOKEN_NEWLINE,
    IO_NUMBER,

    // Operators
    TOKEN_AND_IF, TOKEN_OR_IF, TOKEN_DSEMI,
    TOKEN_DLESS, TOKEN_DGREAT, TOKEN_LESSAND, TOKEN_GREATAND,
    TOKEN_LESSGREAT, TOKEN_DLESSDASH, TOKEN_CLOBBER,

    // Reserved Words
    TOKEN_If, TOKEN_Then, TOKEN_Else, TOKEN_Elif, TOKEN_Fi,
    TOKEN_Do, TOKEN_Done, TOKEN_Case, TOKEN_Esac, TOKEN_While,
    TOKEN_Until, TOKEN_For, TOKEN_Lbrace, TOKEN_Rbrace, TOKEN_Bang,
    TOKEN_In,

    // Single character operators/delimiters
    TOKEN_PIPE,     // '|'
    TOKEN_AMPERSAND,// '&'
    TOKEN_SEMICOLON,// ';'
    LESS_THAN,     // '<'
    GREATER_THAN,  // '>'
    TOKEN_LPAREN,   // '('
    TOKEN_RPAREN,   // ')'

    TOKEN_EOF
};

std::string tokenToString(TokenType type);

struct Token {
    TokenType type;
    std::string value;
    std::string getType(){return tokenToString(type);}
};

inline std::string tokenToString(TokenType type) {
    switch (type) {
        case WORD: return "WORD";
        case NUMBER: return "NUMBER";
        case ASSIGNMENT_WORD: return "ASSIGNMENT_WORD";
        case TOKEN_NAME: return "TOKEN_NAME";
        case TOKEN_NEWLINE: return "NEWLINE";
        case IO_NUMBER: return "IO_NUMBER";
        case TOKEN_AND_IF: return "AND_IF";
        case TOKEN_OR_IF: return "OR_IF";
        case TOKEN_DSEMI: return "DSEMI";
        case TOKEN_DLESS: return "DLESS";
        case TOKEN_DGREAT: return "DGREAT";
        case TOKEN_LESSAND: return "LESSAND";
        case TOKEN_GREATAND: return "GREATAND";
        case TOKEN_LESSGREAT: return "LESSGREAT";
        case TOKEN_DLESSDASH: return "DLESSDASH";
        case TOKEN_CLOBBER: return "CLOBBER";
        case TOKEN_If: return "If";
        case TOKEN_Then: return "Then";
        case TOKEN_Else: return "Else";
        case TOKEN_Elif: return "Elif";
        case TOKEN_Fi: return "Fi";
        case TOKEN_Do: return "Do";
        case TOKEN_Done: return "Done";
        case TOKEN_Case: return "Case";
        case TOKEN_Esac: return "Esac";
        case TOKEN_While: return "While";
        case TOKEN_Until: return "Until";
        case TOKEN_For: return "For";
        case TOKEN_Lbrace: return "Lbrace";
        case TOKEN_Rbrace: return "Rbrace";
        case TOKEN_Bang: return "Bang";
        case TOKEN_In: return "In";
        case TOKEN_PIPE: return "PIPE";
        case TOKEN_AMPERSAND: return "AMPERSAND";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case LESS_THAN: return "LESS_THAN";
        case GREATER_THAN: return "GREATER_THAN";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_EOF: return "EOF_TOKEN";
        default: return "UNKNOWN";
    }
}