#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUEUE_SIZE  100

#define PRINT_ARRAY_INFO(p)                     \
    printf("&%s : %p\n", #p, (void*)&p);        \
    printf(" %s : %p\n", #p, (void*)p);         \
    for (int i=0; i<10; i++) {                  \
        printf("0x%02x ", (unsigned char)p[i]); \
    }                                           \
    printf("\n")

#define MAKE_LEAK

//=============================================================================

static void print_mem_maps()
{
#ifdef __linux__
    FILE* fd = fopen("/proc/self/maps", "r");
    if (!fd) {
        printf("Could not open maps file.\n");
        exit(1);
    }
    char line[1024];
    while (!feof(fd)) {
        fgets(line, 1024, fd);
        printf("> %s", line);
    }
    fclose(fd);
#endif
}

static void test_memory_alloc()
{
    char* p0 = (char*)malloc(10 * sizeof(char));
    PRINT_ARRAY_INFO(p0);

    char* p1 = (char*)calloc(10, sizeof(char));
    PRINT_ARRAY_INFO(p1);

    print_mem_maps();

    free(p1);
#if defined(MAKE_LEAK)
    free(p0);
#endif
}

//-------------------------------------------------------------------


typedef struct {
    int front;
    int rear;
    double* array;
} Queue;

static void init(Queue* q)
{
    q->front = q->rear = 0;
    q->array = (double*)malloc(MAX_QUEUE_SIZE * sizeof(double));
}

static void destroy(Queue* q)
{
    free(q->array);
}

static int size(Queue* q)
{
    return q->rear - q->front;
}

static void enqueue(Queue* q, double item)
{
    q->array[q->rear] = item;
    q->rear++;
}

static double dequeue(Queue* q)
{
    double item = q->array[q->front];
    q->front++;
    return item;
}

static void test_queue()
{
    Queue *queue = (Queue*)malloc(sizeof(Queue));

    init(queue);

    enqueue(queue, 1.0);
    enqueue(queue, 2.0);
    enqueue(queue, 3.0);

    printf("%f\n", dequeue(queue));
    printf("%f\n", dequeue(queue));
    printf("%f\n", dequeue(queue));

    destroy(queue);

    free(queue);
}

//-------------------------------------------------------------------

static void fill_matrix(int* matrix, int rows, int cols)
{
    int counter = 1;
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            *(matrix + i*cols + j) = counter;
            ++counter;
        }
    }
}

static void print_matrix(int* matrix, int rows, int cols)
{
    printf("print_matrix\n");
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            printf("%d ", *(matrix + i*cols + j));
        }
        printf("\n");
    }
}

static void print_flat(int* matrix, int rows, int cols)
{
    printf("print_flat\n");
    for (int i=0; i<(rows*cols); i++) {
        printf("%d ", *(matrix + i));
    }
    printf("\n");
}

static int friendly_sum(int* matrix, int rows, int cols)
{
    int sum = 0;
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            sum += *(matrix + i*cols + j);
        }
    }
    return sum;
}

static int no_friendly_sum(int* matrix, int rows, int cols)
{
    int sum = 0;
    for (int j=0; j<cols; j++) {
        for (int i=0; i<rows; i++) {
            sum += *(matrix + i*cols + j);
        }
    }
    return sum;
}

static void test_cache_process()
{
    const int rows = 4;
    const int cols = 4;

    int* matrix = (int*)malloc(rows * cols * sizeof(int));

    fill_matrix(matrix, rows, cols);

    print_matrix(matrix, rows, cols);
    print_flat(matrix, rows, cols);

    int sum0 = friendly_sum(matrix, rows, cols);
    int sum1 = no_friendly_sum(matrix, rows, cols);

    printf("friendly sum : %d, no friendly sum : %d\n", sum0, sum1);

    free(matrix);
}

//=============================================================================

void test_ch_05()
{
#if (0) //done
    test_memory_alloc();
    test_queue();
#endif

    test_cache_process();
}
