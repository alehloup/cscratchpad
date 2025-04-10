#include "aleh/aleh.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main() {
    int arr[] = {1, 2, 3};
    int *ptr = arr;
    const char str[64] = "Hello, World!";
    char non_literal_str[] = "Hello, Dynamic!";
    
    // Teste para o array
    printf("arr é array: %d\n", is_array(arr));  // Espera 1, pois arr é um array
    printf("arr é array: %d\n", is_array(str));  // Espera 1, pois arr é um array
    printf("ptr é array: %d\n", is_array(ptr));  // Espera 0, pois ptr é um ponteiro
    
    // Teste para string literal
    printf("str é literal: %d\n", is_literal_str("Testando string direta"));  // 1 no GCC, 0 no TCC
    printf("str é literal: %d\n", is_literal_str(str));  // 0 no GCC e TCC ! (achei que ia ser literal)
    printf("non_literal_str é literal: %d\n", is_literal_str(non_literal_str));  // Espera 0, pois non_literal_str não é uma string literal (é uma variável)
    
    return 0;
}

