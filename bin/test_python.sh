#!/bin/bash -l

set -x -e -u -o pipefail

cd /rslinac
pip install -r requirements.txt
python setup.py test
