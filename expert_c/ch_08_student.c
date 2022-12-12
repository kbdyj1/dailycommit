#include <stdlib.h>
#include <string.h>

#include "ch_08_person.h"
#include "ch_08_person_p.h"

typedef struct {
    person_t person;
    char* studentNumber;
    unsigned int passedCredits;
} student_t;

student_t* student_new()
{
    return (student_t*)malloc(sizeof(student_t));
}

void student_construct(student_t* student,
                       const char* firstName,
                       const char* lastName,
                       unsigned int birthYear,
                       const char* studentNumber,
                       unsigned int passedCredits)
{
    struct persont_t* person = (person_t*)student;
    person_construct(person, firstName, lastName, birthYear);

    student->studentNumber = (char*)malloc(16 * sizeof(char));
    strcpy(student->studentNumber, studentNumber);
    student->passedCredits = passedCredits;
}

void student_destruct(student_t* student)
{
    free(student->studentNumber);
    person_destruct((struct person_t*)student);
}

void student_get_student_number(student_t* student, char* buffer)
{
    strcpy(buffer, student->studentNumber);
}

unsigned int student_get_passed_credits(student_t* student)
{
    return student->passedCredits;
}
