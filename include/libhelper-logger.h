//===----------------------------------------------------------------------===//
//
//                         The Libhelper Project
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  Copyright (C) 2021, Is This On? Holdings Limited
//  
//  Harry Moulton <me@h3adsh0tzz.com>
//
//===----------------------------------------------------------------------===//

/* libhelper-core: logger */

#ifndef __LIBHELPER_CORE_LOGGER_H__
#define __LIBHELPER_CORE_LOGGER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdarg.h>

/**
 *  \brief      Log Levels
 */
typedef enum {
    LOG_ERROR,
    LOG_WARNING,
    LOG_DEBUG,
    LOG_CI,
    LOG_PRINT,

    LOG_TEST_FAIL,
    LOG_TEST_SUCCESS
} log_type;

/**
 *  ANSI colour definitions.
 */
#define ANSI_COLOR_WHITE            "\x1b[38;5;254m"
#define ANSI_COLOR_DARK_WHITE       "\x1b[38;5;251m"
#define ANSI_COLOR_DARK_GREY        "\x1b[38;5;243m"
#define ANSI_COLOR_YELLOW           "\x1b[38;5;214m"
#define ANSI_COLOR_DARK_YELLOW      "\x1b[38;5;94m"
#define ANSI_COLOR_RED              "\x1b[38;5;88m"
#define ANSI_COLOR_BLUE             "\x1b[38;5;32m"
#define ANSI_COLOR_BOLD             "\x1b[1m"
#define ANSI_COLOR_GREEN            "\x1b[32m"
#define ANSI_COLOR_MAGENTA          "\x1b[35m"
#define ANSI_COLOR_CYAN             "\x1b[36m"
#define ANSI_COLOR_RESET            "\x1b[0m"

/**
 *  \brief      Libhelper printf
 * 
 *  Re-implementation of printf() that can colour output depending on the
 *  log type.
 * 
 *  \param log_type     The log type.
 *  \param fmt          Characters to print.
 */
extern int
__libhelper_printf (log_type     msg_type,
                    char        *fmt,
                    ...);

/**
 *  Macro's for __libhelper_printf for Error, Debug and Warning.
 */
#define errorf(fmt, ...)            __libhelper_printf (LOG_ERROR, fmt, ##__VA_ARGS__)
#define debugf(fmt, ...)            __libhelper_printf (LOG_DEBUG, fmt, ##__VA_ARGS__)
#define warningf(fmt, ...)          __libhelper_printf (LOG_WARNING, fmt, ##__VA_ARGS__)
#define ci_logf(fmt, ...)         __libhelper_printf (LOG_CI, fmt, ##__VA_ARGS__)

#define test_failuref(fmt, ...)          __libhelper_printf (LOG_TEST_FAIL, fmt, ##__VA_ARGS__)
#define test_successf(fmt, ...)          __libhelper_printf (LOG_TEST_SUCCESS, fmt, ##__VA_ARGS__)

/**
 * 
 */
void
hlog_print_list_header (char *header);
void
hlog_print_list_subheader (char *padding, char *header);
void
hlog_print_list_item (char *padding, char *name, char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* __libhelper_core_logger_h__ */
