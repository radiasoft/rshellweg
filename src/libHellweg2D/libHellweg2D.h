#ifndef _libHellweg2D_h_
#define _libHellweg2D_h_

#include <stdbool.h>
#include "BeamSolver.h"
#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CPP_EXCEPT,
    CPP_EXCEPT_UNKNOWN,
    INPUT_ERROR
} lib_hellweg_err_type;

typedef struct {
    lib_hellweg_err_type type;
    char msg[1024];
    char exc[1024];
} LIB_HELLWEG_ERR_INFO;

bool lib_hellweg_run_beam_solver(const char*, const char*, const char*, LIB_HELLWEG_ERR_INFO*);

#ifdef __cplusplus
}
#endif

class HellwegBeamSolver {
    TBeamSolver *solver = NULL;
    public:
        HellwegBeamSolver(const char*, const char*);
        ~HellwegBeamSolver();
        void solve();
        void dump_bin(const char*);
        int get_number_of_points();
        int get_number_of_particles();
        double* get_structure_parameters(int);
        void save_output(const char*);
        void load_bin(const char*);
};

#endif
