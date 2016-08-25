#ifndef _libHellweg2D_h_
#define _libHellweg2D_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

typedef enum {
    CPP_EXCEPT,
    INPUT_ERROR
} lib_hellweg_err_type;

typedef struct {
    lib_hellweg_err_type type;
    char msg[1024];
} LIB_HELLWEG_ERR_INFO;

bool lib_hellweg_run_beam_solver(const char*, const char*, const char*, LIB_HELLWEG_ERR_INFO*);

#ifdef __cplusplus
}
#endif

#endif
