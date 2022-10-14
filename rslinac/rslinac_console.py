# -*- coding: utf-8 -*-
"""Front-end command line for :mod:`rslinac`.

See :mod:`pykern.pkcli` for how this module is used.

:copyright: Copyright (c) 2016 RadiaSoft LLC.  All Rights Reserved.
:license: http://www.apache.org/licenses/LICENSE-2.0.html
"""
from __future__ import absolute_import, division, print_function

import sys

from pykern import pkcli


def main():
    return pkcli.main("rslinac")


if __name__ == "__main__":
    sys.exit(main())
