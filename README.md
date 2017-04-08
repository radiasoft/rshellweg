#### rslinac -- Python and python-wrapped utilities for modeling linear accelerators

The rslinac repository is where RadiaSoft maintains open source software for modeling linacs.  'Linac' is a long-time pseudo-acronym for 'linear accelerator'. At the moment, rslinac contains the Hellweg kernel, the Windows-only Hellweg GUI, associated Python code for testing, etc. In the future, rslinac could be broadened to include other software specific to modeling linacs.

#### Hellweg -- rapid simulation of beams in cavities, with beam loading

Hellweg is a C++ application for simulating high-average-power, traveling wave (TW) electron linacs, including the important effects of beam loading. It can also be used for modeling beam transport with a variety of standard elements.

Hellweg has a convenient GUI for use on Windows, if compiled with [Embarcadero's C++ Builder] (https://www.embarcadero.com/products/cbuilder) using the provided [configuration file]  (https://github.com/radiasoft/rslinac/blob/master/src/gui/Hellweg.cbproj).

Documentation for use of the GUI is provided https://github.com/radiasoft/rslinac/tree/master/docs
A suite of examples for use with the GUI is also provided https://github.com/radiasoft/rslinac/tree/master/src/examples

The Hellweg physics kernel compiles on Linux without the GUI. The C++ physics kernel provides an API that is called from Python. These Python-wrapped capabilities are referred to as 'rslinac' and will be made available via PyPI.

A browser-based GUI is available for open beta testing of Hellweg simulations in the cloud, http://beta.sirepo.com/hellweg
