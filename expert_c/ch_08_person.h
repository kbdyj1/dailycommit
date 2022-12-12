#ifndef CH_08_PERSON_H
#define CH_08_PERSON_H

struct person_t;

struct person_t* person_new();

void person_construct(struct person_t* person,
                      const char* firstName,
                      const char* lastName,
                      unsigned int birthYear);

void person_destructor(struct person_t*);

void person_get_first_name(struct person_t*, char*);
void person_get_last_name(struct person_t*, char*);
unsigned int person_get_birth_year(struct person_t*);

#endif // CH_08_PERSON_H
