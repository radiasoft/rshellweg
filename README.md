#rslinac

`rslinac` is a python wrapped C++ physics kernel for simulating high-average-power, traveling wave (TW) electron linacs, including the important effects of beam loading. It can also be used for modeling beam transport in drifts, with magnetic fields, etc.

## Building

### Requirements 

* C++ compiler (gcc, clang)
* [Boost](http://www.boost.org)
* [Python](https://www.python.org) 2.7+.

### Installation

```sh
python setup.py install
```

## Development

`rslinac` uses [Cython](http://cython.org) to integrate the C++ within Python. To update or modify the Python interface to the C++ see `pyhellweg.pyx`. 

If `pyhellweg.pyx` is modified it is necessary to regenerate the C++ bindings source code:

```sh
cython --cplus pyhellweg.pyx
```

If the interface changes are permanent, don't forget to check in `pyhellweg.cpp`.

