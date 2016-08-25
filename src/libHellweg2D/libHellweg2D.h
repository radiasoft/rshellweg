#ifndef _libHellweg2D_h_
#define _libHellweg2D_h_

#ifdef __cplusplus
extern "C" {
#endif

enum lib_hellweg_err {
    INPUT_FILE_ERR,
    OUTPUT_FILE_ERR,
    GEOMETRY_STEP_ERR,
    BEAM_STEP_ERR,
    SOLVE_STEP_ERR,
    SOLVER_INIT_ERR,
    INI_FILE_ERR,
    NO_ERR
};

enum lib_hellweg_err lib_hellweg_run_beam_solver(const char*, const char*, const char*);

#ifdef __cplusplus
}
#endif

#endif
