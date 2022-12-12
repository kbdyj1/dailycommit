#include <stdio.h>
#include <stdlib.h>

#include "ch_08_person.h"
#include "ch_08_student.h"

void test_ch_08()
{
    struct student_t* student = student_new();

    student_construct(student, "Haerin", "Kang", 2006, "20060515", 0);

    char buffer[32];

    struct person_t* person = (struct person_t*)student;

    person_get_first_name(person, buffer);

    printf("first name: %s\n", buffer);

    person_get_last_name(person, buffer);

    printf("last name: %s\n", buffer);

    student_get_student_number(student, buffer);

    printf("student number: %s\n", buffer);

    unsigned int passedCredits = student_get_passed_credits(student);

    printf("student passed credits: %u\n", passedCredits);

    student_destruct(student);
    free(student);
}
