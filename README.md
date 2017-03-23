[![Build Status](https://travis-ci.org/stdgregwar/elve.png)](https://travis-ci.org/stdgregwar/elve)

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
* boost
* g++ (at least version 6.2.0)
* libstdc++6

To download Qt, follow the instructions on
[Qt website](https://www.qt.io/download-open-source/).

In *Ubuntu* Qt and the other packages can be installed with the following command.

    sudo apt-get install build-essential git libboost-all-dev g++-6 libstdc++6 
    sudo apt-get install qt5-default qtcreator libqt5svg5-dev

In *Mac*, it is recommended to use [Homebrew](http://brew.sh/) to install
the required packages (except Qt) using the following command.

    brew install git boost gcc6

## Build and Run

The easiest way to build and run ELVE is using **Qt Creator**.

First, open the **elve.pro** project file. One way to do this is to select
*Open File or Project...* from the *File* menu, and then open the file
*elve.pro* from ELVE's folder. Next, press on *Configure Project*. Now, you
can build and run ELVE by selecting the *Run* option from the *Build* menu.

Alternatively, you can build ELVE with your preferred tool
using qmake ([qmake documentation](http://doc.qt.io/qt-4.8/qmake-tutorial.html)).

## Development

Would you like to write your own plugin? Take a look at the 
([available documentation](https://stdgregwar.github.io/elve/html/)).
