#!/bin/bash

set -e -u -o pipefail

REPO_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )"

docker pull radiasoft/beamsim:latest
docker run --privileged --rm -it -u root -v "${REPO_DIR}:/rslinac" radiasoft/beamsim /bin/bash
