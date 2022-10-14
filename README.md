# TAsteroids

TAsteroids is an arcade game inspired by the classic Asteroids game written in C++, Javascript and QML.

## Supported Platforms
Any of the platforms supported by Qt: Windows, Linux, macOS, Android

## Installation

Windows: Binary releases can be found on the releases page of this github repository

Linux: Building from source requires at least Qt 5.10 or newer. This project can also be build with Qt 6.

It can be build using the QtCreator IDE.

Android: Install it from the Play Store

<a href='https://play.google.com/store/apps/details?id=com.blogspot.tasteroids&pcampaignid=MKT-Other-global-all-co-prtnr-py-PartBadge-Mar2515-1'><img alt='Get it on Google Play' 
src='https://play.google.com/intl/en_us/badges/images/generic/en_badge_web_generic.png' height=90px /></a>

## Building

### Linux
Requires at least Qt 5.10 or greater to build. 

Building on Debian based distros requires the following additional Qt-packages to be installed via apt (apart from the usual Qt build dependencies)
qml-module-qtquick-scene3d 
qml-module-qt3d
 qt3d5-dev
 qtmultimedia5-dev
 libqt5svg5-dev
 qtdeclarative5-dev
 qt3d-defaultgeometryloader-plugin

### Windows
Recommended Qt versions to build from source via MinGW and MSVC2017 include Qt 5.15.3 and Qt 6.4
As of March 2020, Qt for Windows > 5.11 suffers from various bugs in Qt 3D and QtJSEngine that will cause runtime errors. 

## Screenshots

<img src="https://raw.githubusercontent.com/taiko000/TAsteroids/master/playstore/Screenshot_2017-01-22-10-24-49.png" width="640" height="360">

<img src="https://raw.githubusercontent.com/taiko000/TAsteroids/master/playstore/Screenshot_2017-01-22-10-18-46.png" width="640" height="360">

## License
TAsteroids is licensed under the MIT License

(C) 2022 The TAsteroids contributors

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
