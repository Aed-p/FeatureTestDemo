#include <libunwind.h>
#include <unwind.h>

int unw_backtrace(void **buffer, int size)
{
  unw_context_t context;
  unw_cursor_t cursor;
  if (unw_getcontext(&context) || unw_init_local(&cursor, &context)) {
    return 0;
  }

  unw_word_t ip;
  int current = 0;
  while (unw_step(&cursor) > 0) {
    if (current >= size || unw_get_reg(&cursor, UNW_REG_IP, &ip)) {
      break;
    }

    buffer[current++] = reinterpret_cast<void *>(static_cast<uintptr_t>(ip));
  }

  return current;
}