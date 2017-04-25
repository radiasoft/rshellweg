### Python and python-wrapped utilities for modeling linear accelerators [![Build Status](https://travis-ci.org/radiasoft/rslinac.svg?branch=master)](https://travis-ci.org/radiasoft/rslinac)
The rslinac repository is where RadiaSoft maintains open source software for modeling linacs.  'Linac' is a long-time nickname for 'linear accelerator'. This includes the Hellweg C++ physics kernel, the Windows-only Hellweg GUI, and associated Python code for testing. In the future, rslinac will be broadened to include other software specific to modeling linacs.

### Hellweg
#### Author
Original author and primary developer: Sergey V. Kutsaev, RadiaBeam Technologies LLC

Hellweg was initially developed in the [RF laboratory of the National Research Nuclear University (MEPhI)](http://rflab.mephi.ru/en/).

Contributing developer: Yury Eidelman, RadiaSoft LLC

Porting to GNU/Linux: Jose Barbe Welzel, RadiaSoft LLC

Porting to the cloud: Paul Moeller and Robert Nagler, RadiaSoft LLC

#### Rapid simulation of beams in cavities, with beam loading
<a target="_blank" class="btn btn-default lp-code-button" href="/hellweg"><h4>Hellweg</h4></a>

Hellweg is a C++ application for simulating high-average-power, traveling wave (TW) electron linacs, including the important effects of beam loading. It can also be used for modeling beam transport with a variety of standard elements.

Hellweg has a GUI for use on Windows, if compiled with [Embarcadero's C++ Builder](https://www.embarcadero.com/products/cbuilder) using the provided [configuration file](https://github.com/radiasoft/rslinac/blob/master/src/gui/Hellweg.cbproj).

We provide a [User Guide](https://github.com/radiasoft/rslinac/tree/master/docs) and a [Suite of Examples](https://github.com/radiasoft/rslinac/tree/master/src/examples).

The Hellweg physics kernel compiles on Linux without the GUI. The C++ physics kernel provides an API that is called from Python. These Python-wrapped capabilities are referred to as 'rslinac' and will be made available via PyPI.

#### Using Hellweg in the cloud
[A browser-based GUI for Hellweg](http://beta.sirepo.com/hellweg) is available for open beta testing.

### Publications
S.V. Kutsaev, “Electron dynamics simulations with Hellweg 2D code”, [Nuclear Instruments and Methods in Physics Research A618, pp. 298–305 (2010).](http://www.sciencedirect.com/science/article/pii/S0168900210003141)

S.V. Kutsaev et al., “Design of hybrid electron linac with standing wave buncher and traveling wave structure”, [Nuclear Instruments and Methods in Physics Research A636, p. 13 (2011).](http://www.sciencedirect.com/science/article/pii/S0168900211001252)

S.V. Kutsaev, R.B. Agustsson, A. Arodzero, R.D.B. Berry, S. Boucher, Y.C. Chen, J.J. Hartzell, B.T. Jacobson, A. Laurich, A.Y. Murokh, E.A. Savin, A.Yu. Smirnov and A. Verma, “Design, Simulations and Experimental Demonstration of an Intra-Pulse Ramped-Energy Travelling Wave Linac for Cargo Inspection”, [Proc. of NAPAC, TUPOA68 (2016).](http://vrws.de/napac2016/papers/tupoa68.pdf)

S.V. Kutsaev, Y. Eidelman, D.L. Bruhwiler, P. Moeller, R. Nagler and J. Barbe Welzel, “Cloud-based design of high average power traveling wave linacs,” Physics Procedia (2017), submitted.

S.V. Kutsaev, Y. Eidelman and D.L. Bruhwiler, “Self-consistent 3D beam dynamics model with space charge for Hellweg code enhancement,” Phys. Rev. Accel. Beams (2017), in preparation.

### Support
Support for Hellweg and rslinac development has been provided by the US Department of Energy, Office of Science, Office of High Energy Physics under Award # DE-SC0015897. Additional support has been provided by [RadiaSoft LLC](http://radiasoft.net) and [RadiaBeam Technologies, LLC](http://radiabeam.com/).

### License
License: http://www.apache.org/licenses/LICENSE-2.0.html

Copyright (c) 2015-2016 [RadiaSoft LLC](http://radiasoft.net/open-source).  All Rights Reserved.

![RadiaSoft](https://github.com/radiasoft/images/blob/master/corporate/RadiaSoftLogoTransparent.png)
