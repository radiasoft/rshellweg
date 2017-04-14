#include <exception>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <typeinfo>

#include "AnsiString.hpp"
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

HellwegBeamSolver::HellwegBeamSolver(const char* ini_filename, const char* input_filename) {
    if (!ini_filename) {
        throw std::runtime_error("null ini_filename");
    }

    if (!input_filename) {
        throw std::runtime_error("null input_filename");
    }

    solver = new TBeamSolver(ini_filename);
    solver->InputFile = input_filename;
}

HellwegBeamSolver::~HellwegBeamSolver() {
    delete solver;
}

void HellwegBeamSolver::solve() {
    if (solver->LoadData() != ERR_NO) {
        throw std::runtime_error("LoadData() failed");
    }

    if (solver->CreateGeometry() != ERR_NO) {
        throw std::runtime_error("CreateGeometry() failed");
    }

    if (solver->CreateBeam() != ERR_NO) {
        throw std::runtime_error("CreateBeam() failed");
    }

    solver->Solve();
}

void HellwegBeamSolver::dump_bin(const char* output_filename) {
    if (!output_filename) {
        throw std::runtime_error("null output_filename");
    }

    AnsiString o(output_filename);

    solver->SaveToFile(o);
}

int HellwegBeamSolver::get_number_of_points() {
  return solver->GetNumberOfPoints();
}

int HellwegBeamSolver::get_number_of_particles() {
  return solver->GetNumberOfParticles();
}

double* HellwegBeamSolver::get_structure_parameters(int param) {
  return solver->GetStructureParameters(static_cast<TStructureParameter>(param));
}

void HellwegBeamSolver::load_bin(const char* input_filename) {
    if (!input_filename) {
        throw std::runtime_error("null input_filename");
    }

    AnsiString o(input_filename);

    solver->LoadFromFile(o);
}

void HellwegBeamSolver::save_output(const char* output_filename) {
    if (!output_filename) {
        throw std::runtime_error("null output_filename");
    }

    AnsiString o(output_filename);

    solver->Output(o);
}
