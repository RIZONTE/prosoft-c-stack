#ifndef CSTACK_H
#define CSTACK_H

#include <stdio.h>

typedef int hstack_t;

// Создать новый стек.
hstack_t stack_new(void);

// Удалить стек, если соответствующий хэндлеру стек существует.
void stack_free(const hstack_t stack);

// Проверить хэндлер.  0 - соответствующий хэндлеру стек существует, 1 - нет.
int stack_valid_handler(const hstack_t stack);

// Получить количество элементов в стеке, если соответствующий хэндлеру стек существует.
// Returns: количество элементов в стеке, если соответствующий хэндлеру стек существует, или 0 в противном случае.
unsigned int stack_size(const hstack_t stack);

// Добавить элемент данных из буфера в стек, если соответствующий хэндлеру стек существует.
void stack_push(const hstack_t stack, const void* data_in, const unsigned int size);

// Извлечь элемент из стека и записать данные этого элемента в буфер, если соответствующий хэндлеру стек существует.
// Returns: размер записанных данных в байтах, если соответствующий хэндлеру стек существует, или 0 в противном случае.
unsigned int stack_pop(const hstack_t stack, void* data_out, const unsigned int size);

#endif /* CSTACK_H */
