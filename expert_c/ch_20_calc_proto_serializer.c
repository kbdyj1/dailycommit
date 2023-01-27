#include "ch_20_calc_proto_seralizer.h"
#include "common_def.h"

#define FIELD_DELIM     '#'
#define MESSAGE_DELIM   '$'

struct buffer_t calc_proto_serializer_server_serialize(
        struct calc_proto_serializer_t* ser,
        const struct calc_proto_res_t* res)
{
    struct buffer_t buf;
    char res_result_str[64];
//    _serialize_double(res_result_str, res->result);   //TODO
    buf.data = (char*)malloc(64 * sizeof(char));
    sprintf(buf.data, "%d%c%d%c%s%c",
            res->req_id, FIELD_DELIM,
            res->status, FIELD_DELIM,
            res_result_str, MESSAGE_DELIM);
    buf.len = strlen(buf.data);

    return buf;
}

void calc_proto_serializer_server_deserialize(
        struct calc_proto_serializer_t* ser,
        struct buffer_t buf,
        bool_t* found)
{
    if (found) {
        *found = FALSE;
    }
//    _deserialize(ser, buf, _parse_req_and_notify, ERROR_INVALID_REQ, found);  //TODO
}
