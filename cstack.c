#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(VAR) (void)(VAR)

struct stack_entries_table g_table = {0u, NULL};

hstack_t stack_new(void)
{
    g_table.size++;                                                                     // увеличиваем размер таблицы стеков
    g_table.entries = realloc(g_table.entries, g_table.size*sizeof(stack_entry_t));     // выделяем память под элемент таблицы стеков

    // Инициализируем новый стек нулевыми значениями
    g_table.entries[g_table.size-1].stack = NULL;
    g_table.entries[g_table.size-1].reserved = 0;

    // Возвращаем размер таблицы меньший на единицу, чтобы было соответствие по индексу
    return g_table.size-1;
}

void stack_free(const hstack_t hstack)
{
    if(!(stack_valid_handler(hstack)))                       // проверка на валидность хэндлера
    {
        g_table.entries[hstack].reserved = -1;            // помечаем как удаленный
        stack_type top = g_table.entries[hstack].stack;   // вершина стека

        while(top != NULL)                                // пока не дойдем до дна стека
        {
            stack_type tmp = top->prev;                   // временная переменная для хранения указателя на следующий элемент
            free(top);
            top = tmp;
        }
    }
}

int stack_valid_handler(const hstack_t hstack)
{
    if(hstack+1 > (int)(g_table.size) ||                  // выходит за размер таблицы
       hstack < 0 ||                                      // отрицательные значения
       g_table.entries[hstack].reserved == -1)            // уже удален
    {
        return 1;
    }
    
    return 0;
}

unsigned int stack_size(const hstack_t hstack)
{
    if(!(stack_valid_handler(hstack)))
    {
        return g_table.entries[hstack].reserved;
    }
    return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if(!(stack_valid_handler(hstack)) &&
        data_in != NULL &&
        size > 0)
    {
        stack_type new = malloc(sizeof(struct node) + size);     // выделяем память под элемент
        
        if (new != NULL)
        {
            g_table.entries[hstack].reserved += 1;            // увеличиваем размер стека
            new->prev = g_table.entries[hstack].stack;        // Указываем на предыдущий узел
            new->size = size;                                 // Сохраняем размер данных
            g_table.entries[hstack].stack = new;              // обновляем вершину
            
            // Копируем данные в массив
            memcpy(new->data, data_in, size);
        }

    }
    else
    {
        //Обозначить ошибку
    }
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if(!(stack_valid_handler(hstack))&&
        data_out != NULL)
    {
        stack_type top = g_table.entries[hstack].stack;          // вершина стека
        
        if (top != NULL)
        {
            // проверка что размеры совпадают
            if(top->size != size) return 0;

            // Копируем данные в массив
            if (data_out != NULL && size > 0) {
                memcpy(data_out, top->data, size);            // копируем данные

                // Обновляем вершину
                g_table.entries[hstack].stack = top->prev;

                // Изменяем размер стека
                g_table.entries[hstack].reserved -= 1;

                // Убираем вершину
                free(top);

                return size;                                  // возвращаем количество записанных байт
            }
        }
    }
    else
    {
        //Обозначить ошибку
    }
    return 0;
}

