#pragma once
// The words of an action with the number in them. No LVGL and no Tessera here, so a test on a computer can run it.
//
//   {digit}   the key that was just pressed: "7"
//   {code}    the digits typed so far: "407"
//
// So `remote.send_command command=NUM_{digit}` sends NUM_7, and `input_number.set_value value={code}` sends 407.
#include <cstddef>
#include <cstring>

namespace esphome::number_pad {

// Copies `text` to `out` (of `size` bytes, the end mark included) with {digit} and {code} replaced. False when the
// result does not fit; `out` then holds what did fit and is not to be sent.
inline bool expand(const char *text, const char *digit, const char *code, char *out, size_t size) {
  if (size == 0) return false;
  size_t n = 0;
  auto put = [&](const char *s, size_t len) {
    if (n + len >= size) return false;
    std::memcpy(out + n, s, len);
    n += len;
    return true;
  };
  bool fits = true;
  for (const char *p = text; *p && fits;) {
    if (std::strncmp(p, "{digit}", 7) == 0) {
      fits = put(digit, std::strlen(digit));
      p += 7;
    } else if (std::strncmp(p, "{code}", 6) == 0) {
      fits = put(code, std::strlen(code));
      p += 6;
    } else {
      fits = put(p, 1);
      ++p;
    }
  }
  out[n] = '\0';
  return fits;
}

}  // namespace esphome::number_pad
