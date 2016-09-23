Hellweg2D is a C++ application for simulating high-average-power, traveling wave (TW) electron linacs, including the important effects of beam loading. It can also be used for modeling beam transport in drifts, with magnetic fields, etc.

Hellweg2D has a convenient GUI for use on Windows, if compiled with Embarcadero's commercial product, C++ Builder https://www.embarcadero.com/products/cbuilder using the provided configuration file,  https://github.com/radiasoft/rslinac/blob/master/src/gui/Hellweg.cbproj 

Documentation for use of the GUI is provided https://github.com/radiasoft/rslinac/tree/master/docs
A suite of examples for use with the GUI is also provided https://github.com/radiasoft/rslinac/tree/master/src/examples

The Hellweg2D physics kernel compiles on Linux without the GUI. The C++ physics kernel provides an API that is called from Python. These Python-wrapped capabilities are referred to as 'rslinac' and will be made available via PyPI.

