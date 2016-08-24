#include "AnsiString.hpp"
#include "BeamSolver.h"
#include "libHellweg2D.h"

lib_hellweg_err lib_hellweg_run_beam_solver(const char* input_filename, const char* output_filename) {
    if (input_filename && output_filename) {
        TBeamSolver Solver = TBeamSolver();

        try {
            Solver.InputFile = input_filename;
            if (Solver.LoadData() != ERR_NO) {
                return INPUT_FILE_ERR;
            }
        } catch (...) {
            return INPUT_FILE_ERR;
        }

        try {
            Solver.CreateGeometry();
        } catch (...) {
            return GEOMETRY_STEP_ERR;
        }

        try {
            Solver.CreateBeam();
        } catch (...) {
            return BEAM_STEP_ERR;
        }

        try {
            Solver.Solve();
        } catch (...) {
            return SOLVE_STEP_ERR;
        }

        try {
            AnsiString o(output_filename);
            Solver.Output(o); 
        } catch (...) {
            return INPUT_FILE_ERR;
        }
    }

    if (!input_filename) {
        return INPUT_FILE_ERR;
    }

    return OUTPUT_FILE_ERR;
}

