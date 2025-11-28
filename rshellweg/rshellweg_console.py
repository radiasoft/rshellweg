"""Front-end command line for :mod:`rshellweg`.

See :mod:`pykern.pkcli` for how this module is used.

:copyright: Copyright (c) 2025 RadiaSoft LLC.  All Rights Reserved.
:license: https://www.apache.org/licenses/LICENSE-2.0.html
"""

import pykern.pkcli
import sys


def main():
    return pykern.pkcli.main("rshellweg")


if __name__ == "__main__":
    sys.exit(main())
