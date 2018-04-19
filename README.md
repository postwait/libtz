# A simple `gmtime_r` inspired time library.

This library allows taking "actual time" (UNIX epoch in seconds from 1970 UTC) and
determining a sensible date for a user in any timezone (available in TZif2 format locally).

### Example

```c
    #include <libtz.h>

    int main() {
      const char *error;
      tzinfo_t *tgt;
      const tzzone_t *tz;
      struct tm space, *result;
      time_t now = time(NULL);

      tgt = libtz_open("Arctic/Longyearbyen", &error);
      if(!tgt) bail(error);

      now = time(NULL);
      result = libtz_zonetime(tgt, &now, &space, &tz);
      if(!result) bail("conversion failed");

      char buf[128];
      libtz_strftime(buf, sizeof(buf), "%c", result, tz);
      printf("datetime: %s\n", buf);

      /*hours*/
      printf("hour: %d\n", result->tm_hour);

      /*offset*/
      printf("offset(seconds): %d\n", libtz_tzzone_offset(tz));
      return 0;
    }
```
