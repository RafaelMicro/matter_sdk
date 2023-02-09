/** @file util_string.c
 *
 * @license
 * @description
 */


#include "util.h"
#include "util_string.h"
//=============================================================================
//                  Constant Definition
//=============================================================================

//=============================================================================
//                  Macro Definition
//=============================================================================

//=============================================================================
//                  Structure Definition
//=============================================================================

//=============================================================================
//                  Global Data Definition
//=============================================================================

//=============================================================================
//                  Private Function Definition
//=============================================================================
static const char *
_utility_trim_spaces(const char *str)
{
    while (*str == ' ')
    {
        ++str;
    }

    return str;
}

//=============================================================================
//                  Public Function Definition
//=============================================================================
long utility_strtol(const char *str, char **ep)
{
    long v = 0;
    long sign = 1;
    long nr_digits;

    str = _utility_trim_spaces(str);

    if (*str == '-')
    {
        sign = -1;
        ++str;
    }
    else if (*str == '+')
    {
        ++str;
    }

    for (nr_digits = 0; nr_digits < 10; ++nr_digits)
    {
        if ((*str < '0') || (*str > '9'))
        {
            break;
        }
        v = v * 10 + (long)(*str++ - '0');
    }

    if (ep)
    {
        *ep = (char *)str;
    }

    return sign * v;
}

// str to unsigned long
unsigned long utility_strtoul(const char *str, char **ep)
{
    unsigned long v = 0;
    unsigned long neg_v = 0;
    long nr_digits;

    str = _utility_trim_spaces(str);

    if (*str == '-')
    {
        neg_v = 1;
        ++str;
    }
    else if (*str == '+')
    {
        ++str;
    }

    for (nr_digits = 0; nr_digits < 10; ++nr_digits)
    {
        if ((*str < '0') || (*str > '9'))
        {
            break;
        }
        v = v * 10 + (unsigned long)(*str++ - '0');
    }

    if (ep)
    {
        *ep = (char *)str;
    }

    if (neg_v)
    {
        return (~v + 1);
    }

    return v;
}

// str to hex
unsigned long utility_strtox(const char *str, char **ep)
{
    long v = 0;
    long nr_digits;

    str = _utility_trim_spaces(str);

    if (*str == '0')
    {
        ++str;
    }
    if (*str == 'x')
    {
        ++str;
    }

    for (nr_digits = 0; nr_digits < 8; ++nr_digits)
    {
        if (*str < '0')
        {
            break;
        }

        if (*str <= '9')
        {
            v = v * 16 + (long)(*str++ - '0');
        }
        else if ((*str >= 'a') && (*str <= 'f'))
        {
            v = v * 16 + (long)(*str++ - ('a' - 10));
        }
        else if ((*str >= 'A') && (*str <= 'F'))
        {
            v = v * 16 + (long)(*str++ - ('A' - 10));
        }
        else
        {
            break;
        }
    }

    if (ep)
    {
        *ep = (char *)str;
    }

    return v;
}

// long to string
void utility_ltoa(char *a, unsigned long *len, long l, bool w_sign)
{
    char *iter_b = a;
    char *iter_e = a + *len;
    char *iter = iter_e - 1;

    if (iter_b < iter_e)
    {
        if ((long)l < 0)
        {
            l = (long)(-((long)l));
            *iter_b++ = '-';
        }
        else if (w_sign)
        {
            *iter_b++ = '+';
        }
    }

    while (iter >= iter_b)
    {
        *iter-- = (char)('0' + (uint8_t)(l % 10));

        l = l / 10;
        if (l == 0)
        {
            break;    // no more digits to extract
        }
    }

    while (++iter < iter_e)
    {
        *iter_b++ = *iter;
    }

    if (iter_b < iter_e)
    {
        *iter_b = 0x00;
    }
    *len = (unsigned long)(iter_b - a);  // not including null-terminating char
}

// long to string
void utility_ultoa(char *a, unsigned long *len, unsigned long u)
{
    char *iter_b = a;
    char *iter_e = a + *len;
    char *iter = iter_e - 1;

    while (iter >= iter_b)
    {
        *iter-- = (char)('0' + (uint8_t)(u % 10));

        u = u / 10;
        if (u == 0)
        {
            break;    // no more digits to extract
        }
    }

    while (++iter < iter_e)
    {
        *iter_b++ = *iter;
    }

    if (iter_b < iter_e)
    {
        *iter_b = 0x00;
    }
    *len = (unsigned long)(iter_b - a);  // not including null-terminating char
}

void utility_ftoa(char *a, unsigned long *len, double f, unsigned long frc_precision)
{
    char *iter = a;
    char *iter_e = iter + *len;
    double round_val = 0.5;  // round-up addition val
    double frc;
    long fint;

    // rounding up at the least significant digit
    if (frc_precision > 0)
    {
        unsigned long pp = frc_precision;
        double div = 1.0;

        while (pp-- > 0)
        {
            div *= 10.0;
        }
        round_val /= div;
    }

    if (f >= 0)
    {
        f = f + round_val;
    }
    else
    {
        f = f - round_val;
    }

    // make the integer part
    fint = (long)f;
    frc = f - (double)fint;

    if ((f < 0) && (fint == 0))
    {
        // a positive frc value is needed for the following conversion loop
        frc *= -1;

        // if "fint" is "0" the sign of "f" will be lost if "f" is negtive
        if (iter < iter_e)
        {
            *iter++ = '-';
            --(*len);
        }
        utility_ltoa(iter, len, fint, false);
    }
    else
    {
        utility_ltoa(iter, len, fint, false);
    }

    iter += *len;
    if ((iter >= iter_e) || (frc_precision <= 0))
    {
        return;
    }

    *iter++ = '.';

    // handle the fraction part
    while ((iter < iter_e) && (frc_precision-- > 0))
    {
        frc = frc * 10.0f;
        fint = (long)frc;
        frc = frc - (double)fint;

        *iter++ = (char)('0' + fint);
    }

    if (iter < iter_e)
    {
        *iter = 0x00;
    }
    *len = (unsigned long)(iter - a);  // not including null-terminating char
}

// hex to string
void utility_xtoa(char *a, unsigned long *len, unsigned long x, bool capitalized)
{
    char *iter = a;
    char *iter_e = iter + *len;
    int skip_leading_zero = 1;
    long i;

    if (x == 0)
    {
        a[0] = '0';
        a[1] = 0;
        *len = 1;
        return;
    }

    for (i = 8 * sizeof(unsigned long) - 4; ((i >= 0) && (iter < iter_e)); i -= 4)
    {
        uint8_t xx = (uint8_t)(x >> i) & (uint8_t)0x0f;

        if (skip_leading_zero)
        {
            if (xx == 0)
            {
                continue;
            }
            else
            {
                skip_leading_zero = 0;
            }
        }

        if (xx < 10)
        {
            *iter++ = (char)('0' + xx);
        }
        else if (capitalized)
        {
            *iter++ = (char)('A' + (xx - 10));
        }
        else
        {
            *iter++ = (char)('a' + (xx - 10));
        }
    }

    if (iter < iter_e)
    {
        *iter = 0x00;
    }
    *len = (unsigned long)(iter - a);  // not including null-terminating char
}

long utility_strlen(const char *s)
{
    const char  *sc = s;

    if ( !sc )
    {
        return 0;
    }

    while ( *sc != '\0' )
    {
        sc++;
    }

    return sc - s;
}
