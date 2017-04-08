### rslinac
#### Python and python-wrapped utilities for modeling linear accelerators
The rslinac repository is where RadiaSoft maintains open source software for modeling linacs.  'Linac' is a long-time pseudo-acronym for 'linear accelerator'. At the moment, rslinac contains the Hellweg kernel, the Windows-only Hellweg GUI, associated Python code for testing, etc. In the future, rslinac could be broadened to include other software specific to modeling linacs.

### Hellweg
#### Author
Original author and primary developer: Sergey V. Kutsaev, RadiaBeam Technologies LLC
Contributing developer: Yury Eidelman, RadiaSoft LLC
Porting to GNU/Linux: Jose Barbe Welzel, RadiaSoft LLC
Porting to the cloud: Paul Moeller and Robert Nagler, RadiaSoft LLC

#### Rapid simulation of beams in cavities, with beam loading
Hellweg is a C++ application for simulating high-average-power, traveling wave (TW) electron linacs, including the important effects of beam loading. It can also be used for modeling beam transport with a variety of standard elements.

Hellweg has a convenient GUI for use on Windows, if compiled with [Embarcadero's C++ Builder](https://www.embarcadero.com/products/cbuilder) using the provided [configuration file](https://github.com/radiasoft/rslinac/blob/master/src/gui/Hellweg.cbproj).

We provide a [User Guide](https://github.com/radiasoft/rslinac/tree/master/docs) and a [Suite of Examples](https://github.com/radiasoft/rslinac/tree/master/src/examples).

The Hellweg physics kernel compiles on Linux without the GUI. The C++ physics kernel provides an API that is called from Python. These Python-wrapped capabilities are referred to as 'rslinac' and will be made available via PyPI.

#### Using Hellweg in the cloud
A [Browser-based GUI](http://beta.sirepo.com/hellweg) is available for open beta testing of Hellweg simulations in the cloud.

### Publications
S.V. Kutsaev, Y. Eidelman, D.L. Bruhwiler, P. Moeller, R. Nagler and J. Barbe Welzel, “Cloud-based design of high average power traveling wave linacs,” Physics Procedia (2017), submitted.

S.V. Kutsaev, Y. Eidelman and D.L. Bruhwiler, “Self-consistent 3D beam dynamics model with space charge for Hellweg code enhancement,” Phys. Rev. Accel. Beams (2017), in preparation.

### Support
Support for Hellweg and rslinac development has been provided by the US Department of Energy, Office of Science, Office of High Energy Physics under Award # DE-SC0015897. Additional support has been provided by [RadiaSoft LLC](http://radiasoft.net) and [RadiaBeam Technologies, LLC](http://radiabeam.com/).

### License
License: http://www.apache.org/licenses/LICENSE-2.0.html

Copyright (c) 2015-2016 [RadiaSoft LLC](http://radiasoft.net/open-source).  All Rights Reserved.

![RadiaSoft](https://github.com/radiasoft/images/blob/master/corporate/RadiaSoftLogoTransparent.png)
