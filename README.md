JetLee
------

Jet Lee - remake of ZX spectrum video game "Bruce Lee" (1984)

Created for igdc.ru contest #125

![screenshot](https://github.com/Uvadzucumi/JetLee/blob/master/data/scr/scr-01.png?raw=true)
![screenshot](https://github.com/Uvadzucumi/JetLee/blob/master/data/scr/scr-02.png?raw=true)
![screenshot](https://github.com/Uvadzucumi/JetLee/blob/master/data/scr/scr-03.png?raw=true)

https://github.com/Uvadzucumi/JetLee/blob/master/data/scr/scr-01.png?raw=true

Windows build (https://raw.github.com/Uvadzucumi/jetlee/master/bin/JetLee.zip)


command promt parmeters:
```
  JetLee.exe [command VALUE] [command VALUE] [command VALUE]
  commands:
    --hero VALUE - select Player hero. VALUE=number for 1 to 3.
        1 - Ninja.
        2 - Yamo.
        3 - JetLee (default).

    --scale VALUE - select pixels scale (VALUE - number for 1 to 20. default: 4)
    --level VALUE - difficulty level. VALUE=number for 1 to 3.
        1 - Karate Kid.
        2 - Bully (default).
        3 - Shaolin master.
```

Player control keys
```
  Left/Right arrows - Run to left or right
  Up arrow - jump.
  Down arrow - hide
  Left Control - Punch
  Run + Left Control - Kick
  c - game pause and free camera mode
```

Requirements: SDL2 (http://libsdl.org/), OpenGL (http://opengl.org), OpenAl (http://openal.org/), libogg, libvorbisfile.

Compile from sources
--------------------

Install Requirements in Debian/Ubuntu OS
```
sudo apt-get install g++ cmake libgl1-mesa-dev libsdl2-dev libopenal-dev libogg-dev libvorbis-dev
```
Compile
```
cmake .
make
```
