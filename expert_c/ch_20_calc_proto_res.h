#ifndef CH_20_CALC_PROTO_RES_H
#define CH_20_CALC_PROTO_RES_H

#include <stdint.h>

#define STATUS_OK               0
#define STATUS_INVALID_REQ      1
#define STATUS_INVALID_METHOD   2
#define STATUS_INVALID_OPERAND  3
#define STATUS_DIV_BY_ZERO      4
#define STATUS_INTERNAL_ERROR   5

typedef int status_t;

struct calc_proto_res_t {
    int32_t req_id;
    status_t status;
    double result;
};

#endif // CH_20_CALC_PROTO_RES_H
