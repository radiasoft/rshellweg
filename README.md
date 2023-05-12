### Utilities for working with the Hellweg code.
The rshellweg repository is where RadiaSoft maintains open source software for modeling linacs. This includes the Hellweg C++ physics kernel, the Windows-only Hellweg GUI, and associated Python code for testing.

#### Support
For support or more information, please [create an issue](https://github.com/radiasoft/rshellweg/issues) in this repository.

#### Author
Original author and primary developer: [Sergey V. Kutsaev](https://www.linkedin.com/in/sergey-kutsaev-9990147), RadiaBeam Technologies LLC

Hellweg was initially developed at the [RF laboratory of the National Research Nuclear University (MEPhI)](http://rflab.mephi.ru/en/).

Contributing developers: Ilya Pogorelov, Paul Moeller, Robert Nagler, Garret Sugarbaker and Evan Carlin (RadiaSoft LLC), Yury Eidelman (Eidelman Scientific Consulting)

Original port to GNU/Linux: [Jose 'Pepe' Barbe-Welzel](https://github.com/elventear)

#### Rapid simulation of beams in cavities, with beam loading
[Hellweg](https://github.com/radiasoft/rshellweg/blob/master/README.md)

Hellweg is a C++ application for simulating high-average-power, traveling wave (TW) electron linacs, including the important effects of beam loading. It can also be used for modeling beam transport with a variety of standard elements.

Hellweg has a GUI for use on Windows, if compiled with [Embarcadero's C++ Builder](https://www.embarcadero.com/products/cbuilder) using the provided [configuration file](https://github.com/radiasoft/rshellweg/blob/master/src/gui/Hellweg.cbproj).

We provide a [User Guide](https://github.com/radiasoft/rshellweg/tree/master/docs) and a [Suite of Examples](https://github.com/radiasoft/rshellweg/tree/master/src/examples).

The Hellweg physics kernel compiles on Linux without the GUI. The C++ physics kernel provides an API that is called from Python. These Python-wrapped capabilities are referred to as 'rshellweg'.

### Publications
S.V. Kutsaev, “Electron dynamics simulations with Hellweg 2D code”, [Nuclear Instruments and Methods in Physics Research A618, pp. 298–305](http://www.sciencedirect.com/science/article/pii/S0168900210003141) (2010).

S.V. Kutsaev, N.P. Sobenin, A.Yu. Smirnov, D.S. Kamenschikov, M.A. Gusarova, K.I. Nikolskiy, A.A. Zavadtsev and M.V. Lalayan, “Design of hybrid electron linac with standing wave buncher and traveling wave structure”, [Nuclear Instruments and Methods in Physics Research A636, p. 13](http://www.sciencedirect.com/science/article/pii/S0168900211001252) (2011).

S.V. Kutsaev, R.B. Agustsson, A. Arodzero, R.D.B. Berry, S. Boucher, Y.C. Chen, J.J. Hartzell, B.T. Jacobson, A. Laurich, A.Y. Murokh, E.A. Savin, A.Yu. Smirnov and A. Verma, “Design, Simulations and Experimental Demonstration of an Intra-Pulse Ramped-Energy Travelling Wave Linac for Cargo Inspection”, [Proc. of NAPAC, TUPOA68](http://vrws.de/napac2016/papers/tupoa68.pdf) (2016).

S.V. Kutsaev, Y. Eidelman, D.L. Bruhwiler, P. Moeller, J.F. Barbe Welzel and R. Nagler, “Cloud-based design of high average power traveling wave linacs,” [J. Phys.: Conf. Series 941 012106](https://iopscience.iop.org/article/10.1088/1742-6596/941/1/012106) (2018).

S.V. Kutsaev, Y. Eidelman and D.L. Bruhwiler, “Generalized 3D beam dynamics model for industrial traveling wave linacs design and simulations,” [Nuclear Instr. and Meth. in Phys. Research A906, p. 127](https://doi.org/10.1016/j.nima.2018.07.078) (2018).

S.V. Kutsaev, Y. Eidelman and D.L. Bruhwiler, “Self-consistent 3D beam dynamics model with space charge for Hellweg code enhancement” (2019), in preparation.

### Support
Support for Hellweg and rshellweg development has been provided by the US Department of Energy, Office of Science, Office of High Energy Physics under Award # DE-SC0015897. Additional support has been provided by [RadiaSoft LLC](http://radiasoft.net) and [RadiaBeam Technologies, LLC](http://radiabeam.com/).

### License
License: http://www.apache.org/licenses/LICENSE-2.0.html

Copyright (c) 2015-2023 [RadiaSoft LLC](http://radiasoft.net/open-source).  All Rights Reserved.

![RadiaSoft](https://github.com/radiasoft/images/blob/master/corporate/RadiaSoftLogoTransparent.png)
