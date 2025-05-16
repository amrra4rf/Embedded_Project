#include "My built-in Functions.h"
////////////////////////////////////////////////////////////////////////
// mustrtofloat
float my_strtof(const char *str)
{
    float result = 0.0f;
    float frac = 0.0f;
    int sign = 1;
    int i = 0;
    int frac_div = 1;
    int after_decimal = 0;

    // Skip whitespace
    while (str[i] == ' ' || str[i] == '\t')
        i++;

    // Handle sign
    if (str[i] == '-')
    {
        sign = -1;
        i++;
    }
    else if (str[i] == '+')
    {
        i++;
    }

    // Process digits
    while ((str[i] >= '0' && str[i] <= '9') || str[i] == '.')
    {
        if (str[i] == '.')
        {
            after_decimal = 1;
            i++;
            continue;
        }

        int digit = str[i] - '0';
        if (!after_decimal)
        {
            result = result * 10.0f + digit;
        }
        else
        {
            frac = frac * 10.0f + digit;
            frac_div *= 10;
        }
        i++;
    }

    result += frac / frac_div;
    return result * sign;
}
////////////////////////////////////////////////////////////////////////
void float_to_string(float num, char *str, int precision)
{
    if (num < 0)
    {
        *str++ = '-';
        num = -num;
    }

    int int_part = (int)num;
    float frac_part = num - int_part;

    // Convert integer part to string
    char temp[10];
    int i = 0;
    if (int_part == 0)
    {
        temp[i++] = '0';
    }
    else
    {
        while (int_part > 0)
        {
            temp[i++] = '0' + (int_part % 10);
            int_part /= 10;
        }
    }

    // Reverse and copy
    while (i > 0)
    {
        *str++ = temp[--i];
    }

    *str++ = '.'; // Decimal point

    // Convert fractional part
    for (int j = 0; j < precision; j++)
    {
        frac_part *= 10;
        int digit = (int)frac_part;
        *str++ = '0' + digit;
        frac_part -= digit;
    }

    *str = '\0'; // Null-terminate
}
////////////////////////////////////////////////////////////////////////
