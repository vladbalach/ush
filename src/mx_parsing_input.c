char *mx_parsing_input(char *str) {
    char *temp = mx_strdup(str);
    int end = mx_strlen(temp);
    int position = 0;
    int flag = 0;

    while (position <= end) {
        if (flag != 0)
            flag = end_flag(temp, &position, end, flag);
        if (position < end) {
            if (check_symbol(temp, position, ';')) {
                if (temp[position + 1] == ';') {
                    mx_printerr("u$h: parse error near \';;\'\n");
                    position = end;
                    flag = 1;
                }
                else
                    temp[position] = 0;
            }
            if (check_symbol(temp, position, 34) || check_symbol(temp, position, 39) || check_symbol(temp, position, 96))
                flag = temp[position];
            if (position > 0 && temp[position] == 40 && check_symbol(temp, position - 1, '$'))
                flag = ')';
        }
        position++;
    }
    if (flag != 0)
        mx_strdel(&temp);
    return temp;
}