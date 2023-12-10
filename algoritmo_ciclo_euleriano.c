#include <stdio.h>
#include <stdlib.h>

#define len(arr) sizeof(arr)/sizeof(arr[0])

typedef unsigned int Vertice;

typedef struct
{
    Vertice v1, v2;
}Arista;


//vertices de la forma {0,1,...,n} 
unsigned int* get_matrix(Vertice* vertices, size_t v_n, Arista* aristas, size_t a_n)
{
    unsigned int* matrix = (unsigned int*) malloc(sizeof(unsigned int)*v_n*v_n);
    int e;
    for (int i = 0; i<v_n; i++)
    for (int j = 0; j<=i; j++)
    {
        e = 0;
        for (int k = 0; k < a_n; k++)
            if ((aristas[k].v1==i && aristas[k].v2==j) || (aristas[k].v1==j && aristas[k].v2==i))
                e++;
        matrix[i*v_n + j] = e;  
    }
    for (int i = 0; i<v_n; i++)
    for (int j = 0; j<=i; j++)
        matrix[j * v_n + i] = matrix[i * v_n +j];

    return matrix;
}


void printmatrix(Vertice* matrix, size_t len)
{
    for(int i = 0; i < len*2; i++) putchar('-');putchar('\n');
    for (int i = 0; i<len; i++)
    {
        for (int j = 0; j<len; j++)
            printf("%d ", matrix[i * len + j]);
        putchar('\n');
    }
    for(int i = 0; i < len*2; i++) putchar('-');putchar('\n');
}


void printcamino(Arista* camino, size_t len)
{
    printf("Camino: ");
    for (int i = 0; i<len; i++)
    {
        printf("{%d,%d},", camino[i].v1, camino[i].v2);
        
    }printf("{END}\n");
}


void copy(Vertice* dest, Vertice* from, size_t size)
    {
        for(int i = 0; i < size; i++)
            dest[i] = from[i];
    }



// Función principal para obtener el ciclo euleriano
Arista* get_ciclo_euleriano(Vertice* inc_matrix, size_t v_n, size_t a_n)
{
    // Se crea un array para almacenar el ciclo euleriano
    Arista* ciclo = (Arista*)malloc(a_n*sizeof(Arista));
    
    // Se crea una copia de la matriz de incidencia para modificarla durante el algoritmo
    Vertice* matrix = (Vertice*) malloc(sizeof(Vertice) * v_n * v_n);
    copy(matrix, inc_matrix, v_n * v_n);
    
    // Se inicializa el vértice actual y la variable para contar aristas
    Vertice current_v;
    int e;

    // Encontrar un vértice inicial con al menos una arista
    for (int i = 0; i < v_n; i++)
    {
        e = 0;
        for (int j = 0; j < v_n; j++)
            if (matrix[i * v_n + j])
            {
                e++;
                current_v = i;
                break;
            }
    }

    // Inicializar la cantidad mínima de aristas requeridas para continuar
    int min_e = 2;

    // Bucle principal para construir el ciclo euleriano
    for (int k = 0; k < a_n; k++)
    {   
        // Inicializar la variable para contar aristas en la fila actual
        e = 0;
        // Recorrer la fila actual del vértice actual
        for (int j = 0; j < v_n; j++)
        {
            if ((matrix[current_v * v_n + j] > 0))  
                // Recorrer la columna actual
                for (int i = 0; i < v_n; i++)
                {
                    // Contar las aristas disponibles
                    if (matrix[current_v * v_n + j] > 0) e++;
                    // Cuando se alcanza la cantidad mínima requerida de aristas
                    if (e == min_e)
                    {
                        // Almacenar la arista en el ciclo euleriano
                        ciclo[k] = (Arista) {current_v, j};
                        // Eliminar la arista en ambas direcciones de la matriz
                        matrix[current_v * v_n + j]--;
                        matrix[j * v_n + current_v]--;
                        // Actualizar el vértice actual
                        current_v = j;
                        // Salir del bucle y continuar con el siguiente paso del algoritmo
                        goto break_reset;
                    }
                }
        }

        // Si se necesitan menos aristas, reducir la cantidad mínima requerida
        if (--min_e) k--;
        // Si ya no se necesitan más aristas, salir del bucle
        else
        {
            printf("No es un grafo euleriano!");
            break;
        } 

        // Etiqueta para reiniciar la búsqueda
        break_reset:
    }
    return ciclo;

    // El ciclo euleriano se almacena en el array 'ciclo'
}





int main()
{
    Vertice vertices[] = {0,1,2,3,4};
    Arista aristas[] = {
                (Arista){0,1},
                (Arista){0,2},
                (Arista){0,3},
                (Arista){0,4},
                (Arista){1,2},
                (Arista){1,2},
                (Arista){1,4},
                (Arista){2,3},
                };
    unsigned int* matrix = get_matrix(vertices, len(vertices), aristas, len(aristas));
    printmatrix(matrix, len(vertices));
    Arista* eul_cy = get_ciclo_euleriano(matrix, len(vertices), len(aristas));
    printcamino(eul_cy, len(aristas));

    return 0;
}