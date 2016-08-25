#include <exception>
#include <stdexcept>
#include <cstdlib>
#include <cstring>

#include "AnsiString.hpp"
#include "BeamSolver.h"
#include "libHellweg2D.h"

void set_error(LIB_HELLWEG_ERR_INFO *err_info, lib_hellweg_err_type type, const char *msg) {
    if (err_info) {
        err_info->type = type;

        if (msg) {
            std::strncpy(err_info->msg, msg, 1024);
        }
    }
}


bool lib_hellweg_run_beam_solver(
        const char *ini_filename,
        const char *input_filename, 
        const char *output_filename,
        LIB_HELLWEG_ERR_INFO *err_info) {
    
    bool success = true;
    
    try {
        do {
            if (ini_filename) {
                AnsiString ini(ini_filename);

                if (input_filename && output_filename) {
                    TBeamSolver Solver(ini);
                    AnsiString o(output_filename);

                        Solver.InputFile = input_filename;
                         
                        if (Solver.LoadData() != ERR_NO) {
                            success = false;
                            set_error(err_info, INPUT_ERROR, "LoadData() failed");
                            break;
                        }

                        Solver.CreateGeometry();

                        Solver.CreateBeam();

                        Solver.Solve();

                        Solver.Output(o);
                } else {
                    success = false;

                    if (input_filename) {
                        set_error(err_info, INPUT_ERROR, "Missing output_filename");
                    } else {
                        set_error(err_info, INPUT_ERROR, "Missing input_filename");
                    }
                }
            } else {
                success = false;
                set_error(err_info, INPUT_ERROR, "Missing ini_filename");
            }
        } while(false);
    } catch (...) {
        std::exception_ptr eptr = std::current_exception();

        success = false;

        if (eptr) {
            try {
                std::rethrow_exception(eptr); 
            } catch(const std::exception& e) {
                set_error(err_info, CPP_EXCEPT, e.what());
            } catch (...) {
                set_error(err_info, CPP_EXCEPT, "Unknown exception class.");
            }
        }
    }

    return success;

}

