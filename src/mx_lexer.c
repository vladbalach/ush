#include "ush.h"

// static bool is_empty_str(char *str, int len) {
//     if (len == 0) 
//         return true;
//     for(int i = 0; i < len; i++) {
//         if ((str[i] != ';') && (str[i] != ' ')) {
//             return false;
//         }
//     }
//     return true;
// }



// static char* checkEndOfLine(char *str, int i, int *startI) {
//     while(str[i]) {
//         if ((str[i] != ' ') && (str[i] != ';')) {
//             *startI = i;
//             return mx_strdup(";"); // if finded some other word
//         }
//         i++;
//     }
//     return 0; // if end of str
// }




// char* copyFromTo(char *str, int *start, int end) {
//     if (end - *start >= 200) {
//         errno = 666;
//         return 0;
//     }
//     char *newStr = mx_strnew(end - *start);
//     for(int i = *start; i < end; i++) {
//         newStr[i - *start] = str[i];
//     }
//     *start += end - *start;
//     return newStr;
// }

// char *getNextTokenStr(char *str, int *startI, int endI) {
//     int i = *startI;
//     int startToken = *startI;
//     int endToken = endI;
//     char *tokenStr = 0;
    
//     if ((str[i] == 0) || (*startI >= endI)) {
//         return 0;
//     }
//     skipSpaces(str, startI, endI);
   
//     i = *startI;
//     if (str[i] == ';') {
//         i++;
//         return checkEndOfLine(str, i, startI);
//     }
//     else if (str[i] == '|') {
//         if (str[i + 1] == '|') {
//             *startI += 2;
//             return mx_strdup("||");
//         }
//         *startI += 1;
//         return mx_strdup("|");
//     }
//     else if (str[i] == '<') {
//         if (str[i + 1] == '<') {
//             *startI += 2;
//             return mx_strdup("<<");
//         }
//         *startI += 1;
//         return mx_strdup("<");
//     }
//     else if (str[i] == '>') {
//         if (str[i + 1] == '>') {
//             *startI += 2;
//             return mx_strdup(">>");
//         }
//         *startI += 1;
//         return mx_strdup(">");
//     }
//     else if ((str[i] == '&') && (str[i + 1] == '&')) {
//         *startI += 2;
//         return mx_strdup("&&");
//     }
//     else {
//         startToken = i;
//         while (isChar(str[i])) {
//             i++;
//         }
        
//         // endToken = i;
//         return copyFromTo(str, startI, i);
//         // skipSpaces(str, startI, endI);
//     }
    
//     return 0;
// }



// t_list *mx_getMasOfValues(char *str, int *startI, int endI) {
//     t_list *head = 0;
//     char *value = getNextTokenStr(str, startI, endI);
//     char type = mx_getTokenType(value);
    
//     if (value == 0)
//         return 0;
//     while (value)
//     {
//         mx_push_back(&head, value);
//         value = getNextTokenStr(str, startI, endI);
//     }
//     return head;
// }

// static void concatenateList(t_list *strs) {
//     t_list *tmp = strs;
//     t_list *buff = 0;

//     while (tmp) {
//         if ((mx_getTokenType(tmp->data) == TYPE_COMMAND) 
//             && (tmp->next) 
//             && (mx_getTokenType(tmp->next->data) == TYPE_COMMAND))
//         {
//             tmp->data = mx_strjoin2(tmp->data, tmp->next->data);
//             free(tmp->next->data);
//             buff = tmp->next;
//             tmp->next = tmp->next->next;
//             free(buff);
//         }
//         else
//             tmp = tmp->next;
//     }
//     tmp = strs;
// }

// t_list* mx_getTokenValues(char *str) {
//     int start = get_start_index(str);
//     int end = get_end_index(str);
//     if (start >= end) // string empty 
//         return 0;
//     t_list *strs = mx_getMasOfValues(str, &start, end);
//     concatenateList(strs);
//     t_list *tmp = strs;
//     while (tmp) {
//         tmp = tmp->next;
//     }
//     return strs;
// }

// t_list* mx_createTokens(t_list **headStrs) {
//     t_list* tmp = *headStrs;
//     t_list* tokens = 0;
//     t_token *newToken = 0;
//     char type = 0;

//     int i = 0;
//     while (tmp) {
//         type = mx_getTokenType(tmp->data);
//         newToken = mx_create_token(type, mx_strdup(tmp->data), 0);
//         mx_push_back(&tokens, newToken);
//         mx_pop_front(headStrs);
//         tmp = *headStrs;
//         // tmp = tmp->next;
//     }
//     return tokens;
// }

static int get_start_index(char *str) {
    int i = 0;
    while(str[i]) {
        if (str[i] != ' ') {
            return i;
        }
        i++;
    }
    return i;
}

static int get_end_index(char *str) {
    int i = mx_strlen(str) - 1;

    while(i >= 0) {
        if (str[i] != ' ') {
            return i + 1;
        }
        i--;
    }
    return i + 1;
}

t_list *mx_create_tokens(char *str) {
    int start   = get_start_index(str);
    int end     = get_end_index(str);
    t_token *newToken = 0;
    t_list *tokens = 0;

    while((newToken = mx_get_next_token(&start, end, str))) {
        mx_push_back(&tokens, newToken);
    }
    return tokens;
}

t_list *mx_lexer(char *str) {
    t_list *tokens = 0;
    if (str == 0)
        return 0;
    tokens = mx_create_tokens(str);
    
    return tokens;
}
