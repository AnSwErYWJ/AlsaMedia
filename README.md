# AlsaMedia 
[![Open Source Love](https://badges.frapsoft.com/os/v1/open-source.png?v=103)](https://github.com/ellerbrock/open-source-badge/)
[![stable](http://badges.github.io/stability-badges/dist/stable.svg)](http://github.com/badges/stability-badges)

## Introduction
AlsaMedia includes player and recorder  realized with [Alsa](http://www.alsa-project.org/main/index.php/Main_Page).

## Installation
1. Alsa :
	```
	$ wget ftp://ftp.alsa-project.org/pub/lib/alsa-lib-1.1.3.tar.bz2
	$ tar -jxvf alsa-lib-1.1.3.tar.bz2
	$ cd alsa-lib-1.1.3
	$ ./configure
	$ make
	$ sudo make install
	```
	
2. AlsaMedia :
	```
	$ git clone git@github.com:AnSwErYWJ/AudioResamplerate.git
	```

## Usage
1. Compile :
	```
	$ make player
	$ make recorder
	```
	or 
	```
	$ make
	```
	
2. Run :
	```
	$ ./arecorder test.pcm
	$ ./aplayer test.pcm
	```

## Environment
+ Linux
+ POSIX C
+ Bash Shell

## Todo
- [ ] support mp3 decode.
- [ ] dynamic configuration.


## About me
[![forthebadge](http://forthebadge.com/images/badges/ages-20-30.svg)](http://forthebadge.com)
- WebSite：[http://www.answerywj.com](http://www.answerywj.com)
- Email：[yuanweijie1993@gmail.com](https://mail.google.com)
- GitHub：[AnSwErYWJ](https://github.com/AnSwErYWJ)
- Blog：[AnSwEr不是答案的专栏](http://blog.csdn.net/u011192270)
- Weibo：[@AnSwEr不是答案](http://weibo.com/1783591593)

## Copyright and License
[![GPL Licence](https://badges.frapsoft.com/os/gpl/gpl.svg?v=103)](https://opensource.org/licenses/GPL-3.0/)

    AudioResamplerate
    Copyright (C) 2016-2017  AnSwErYWJ

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
