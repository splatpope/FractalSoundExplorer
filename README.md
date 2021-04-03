# Fractal Sound Explorer
Explore fractals in an audio-visual sandbox (development repo for cross-platform, improved version)

Download original executable on HackerPoet's itch.io page:
https://codeparade.itch.io/fractal-sound-explorer

YouTube video explanation:
https://youtu.be/GiAj9WW1OfQ

![](https://img.itch.zone/aW1hZ2UvOTM1NzMzLzUzMTU0MzEucG5n/original/ay7ju0.png)

Notes
---------------
The fractals are designed to run in real time on a GPU.  If the program is going too slow, you can simply shrink the size of the window to make it run faster.  The rendering is also designed to increase the image quality over time.  So keep still for a bit before taking screenshots to get higher quality.

Audio is currently somewhat supported.

Dependencies
---------------
* libconfig++
* SFML
* PortAudio

Building
---------------
* Linux : on the project root :  

```
git submodule update Gamma
git submodule init Gamma
cd Gamma && make && cd ..
cmake . -B./build
cd build && make
```
* Windows : tbd
* Mac : tbd, should be the same as windows

Controls
---------------
* H - Toggle Help Menu
* Left Mouse - Click or drag to hear orbits -- currently not supported
* Middle Mouse - Drag to pan view
* Right Mouse - Stop orbit and sound
* Scroll Wheel - Zoom in and out
* D - Toggle Audio Dampening -- currently not supported
* C - Toggle Color
* F11 - Toggle Fullscreen
* S - Save Snapshot
* R - Reset View
* J - Hold down, move mouse, and release to make Julia sets. Press again to switch back.
* 1 - Mandelbrot Set
* 2 - Burning Ship
* 3 - Feather Fractal
* 4 - SFX Fractal
* 5 - Hénon Map
* 6 - Duffing Map
* 7 - Ikeda Map
* 8 - Chirikov Map
