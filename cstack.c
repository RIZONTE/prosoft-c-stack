#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cstack.h"

#define UNUSED(VAR) (void)(VAR)

struct node
{
    struct node* prev;
    unsigned int size;
    char data[0];
};

typedef struct node* stack_type;

struct stack_entry
{
    int stack_size;                         // Поле хранящее кол-во элементов в стеке. -1 если стек удален
    stack_type stack;
};

typedef struct stack_entry stack_entry_t;

struct stack_entries_table
{
    unsigned int capacity;
    unsigned int size;
    stack_entry_t* entries;
};

struct stack_entries_table g_table = {1u, 0u, NULL};

hstack_t stack_new(void)
{
    // Сначала ищем очищенный стек или свободный элемент entries
    if(g_table.entries != NULL && g_table.size != 0)
    {
        for(size_t i = 0; i < g_table.size; i++)
        {
            if(g_table.entries[i].stack_size == -1)
            {
                // Инициализируем новый стек нулевыми значениями
                g_table.entries[i].stack = NULL;
                g_table.entries[i].stack_size = 0;

                return i;
            }
        }

        if(g_table.size < g_table.capacity)
        {
            // Инициализируем новый стек нулевыми значениями
            g_table.entries[g_table.size].stack = NULL;
            g_table.entries[g_table.size].stack_size = 0;

            g_table.size++;

            return g_table.size - 1;
        }
    }


    // Если нет очищенного или все занято, то реаллоцируем
    g_table.capacity *= 2;
    // g_table.capacity = g_table.capacity > 0 ? 1 : _table.capacity;
    g_table.size++;                                                                         // увеличиваем размер таблицы стеков
    g_table.entries = realloc(g_table.entries, g_table.capacity*sizeof(stack_entry_t));     // выделяем память под элемент таблицы стеков

    // Проверка на ошибку при реаллокации
    if(g_table.entries == NULL)
    {
        return -1;
    }

    // Инициализируем новый стек нулевыми значениями
    g_table.entries[g_table.size-1].stack = NULL;
    g_table.entries[g_table.size-1].stack_size = 0;

    // Возвращаем размер таблицы меньший на единицу, чтобы было соответствие по индексу
    return g_table.size-1;
}

void stack_free(const hstack_t hstack)
{
    if(stack_valid_handler(hstack) == 0)                       // проверка на валидность хэндлера
    {
        g_table.entries[hstack].stack_size = -1;             // помечаем как удаленный
        stack_type top = g_table.entries[hstack].stack;      // вершина стека

        while(top != NULL)                                   // пока не дойдем до дна стека
        {
            stack_type tmp = top->prev;                      // временная переменная для хранения указателя на следующий элемент
            free(top);
            top = tmp;
        }

        g_table.entries[hstack].stack = NULL;
    }
}

int stack_valid_handler(const hstack_t hstack)
{
    if(hstack+1 > (int)(g_table.size) ||                     // выходит за размер таблицы
       hstack < 0 ||                                         // отрицательные значения
       g_table.entries[hstack].stack_size == -1)             // уже удален
    {
        return 1;
    }
    return 0;
}

unsigned int stack_size(const hstack_t hstack)
{
    if(stack_valid_handler(hstack) == 0)
    {
        return g_table.entries[hstack].stack_size;
    }
    return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if(stack_valid_handler(hstack) == 0 &&
        data_in != NULL &&
        size > 0)
    {
        stack_type new = malloc(sizeof(struct node) + size);     // выделяем память под элемент
        
        if (new != NULL)
        {
            g_table.entries[hstack].stack_size += 1;             // увеличиваем размер стека
            new->prev = g_table.entries[hstack].stack;           // Указываем на предыдущий узел
            new->size = size;                                    // Сохраняем размер данных
            g_table.entries[hstack].stack = new;                 // обновляем вершину
            
            // Копируем данные в массив
            memcpy(new->data, data_in, size);
        }

    }
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if(stack_valid_handler(hstack) == 0&&
        data_out != NULL)
    {
        stack_type top = g_table.entries[hstack].stack;          // вершина стека
        
        if (top != NULL)
        {
            // проверка что размеры совпадают
            if(top->size != size) return 0;

            // Копируем данные в массив
            if (size > 0) {
                memcpy(data_out, top->data, size);            // копируем данные

                // Обновляем вершину
                g_table.entries[hstack].stack = top->prev;

                // Изменяем размер стека
                g_table.entries[hstack].stack_size -= 1;

                // Убираем вершину
                free(top);

                return size;                                  // возвращаем количество записанных байт
            }
        }
    }
    return 0;
}

