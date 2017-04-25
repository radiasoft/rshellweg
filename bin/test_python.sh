#!/bin/bash -l

set -x -e -u -o pipefail

cd /rslinac
python setup.py test
