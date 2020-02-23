#include "ush.h"

void mx_charge_parametr_export(char *value, char *tmp, t_variable *data) {
    if (value != 0) {
        free(data->value);
        data->value = value;
        free(data->mem);
        data->mem = mx_strjoin(tmp, value);
    }
    if (data->mem == NULL)
        data->mem = mx_strjoin(tmp, data->value);
    data->is_env = true;
    putenv(data->mem);
}
