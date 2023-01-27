#ifndef CH_20_CALC_PROTO_SERALIZER_H
#define CH_20_CALC_PROTO_SERALIZER_H

#include "ch_20_calc_proto_req.h"
#include "ch_20_calc_proto_res.h"
#include "common_def.h"

#define ERROR_INVALID_REQ           101
#define ERROR_INVALID_REQ_ID        102
#define ERROR_INVALID_REQ_METHOD    103
#define ERROR_INVALID_REQ_OP1       104
#define ERROR_INVALID_REQ_OP2       105

#define ERROR_INVALID_RES           201
#define ERROR_INVALID_RES_REQ_ID    202
#define ERROR_INVALID_RES_STATUS    203
#define ERROR_INVALID_RES_RESULT    204

#define ERROR_UNKNOWN               210

struct buffer_t {
    char* data;
    int len;
};

struct calc_proto_serializer_t;

typedef void (*req_callback_t)(void* owner, struct calc_proto_req_t);
typedef void (*res_callback_t)(void* owner, struct calc_proto_res_t);
typedef void (*err_callback_t)(void* owner, const int req_id, const int error_code);

struct calc_proto_serializer_t* calc_proto_serializer_new();

void calc_proto_serializer_delete(struct calc_proto_serializer_t* ser);
void calc_proto_serializer_construct(struct calc_proto_serializer_t* ser, void* owner, int ring_buffer_size);
void calc_proto_serializer_destruct(struct calc_proto_serializer_t* ser);
void*calc_proto_serializer_get_context(struct calc_proto_serializer_t* ser);
void calc_proto_serializer_set_req_callback(struct calc_proto_serializer_t* ser, req_callback_t cb);
void calc_proto_serializer_set_res_callback(struct calc_proto_serializer_t* ser, res_callback_t cb);
void calc_proto_serializer_set_error_callback(struct calc_proto_serializer_t* ser, err_callback_t cb);

void calc_proto_serializer_server_deserializer(struct calc_proto_serializer_t* ser, struct buffer_t buffer, bool_t* found);
struct buffer_t calc_proto_serializer_server_serializer(struct calc_proto_serializer_t* ser, const struct calc_proto_res_t* res);

void calc_proto_serializer_client_deserialize(struct calc_proto_serializer_t* ser, struct buffer_t buffer, bool_t* found);
struct buffer_t calc_proto_serializer_client_serialize(struct calc_proto_serializer_t* ser, const struct calc_proto_req_t* req);

#endif // CH_20_CALC_PROTO_SERALIZER_H
