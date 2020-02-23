#include "libmx.h"

static void write_str_to_strstr(char ***s1, const char *s, char c, int b) {
    int a = 0;
    int d = 0;
    int i = 0;
    char **s2 = *s1; 
    
    while (a < b) {
        while (mx_get_char_index(&s[i], c) == 0 && s[i] != '\0')
            i++;
        d = mx_get_char_index(&s[i], c);
        if (d > 0) {
            s2[a++] = mx_strndup(&s[i], d);
            i += d;
        }
        else {
            s2[a++] = mx_strndup(&s[i], mx_strlen(&s[i])); 
            i += d;
        }
    }
    s2[b] = 0;
    *s1 = s2;
}

char **mx_strsplit(const char *s, char c) {
    int b = 0;
    char **s1 = NULL;

    if (s == 0)
        return 0;
    b = mx_count_words(s, c);
    s1 = (char **)malloc((b + 1) * sizeof(char *));
    if (s1 != 0) { 
        write_str_to_strstr(&s1, s, c, b);
    }
    return s1;
}
