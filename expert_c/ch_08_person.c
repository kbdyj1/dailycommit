#include <stdlib.h>
#include <string.h>

#include "ch_08_person_p.h"

person_t* person_new()
{
    return (person_t*)malloc(sizeof(person_t));
}

void person_construct(person_t* person,
                        const char* firstName,
                        const char* lastName,
                        unsigned int birthYear)
{
    strcpy(person->firstName, firstName);
    strcpy(person->lastName, lastName);
    person->birthYear = birthYear;
}

void person_destruct(person_t* person)
{
}

void person_get_first_name(person_t* person, char* buffer)
{
    strcpy(buffer, person->firstName);
}

void person_get_last_name(person_t* person, char* buffer)
{
    strcpy(buffer, person->lastName);
}

unsigned int person_get_birth_year(person_t* person)
{
    return person->birthYear;
}
