#include <string.h>
#include <stdio.h>
//=============================================================================

typedef struct {
    char name[32];
    double speed;
    double fuel;
} car_t;

void car_construct(car_t* car, const char* name)
{
    unsigned long len = strlen(name);
    strncpy(car->name, name, len);
    car->speed = 0.0;
    car->fuel = 0.0;
}

void car_destruct(car_t* car)
{}

void car_accelerate(car_t* car)
{
    car->speed += 0.05;
    car->fuel -= 1.0;
    if (car->fuel < 0.0) {
        car->fuel = 0.0;
    }
}

void car_brake(car_t* car)
{
    car->speed -= 0.07;
    if (car->speed < 0.0) {
        car->speed = 0.0;
    }
    car->fuel -= 2.0;
    if (car->fuel < 0.0) {
        car->fuel = 0.0;
    }
}

void car_refuel(car_t* car, double amount)
{
    car->fuel = amount;
}

//-------------------------------------------------------------------

struct list_t;

struct list_t* list_alloc();

void list_init(struct list_t*);
void list_destroy(struct list_t*);

int list_add(struct list_t*, int);
int list_get(struct list_t*, int, int*);
void list_clear(struct list_t*);
size_t list_size(struct list_t*);
void list_print(struct list_t*);

//=============================================================================

void test_ch_06()
{

}
