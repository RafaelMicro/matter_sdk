
/** @file util_printf.c
 *
 * @license
 * @description
 */

#include "cm3_mcu.h"
#include "util.h"
#include "util_string.h"
#include "util_printf.h"
//=============================================================================
//                  Constant Definition
//=============================================================================
#define CONFIG_PRINTF_FMTB_SIZE     32
//=============================================================================
//                  Macro Definition
//=============================================================================

//=============================================================================
//                  Structure Definition
//=============================================================================

//=============================================================================
//                  Global Data Definition
//=============================================================================
static cb_putchar   g_pf_putc = 0;
static cb_puts      g_pf_puts = 0;

//=============================================================================
//                  Private Function Definition
//=============================================================================
static int
_fputc(int ch, char *pBuf, int *pCurlen, int maxlen)
{
    int rval = 0;
    do
    {
        if ( pBuf )
        {
            if ( *pCurlen < maxlen )
            {
                pBuf[(*pCurlen)] = (char)(ch & 0xFF);
                (*pCurlen)++;
            }

        }
        else
        {
            if ( g_pf_putc ) // STDOUT
            {
                g_pf_putc(ch);
            }
        }

    } while (0);
    return rval;
}

static int
_fputs(const char *str, uint32_t length, char *pBuf, int *pCurlen, int maxlen)
{
    int     rval = 0;
    do
    {
        if ( pBuf )
        {
            while ( length-- && *pCurlen < maxlen )
            {
                pBuf[(*pCurlen)] = (char)((*str++) & 0xFF);
                (*pCurlen)++;
            }
        }
        else
        {
            if ( g_pf_puts ) // STDOUT
            {
                g_pf_puts((char *)str, length);
            }
        }
    } while (0);
    return rval;
}
//=============================================================================
//                  Public Function Definition
//=============================================================================
void utility_vsprintf(char *pBuf, int maxlen, const char *fmt, va_list args)
{
    char arg_zstr[CONFIG_PRINTF_FMTB_SIZE];
    int  curlen = 0;

    while (*fmt != 0x00)
    {
        if (*fmt != '%')
        {
            _fputc((char)*fmt++, pBuf, &curlen, maxlen);
        }
        else
        {
            bool flag_sign_always = false;      // insert a plus or minus sign (+ or -) even for positive numbers
            bool flag_sign_space = false;       // insert a sapce if no sign is going to be written
            bool flag_fmtfix = false;           // for o, x or X specifiers the value is preceeded with 0, 0x or 0X
            // for a, A, e, E, f, F, g or G it forces appending a decimal point
            bool flag_left_justify = false;     // left-justify within the given field width "flag_field_width"
            bool flag_left_zpad = false;        // left-pading with 0 instead of spaces
            bool flag_eos = false;              // end of specifier
            uint8_t flag_arg_len = 0;           // length specifier (data type) [0]normal [1]short, [2]long/wide, [4]long double

            int flag_field_precision = -1;     // precision
            int flag_field_width = -1;         // field width

            if (*++fmt == 0x00)
            {
                break;
            }

            if (*fmt == '%')
            {
                _fputc((char)*fmt++, pBuf, &curlen, maxlen);
                continue;
            }

            // decode and dispatch tasks according format specifier ...
            while ((*fmt != 0x00) && (!flag_eos))
            {
                switch (*fmt)
                {
                /* format flags */
                case '+':
                    flag_sign_always = true;
                    ++fmt;
                    break;
                case '-':
                    flag_left_justify = true;
                    ++fmt;
                    break;
                case ' ':
                    flag_sign_space = true;
                    ++fmt;
                    break;
                case '0':
                    flag_left_zpad = true;
                    ++fmt;
                    break;
                case '#':
                    flag_fmtfix = true;
                    ++fmt;
                    break;

                /* format precision */
                case '.':
                    ++fmt;
                    if (*fmt == '*')
                    {
                        // size of parameters in stack depends on AVR ABI (16/32 bits) ...
                        flag_fmtfix = (long)va_arg(args, int);
                        ++fmt;
                    }
                    else if (*fmt != 0x00)
                    {
                        char *ep;
                        flag_field_precision = utility_strtol(fmt, &ep);
                        fmt = ep;
                    }
                    break;

                /* width */
                case '*':
                    // size of parameters in stack depends on AVR ABI (16/32 bits) ...
                    flag_field_width = (long)va_arg(args, int);
                    ++fmt;
                    break;
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    char *ep;
                    flag_field_width = utility_strtol(fmt, &ep);
                    fmt = ep;
                }
                break;

                /* length */
                case 'h':
                    flag_arg_len |= 1;  // short (d i u o x X)
                    ++fmt;
                    break;
                case 'l':
                    flag_arg_len |= 2;  // long (d i u o x X)
                    ++fmt;
                    break;
                case 'L':
                    flag_arg_len |= 4;  // long double (f F e E g G a A)
                    ++fmt;
                    break;

                // format specifier
                case 'c':
                {
                    long i;

                    // size of parameters in stack depends on AVR ABI (16/32 bits) ...
                    arg_zstr[0] = (char)va_arg(args, int);  // the c specifier takes an long (or wint_t) as argument

                    if (!flag_left_justify)
                    {
                        for (i = 0; i < (flag_field_width - 1); ++i)
                        {
                            _fputc((char)' ', pBuf, &curlen, maxlen);
                        }
                    }

                    _fputc((char)*arg_zstr, pBuf, &curlen, maxlen);

                    if (flag_left_justify)
                    {
                        for (i = 0; i < (flag_field_width - 1); ++i)
                        {
                            _fputc((char)' ', pBuf, &curlen, maxlen);
                        }
                    }

                    flag_eos = true;
                    ++fmt;
                    break;
                }
                case 's':
                {
                    long i;

                    // size of parameters in stack depends on AVR ABI (16/32 bits) ...
                    char *zstr = va_arg(args, char *);

                    long zstr_len = (zstr) ? (long)utility_strlen(zstr) : 0;

                    if (!flag_left_justify)
                    {
                        for (i = 0; i < (flag_field_width - zstr_len); ++i)
                        {
                            _fputc((char)' ', pBuf, &curlen, maxlen);
                        }
                    }

                    _fputs((const char *)zstr, (unsigned long)zstr_len, pBuf, &curlen, maxlen);

                    if (flag_left_justify)
                    {
                        for (i = 0; i < (flag_field_width - zstr_len); ++i)
                        {
                            _fputc((char)' ', pBuf, &curlen, maxlen);
                        }
                    }

                    flag_eos = true;
                    ++fmt;
                }
                break;
                case 'p':
                {
                    unsigned long arg;
                    unsigned long zstr_len = CONFIG_PRINTF_FMTB_SIZE;

                    // size of parameters in stack depends on AVR ABI (16/32 bits) ...
                    arg = (unsigned long)va_arg(args, void *);

                    utility_xtoa(arg_zstr, &zstr_len, arg, true);

                    _fputs((const char *)arg_zstr, zstr_len, pBuf, &curlen, maxlen);

                    flag_eos = true;
                    ++fmt;
                }
                break;
                case 'x':
                case 'X':
                {
                    long i;
                    unsigned long arg;
                    long zstr_len = CONFIG_PRINTF_FMTB_SIZE, prt_len = 0, nr_zpad = 0;
                    char *s_iter = &arg_zstr[0];

                    if (flag_arg_len & 2)
                    {
                        arg = va_arg(args, unsigned long);
                    }
                    else
                    {
                        arg = va_arg(args, unsigned int);
                    }

                    // a precision of 0 means that no character is written for the value 0.
                    if ((flag_field_precision == 0) && (arg == 0))
                    {
                        flag_eos = true;
                        ++fmt;
                        break;
                    }

                    if (*fmt == 'x')
                    {
                        utility_xtoa(arg_zstr, (unsigned long *)&zstr_len, arg, false);
                    }
                    else
                    {
                        utility_xtoa(arg_zstr, (unsigned long *)&zstr_len, arg, true);
                    }

                    if (zstr_len <= 0)
                    {
                        flag_eos = true;
                        ++fmt;
                        break;
                    }

                    if (flag_fmtfix)
                    {
                        prt_len = 2;
                    }

                    if (flag_field_precision > 0)
                    {
                        // for integer specifiers (d, i, o, u, x, X):
                        // precision specifies the minimum number of digits to be written.
                        if (zstr_len < flag_field_precision)
                        {
                            nr_zpad = flag_field_precision - zstr_len;
                            prt_len += flag_field_precision;
                        }
                        else
                        {
                            prt_len += zstr_len;
                        }

                        flag_left_zpad = false;
                    }
                    else
                    {
                        prt_len += zstr_len;

                        if (flag_left_zpad && (prt_len < flag_field_width))
                        {
                            nr_zpad = flag_field_width - prt_len;
                        }
                    }

                    if (!flag_left_zpad && !flag_left_justify)
                    {
                        for (i = 0; i < (flag_field_width - prt_len); ++i)
                        {
                            _fputc((char)' ', pBuf, &curlen, maxlen);
                        }
                    }

                    if (flag_fmtfix)
                    {
                        _fputs((const char *)"0x", 2, pBuf, &curlen, maxlen);
                    }

                    for (i = 0; i < nr_zpad; ++i)
                    {
                        _fputc((char)'0', pBuf, &curlen, maxlen);
                    }

                    _fputs((const char *)s_iter, zstr_len, pBuf, &curlen, maxlen);

                    if (!flag_left_zpad && flag_left_justify)
                    {
                        for (i = 0; i < (flag_field_width - prt_len); ++i)
                        {
                            _fputc((char)' ', pBuf, &curlen, maxlen);
                        }
                    }

                    flag_eos = true;
                    ++fmt;
                }
                break;
                case 'd':
                case 'i':
                case 'u':
                {
                    unsigned long arg_u;
                    long arg, i;
                    char *zstr = (char *)&arg_zstr[0];
                    long zstr_len = CONFIG_PRINTF_FMTB_SIZE, prt_len = 0, nr_zpad = 0;

                    if (*fmt == 'u')
                    {
                        //if (flag_arg_len & 1)
                        //  arg = va_arg(args, unsigned short);
                        //else
                        if (flag_arg_len & 2)
                        {
                            arg_u = va_arg(args, unsigned long);
                        }
                        else
                        {
                            arg_u = va_arg(args, unsigned int);
                        }

                        // a precision of 0 means that no character is written for the value 0.
                        if ((flag_field_precision == 0) && (arg_u == 0))
                        {
                            flag_eos = true;
                            ++fmt;
                            break;
                        }

                        utility_ultoa(zstr, (unsigned long *)&zstr_len, arg_u);

                    }
                    else
                    {
                        //if (flag_arg_len & 1)
                        //  arg = va_arg(args, unsigned short);
                        //else
                        if (flag_arg_len & 2)
                        {
                            arg = va_arg(args, long);
                        }
                        else
                        {
                            arg = va_arg(args, int);
                        }

                        // a precision of 0 means that no character is written for the value 0.
                        if ((flag_field_precision == 0) && (arg == 0))
                        {
                            flag_eos = true;
                            ++fmt;
                            break;
                        }

                        utility_ltoa(zstr, (unsigned long *)&zstr_len, (long)arg, false);
                    }

                    if (zstr_len <= 0)
                    {
                        flag_eos = true;
                        ++fmt;
                        break;
                    }

                    if (flag_field_precision > 0)
                    {
                        if (zstr_len < flag_field_precision)
                        {
                            nr_zpad = flag_field_precision - zstr_len;
                            prt_len += flag_field_precision;
                        }
                        else
                        {
                            prt_len += zstr_len;
                        }

                        flag_left_zpad = 0;
                    }
                    else
                    {
                        prt_len += zstr_len;

                        if (flag_left_zpad && (prt_len < flag_field_width))
                        {
                            nr_zpad = flag_field_width - prt_len;
                        }
                    }

                    if (!flag_left_zpad && !flag_left_justify)
                    {
                        for (i = 0; i < (flag_field_width - prt_len); ++i)
                        {
                            _fputc((char)' ', pBuf, &curlen, maxlen);
                        }
                    }

                    if (flag_sign_always)
                    {
                        if (*zstr >= '0')
                        {
                            _fputc((char)'+', pBuf, &curlen, maxlen);
                        }
                        else
                        {
                            _fputc(*zstr, pBuf, &curlen, maxlen);
                            --zstr_len;
                            ++zstr;
                        }
                        ++prt_len;
                    }
                    else if (flag_sign_space)
                    {
                        if (*zstr >= '0')
                        {
                            _fputc((char)' ', pBuf, &curlen, maxlen);
                        }
                        else
                        {
                            _fputc(*zstr, pBuf, &curlen, maxlen);
                            --zstr_len;
                            ++zstr;
                        }
                        ++prt_len;
                    }

                    for (i = 0; i < nr_zpad; ++i)
                    {
                        _fputc((char)'0', pBuf, &curlen, maxlen);
                    }

                    _fputs((const char *)zstr, (unsigned long)zstr_len, pBuf, &curlen, maxlen);

                    if (!flag_left_zpad && flag_left_justify)
                    {
                        for (i = 0; i < (flag_field_width - prt_len); ++i)
                        {
                            _fputc((char)' ', pBuf, &curlen, maxlen);
                        }
                    }

                    flag_eos = true;
                    ++fmt;
                }
                break;

                case 'g':
                case 'G':
                // todo
                case 'e':
                case 'E':
                // todo
                case 'f':
                {
                    long i;
                    double arg;
                    char *zstr = (char *)&arg_zstr[0];
                    long zstr_len = CONFIG_PRINTF_FMTB_SIZE, prt_len = 0, nr_zpad = 0;

                    //if (flag_arg_len & 4)
                    //  arg = (float)va_arg(args, double);  // ms library always pass float as double type
                    //else if (flag_zstr_len & 2)
                    //  arg = va_arg(args, unsigned long int);
                    //else
                    arg = va_arg(args, double);

                    // a precision of 0 means that no character is written for the value 0.
                    if ((flag_field_precision == 0) && (arg == 0))
                    {
                        flag_eos = true;
                        ++fmt;
                        break;
                    }

                    if (flag_field_precision > 0)
                    {
                        utility_ftoa(zstr, (unsigned long *)&zstr_len, arg, (unsigned long)flag_field_precision);
                    }
                    else
                    {
                        utility_ftoa(zstr, (unsigned long *)&zstr_len, arg, 3);
                    }

                    if (zstr_len <= 0)
                    {
                        flag_eos = true;
                        ++fmt;
                        break;
                    }

                    if (flag_field_precision > 0)
                    {
                        if (zstr_len < flag_field_precision)
                        {
                            prt_len += flag_field_precision;
                        }
                        else
                        {
                            prt_len += zstr_len;
                        }

                        if (zstr_len < flag_field_precision)
                        {
                            nr_zpad = flag_field_precision - zstr_len;
                        }

                        flag_left_zpad = 0;
                    }
                    else
                    {
                        prt_len += zstr_len;

                        if ((flag_left_zpad != 0) && (prt_len < flag_field_width))
                        {
                            nr_zpad = flag_field_width - prt_len;
                        }
                    }

                    if (!flag_left_zpad && !flag_left_justify)
                    {
                        for (i = 0; i < (flag_field_width - prt_len); ++i)
                        {
                            _fputc((char)' ', pBuf, &curlen, maxlen);
                        }
                    }

                    if (flag_sign_always)
                    {
                        if (*zstr >= 0x30)
                        {
                            _fputc((char)'+', pBuf, &curlen, maxlen);
                        }
                        else
                        {
                            _fputc(*zstr, pBuf, &curlen, maxlen);
                            --zstr_len;
                            ++zstr;
                        }
                        ++prt_len;
                    }
                    else if (flag_sign_space)
                    {
                        if (*zstr >= 0x30)
                        {
                            _fputc((char)' ', pBuf, &curlen, maxlen);
                        }
                        else
                        {
                            _fputc(*zstr, pBuf, &curlen, maxlen);
                            --zstr_len;
                            ++zstr;
                        }
                        ++prt_len;
                    }

                    for (i = 0; i < nr_zpad; ++i)
                    {
                        _fputc((char)'0', pBuf, &curlen, maxlen);
                    }

                    _fputs((const char *)zstr, zstr_len, pBuf, &curlen, maxlen);

                    if (!flag_left_zpad && flag_left_justify)
                    {
                        for (i = 0; i < (flag_field_width - prt_len); ++i)
                        {
                            _fputc((char)' ', pBuf, &curlen, maxlen);
                        }
                    }

                    flag_eos = true;
                    ++fmt;

                }
                break;
                case 'n':
                {
                    // size of parameters in stack depends on AVR ABI (16/32 bits) ...
                    va_arg(args, unsigned long *);
                    //unsigned long *ptr = va_arg(args, long *);
                    //*ptr = r;  // todo

                    flag_eos = true;
                    ++fmt;
                }
                break;
                case 'o':
                    // todo
                    arg_zstr[0] = (char)va_arg(args, unsigned int);
                    flag_eos = true;
                    ++fmt;
                    break;
                default:
                    if ( maxlen > 0 )
                    {
                        if ( curlen < (maxlen - 1) )
                        {
                            pBuf[curlen] = '\0';
                        }
                        else if ( maxlen > 0 )
                        {
                            pBuf[maxlen - 1] = '\0';
                        }
                    }
                    return;
                }
            }
        }
    }

    if ( maxlen > 0 )
    {
        if ( curlen < (maxlen - 1) )
        {
            pBuf[curlen] = '\0';
        }
        else if ( maxlen > 0 )
        {
            pBuf[maxlen - 1] = '\0';
        }
    }
    return;
}

int utility_snprintf(char *pbuf, int buf_size, const char *fmt, ...)
{
    va_list     va;

    va_start(va, fmt);
    utility_vsprintf(pbuf, buf_size, fmt, va);

    va_end(va);
    return 0;
}

void utility_printf(const char *fmt, ...)
{
    va_list     va;

    va_start(va, fmt);
    utility_vsprintf(0, 0, fmt, va);
    va_end(va);
    return;
}

int utility_puts(char *str)
{
    int     rval = -1;

    if ( g_pf_puts )
    {
        rval = g_pf_puts((char *)str, utility_strlen(str));    // STDOUT
    }

    return rval;
}

void
utility_register_stdout(
    cb_putchar  pf_putc,
    cb_puts     pf_puts)
{
    g_pf_putc = pf_putc;
    g_pf_puts = pf_puts;
    return;
}

