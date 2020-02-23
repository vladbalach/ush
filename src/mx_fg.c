#include "ush.h"
#include "macroses.h"

static t_process* get_process(int n, char *str, t_info *info) {
    t_list *tmp = info->processes;

    if (n != -1) {
        while (tmp) {
            if (((t_process*)tmp->data)->index == n)
                return ((t_process*)tmp->data);
            tmp = tmp->next;
        }
        fprintf(stderr, "fg: job not found: %s\n", str);
    }
    else {
        while (tmp) {
            if (mx_strcmp(((t_process*)tmp->data)->name[0], str) == 0)
                return ((t_process*)tmp->data);
            tmp = tmp->next;
        }
        fprintf(stderr, "fg: job not found: %s\n", str);
    }
    return 0;
}

/*return true if all elements - numbers*/
static bool mx_is_number_fg(char *str) {
    int i = -1;
    if (str[0] == '%')
        i++;
    while (str[++i]) {
        if (str[i] < 48 || str[i] > 57)
            return false;
    }
    return true;
}

static int fg_continue(char **argv, t_info *info) {
    t_process *pr = (t_process*)info->processes->data;
    int i = 0;

    if (argv[1] == 0) {
        kill(pr->pid, SIGCONT);
        return 0;
    }
    i = (argv[1][0] == '%') ? 1 : 0;
    if (mx_is_number_fg(argv[1]) && (i == 1)) {
            pr = get_process(atoi(&argv[1][i]), argv[1], info);
    }
    else {
        pr = get_process(-1, &argv[1][i], info);
    }
    if (pr == 0) {
        info->exit_status = 1;
        return 1;
    }
    mx_print_cont(pr->name, pr->index);
    kill(pr->pid, SIGCONT);
    return 0;
}

static void fg_wait(int status, pid_t ch_pr, t_info *info) {
    if (MX_WIFSIG(status)) {
        if (MX_WTERMSIG(status) == SIGSEGV)
            mx_segfault();
        else if (MX_WTERMSIG(status) == SIGINT) {
            mx_del_pid_process(info, ch_pr);
            info->last_status = 130;
        }
        else {
            char **str = mx_get_name(info, ch_pr);
            mx_print_susp(str);
        }
    }
}

void mx_fg(char **argv, t_info *info) {
    pid_t ch_pr = 0;
    int status = 0;

    if (info->processes) {
        if (fg_continue(argv, info) == 0) {
            ch_pr = waitpid(-1, &status, WUNTRACED);
            if (!MX_WIFEXIT(status))
                fg_wait(status, ch_pr, info);
            else {
                mx_del_pid_process(info, ch_pr);
                info->last_status = MX_EXSTATUS(status);
            }
        }
    }
    else
        mx_printerr("fg: no current job\n");
}
