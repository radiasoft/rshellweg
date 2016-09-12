#include <exception>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <typeinfo>

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

void set_error(LIB_HELLWEG_ERR_INFO *err_info, const std::exception& e) {
    if (err_info) {
        std::strncpy(err_info->exc, typeid(e).name(), 1024);
        set_error(err_info, CPP_EXCEPT, e.what());
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
                            throw std::runtime_error("LoadData() failed");
                        }

                        if (Solver.CreateGeometry() != ERR_NO) {
                            throw std::runtime_error("CreateGeometry() failed");
                        }

                        if (Solver.CreateBeam() != ERR_NO) {
                            throw std::runtime_error("CreateBeam() failed");
                        }
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
    } catch(const std::exception& e) {
        success = false;
        set_error(err_info, e); 
    } catch (...) {
        success = false;
        set_error(err_info, CPP_EXCEPT_UNKNOWN, "Unknown exception class.");
    }

    return success;
}

