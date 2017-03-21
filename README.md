# ELVE: ELVE Logic Visualisation Explorer

ELVE is a novel interactive open source desktop application for
visualisation of logic circuits. The current state of the tool offers basic
features for visualising circuits and provides a mechanism for easy
extension through plug-ins in order to fit the user’s needs. The
interactive user interface allows the user to change how the circuit is
visualised, as well as to compress, highlight or extract some logic for
analysing the circuit. The proposed tool can be used to better understand
the effects of the developed algorithms in research projects, for
educational purposes, as well as for generating figures for technical
documents.

Check the [DUHDe'17
paper](https://github.com/stdgregwar/elve/blob/master/duhde17-elve.pdf) for
more details.

## Requirements

The following software is required in order to build and use ELVE.

* Qt
* git
* g++
* boost
* libstdc++6

To download Qt, follow the instructions on
[Qt website](https://www.qt.io/download-open-source/).

In *Ubuntu* Qt and the other packages can be installed with the following command.

    sudo apt-get install build-essential git g++ libboost-all-dev libstdc++6 qt5-default qtcreator libqt5svg5-dev

In *Mac*, it is recommended to use [Homebrew](http://brew.sh/) to install
the other required packages using the following command.

    brew install git boost gcc45 --enable-cxx

## Build and Run

The easiest way to build and run ELVE is using **Qt Creator**.

First, open the **elve.pro** project file. From the *File* menu, select
*Open File or Project...*, and then open the file *elve.pro* from ELVE's
folder. Next, press on *Configure Project*. Now, you can build and run
ELVE by selecting the *Run* option from the *Build* menu.

Alternatively, you can build ELVE with your preferred tool
using qmake ([qmake documentation](http://doc.qt.io/qt-4.8/qmake-tutorial.html)).

