/*
 * Copyright (c) 2018, Circonus, Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *    * Neither the name Circonus, Inc. nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <libtz.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int tcount = 1;
static int failed = 0;
static char *test_desc = "??";
#define okf(fmt, ex...) do { \
  printf("ok %d - %s : " fmt "\n", tcount++, test_desc, ex); \
} while(0)
#define ok() do { \
  printf("ok %d - %s\n", tcount++, test_desc); \
} while(0)
#define notokf(fmt, ex...) do { \
  printf("not ok %d - %s : " fmt "\n", tcount++, test_desc, ex); \
  failed++; \
} while(0)
#define notok() do { \
  printf("not ok %d - %s\n", tcount++, test_desc); \
  failed++; \
} while(0)
static void is(int expr) {
  if(expr) { ok(); } else { notok(); }
};
#define isf(expr, fmt, ex...) do { \
  if(expr) { \
    printf("ok %d - %s : " fmt "\n", tcount++, test_desc, ex); \
  } else { \
    printf("not ok %d - %s : " fmt "\n", tcount++, test_desc, ex); \
    failed++; \
  } \
} while(0)
#define T(a) do { \
  test_desc = #a; \
  a; \
  test_desc = "??"; \
} while(0)

int main() {
  const char *err;
  tzinfo_t *useast = libtz_open("US/Eastern", &err);
  T(is(useast != NULL));
  T(isf(err == NULL, "%s", err));
  T(isf(!strcmp(libtz_tzinfo_name(useast), "EST"), "%s", libtz_tzinfo_name(useast)));

  struct tm foo, *res;
  time_t tt = 1523653052; /*Fri Apr 13 20:57:32 2018 UTC*/
  const tzzone_t *tz;
  res = libtz_zonetime(useast, &tt, &foo, &tz);
  T(is(res != NULL));
  T(is(res->tm_isdst == 1));
  T(is(res->tm_hour == 16));
  T(is(!strcmp(libtz_tzzone_name(tz), "EDT")));

  tt = 1520197275; /* Sun Mar  4 21:01:15 2018 UTC */
  res = libtz_zonetime(useast, &tt, &foo, NULL);
  T(is(res != NULL));
  T(is(res->tm_isdst == 0));
  T(is(res->tm_hour == 16));

  exit(!(failed==0));
}
