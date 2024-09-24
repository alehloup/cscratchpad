#include <stdio.h>
#include <string.h>

#if defined(_MSC_VER) || defined(__cplusplus)
  #define typeof_(x) auto
#else
  #define typeof_(x) typeof(x)
#endif

#define fortimes(var, ...)                     \
    for (typeof_((__VA_ARGS__)) pretty_result = (__VA_ARGS__), \
      var = 0; var < pretty_result; ++var)

#ifdef __cplusplus
  #include <type_traits>
  constexpr size_t len<char*>(char* ptr) {
      return ptr == nullptr ? 0 : strlen(ptr);
  }
  template <typename T, size_t N> constexpr size_t len(T (&)[N]) {
      return N;
  }
#else
  #define len(...) \
    ((__VA_ARGS__) == NULL) ? 0                  \
      : _Generic((__VA_ARGS__),                    \
        char const *: (strlen((const char*)(__VA_ARGS__))),    \
        char *: (strlen((char*)(__VA_ARGS__))),          \
        default: (sizeof(__VA_ARGS__) / sizeof(__VA_ARGS__[0])))
#endif

int main(void) {
  //int x[5] = {0};

  fortimes(y, 5)
    printf("%d ", y);
  printf("\n");

  //printf("%zu %zu \n", len(x), len("alessandro"));

  return 0;
}
