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
- [ ] pause,resume
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
BSD 2-Clause License

Copyright (c) 2017-2018, AnSwErYWJ(Weijie Yuan),yuanweijie1993@gmail.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
