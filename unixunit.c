/**
 * @file unixunit.c
 * @brief Unix Unit, a Unix unit test toolchain for PUnit.
 */

/* Copyright (c) 2012, Ninjaware Oy, Olli Vanhoja <olli.vanhoja@ninjaware.fi>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "unixunit.h"

/* Stdin writer variables */
static FILE * uu_stdin_writer = NULL;
static int uu_pipe[2];

/**
 * Open pipe.
 * This should be called in test setup.
 */
void uu_open_pipe()
{
    /* Open pipe */
    if (pipe(uu_pipe) == -1) {
        fprintf(stderr, "FAILED: Pipe failed.");
        exit(1);
    }
}

/**
 * Opens a file stream for writing to stdin.
 * This should be called before using uu_write_stdin(char * str).
 */
void uu_open_stdin_writer()
{
    dup2(uu_pipe[0], STDIN_FILENO);
    uu_stdin_writer = fdopen(uu_pipe[1], "w");
    if (uu_stdin_writer == NULL) {
        fprintf(stderr, "FAILED: Cannot open stdin for write access.");
        exit(1);
    }
}

/**
 * Writes a null-terminated string to the stdin.
 * @param str a null-terminated string.
 */
void uu_write_stdin(char * str)
{
    if (uu_stdin_writer == NULL) {
        fprintf(stderr, "FAILED: Stdin writer not open.");
        exit(1);
    }
    fwrite(str, 1, strlen(str), uu_stdin_writer);
}

/**
 * Closes the stdin writer.
 * This should be called before executing any code that uses stdin.
 */
void uu_close_stdin_writer()
{
    fclose(uu_stdin_writer);
    uu_stdin_writer = NULL;
}

/**
 * Close pipe.
 * This should be called in test teardown.
 */
void uu_close_pipe()
{
    /* Close pipe */
    close(uu_pipe[0]);
    close(uu_pipe[1]);
}
