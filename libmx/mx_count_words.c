int mx_count_words(const char *str, char c) {
    int a = 0;

    if (str == 0)
        return - 1;
    for (int i = 0; str[i] != '\0'; i++) {
        while (str[i] == c && str[i + 1] != '\0')
            i++;
        if (str[i] != c && str[i] != '\0') {
            while (str[i] != c && str[i + 1] != '\0')
                i++;
            a++;
        }
    }
    return a;
}
