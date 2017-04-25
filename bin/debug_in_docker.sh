#!/bin/bash -l

set -e -u -o pipefail -x

GDB_SH=/tmp/gdb.sh
export REPO_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )"

do_vagrant() {
    cat <<EOF | sudo -i -u vagrant /bin/bash
set -e -u -o pipefail -x
declare -x REPO_DIR="$REPO_DIR"
$(declare -pf run_as_vagrant)
run_as_vagrant
EOF
}

main() {
    run_as_root
    do_vagrant
}

run_as_root() {
    yum install -y gdb
}

run_as_vagrant() {
    export CFLAGS="-O0 -gdwarf -g3"
    cd $REPO_DIR
    python setup.py develop 
    pip uninstall -y pytest-xdist
    gdb --args $(pyenv which python) -m pytest
}

main
