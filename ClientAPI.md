# SnesGame API

## High Level Overview

The only line of code in the `main()` method defined by an executable using the SnesGame runtime should be to call `playSnesGame()`, passing function pointers to the various lifecycle methods.

### Init lifecycle method

The init callback is called once, at application start, after the SnesGame runtime has been initialized and before the game loop starts. During this callback, the following is available:
* Utilities to read files from disk
* Initialization of runtime 'ROM' data

### Deinit lifecycle method
The deinit callback is called once, during application shutdown. If the game has any sort of final shutdown (such as saving high scores, recording play time, etc), it should be done during this callback.

## Game loop

The SnesGame game loop runs at 50 fps. The loop does steps in the following order:
* update
* render
* wait

### Update lifecycle method

The update callback is called once per game loop iteration, and is responsible for advancing the game state by one tick. During this method, the following is available:
* polling the current state of the controllers
* adjusting audio channel parameters

### Render lifecycle method

The render callback is called once per game loop iteration, and is responsible for configuring the render state of the current tick.

## Graphical Capabilities

The SnesGame runtime is designed to produce visuals that resemble the SNES in appearance (hence the name), although in reality the capabilities of the runtime fall somewhere between the NES and SNES in terms of features. It has a logical screen with a width and height of 248 'dots', and a coordinate system with (0,0) in the top left of the screen, a horizontal X axis and a vertical Y axis. Depending on the actual size of the game window on the user's computer, a dot may be a single physical pixel, two pixels square, three pixels square, etc.

Two different graphical objects are supported: four 'backdrops' and 128 'actors'. These objects are partitioned into containers called 'layers'. Each layer contains a single backdrop; the actors may be assigned to layers on a frame by frame basis. Both of these objects are composed of 'glpyhs', which are 4bpp indexed images, eight dots tall and wide, rendered to the logical screen.

The color indices for glyphs are called 'brushes'. A 'brush list' contains 256 brushes. Brush lists are initialized during the init lifecycle method and become read-only after that point. Brush lists can be used to render both backdrops and actors. No more than four brush lists may be used per render lifecycle method call by backdrops, and no more than four may be used per call by actors, although backdrops and actors may use the same brush list in a single call.

The colors for glyphs are called 'palettes'. Each palette specifies the sixteen 'colors' that are combined with a brush to make a glyph. Individual colors in a palette have a color space with five bits each for red, green, and blue, and may be either opaque, or 50% transparent. Palettes are not read-only; they may be modified each frame to produce graphical effects such as fire, running water, blinking lights, etc. Backdrops and actors use separate palettes. No more than eight palettes may be used per render lifecycle method call by backdrops, and no more than eight may be used per call by actors.

Glyphs have a few additional properties that may be specified:
* masked (the color with index zero is not draw to the screen)
* horizontal flip (the glyph is drawn right-to-left instead of left-to-right)
* vertical flip (the glyph is drawn bottom-to-top instead of top-to-bottom)
  * for flips, the position of the glyph on the screen does not change, only the dots of the image

Collectively, a brush, palette, mask flag, horizontal flip flag, and vertial flip flag are referred to as a 'stroke'. All that is then needed to turn a stroke into a glyph on the screen is to know the X and Y coordinates to place the resulting glyph.

Backdrops are 256 by 256 dot images, created from 32 by 32 glyphs. The stroke for each glyph may be individually specified. The glyphs in the backdrop are given coordinates, with (0, 0) being the glyph drawn at the top-left of the backdrop, and a horizontal X axis and vertical Y axis. Backdrops have a few additional properties that may be specified:
* visible (whether to draw the backdrop at all)
* offsetX/offsetY (backdrop are eight dots larger than the screen, so these values in the range 0-7 allow it to be scrolled to postion)
* topmost (if set, the backdrop is drawn after the actors in the layer, so it appears 'on top' of them in the render order)

Actors are smaller images, which can be varying sizes. Actors can be positioned anywhere on the screen with an X and Y position in the range of -256 to 256 (parts of the actor which are outside of the screen edge will not be displayed). Actors may be visible or not; if not, they are not rendered. Actors may also be partitioned into different layers.

Actors may assume the following sizes:
* 8 by 8 dots (1 by 1 glyph)
* 16 by 16 dots (2 by 1 glyph)
* 32 by 32 dots (4 by 1 glyph)
* 64 by 64 dots (8 by 1 glyph)
* 16 by 32 dots (2 by 4 glyph)
* 32 by 64 dots (4 by 8 glyph)
* 32 by 16 dots (4 by 2 glyph)
* 64 by 32 dots (8 by 4 glyph)

Actors are rendered by specifying a stroke for the top-left glyph. The same stroke settings are used for the rest of the glyphs in the actor, except for the brush index, which increses by one for each horizontal glyph, and by sixteen for each vertical glyph. (If the index calculated ever exceeds 256, it is calculated as modulo 256). Additionally, if the horizontal or vertical flip flags are specified for an actor, in addition to the individual glyphs being flipped, the order that the glyphs are rendered is also flipped (visually, the whole actor is flipped).

Finally, each layer can define mask regions around the edges of the screen. Each edge has its own mask thickness. When rendering glyphs from backdrops or actors in a layer, if the resulting dot would be within a mask region, it is not rendered. The mask regions can also be 'inverted' so that dots are rendered only when within the region, instead of without. Masks can be up to 128 dots in thickness.

## Audio Capabilities

The SnesGame runtime plays at 48 KHz. It supports up to eight simultaneous audio channels. Each audio channel can play square wave notes with the following audio properties:

* Length: the number of samples to play before silencing the channel. Values are in the range of 0-65534. Use value '65535' for a note that does not end.
* Period: the length in samples of _half_ of a note's cycle (thus, the note's frequency will be 24000 divided by this value). Values are in the range of 16-24000.
* Period shift: a note can be played with a constant period, or it can be set to increase, or decrease, the period after each _half_ of a note's cycle. Shifts are defined with a low period, a high period, a direction (increasing or decreasing), a count (in half-cycles) between changes to the currently-playing period, and an edge behaviour: silence the channel, reverse the direction of the shift, or jump from one edge of the range to the other.
* Volume: each channel has eight different volumes at which it can be playing.
* Volume shift: a note can be played with a variable volume. Shifts are defined a direction (increasing or decreasing), a count (in half-cycles) between changes to the currently-playing period, and an edge behaviour: silence the channel, reverse the direction of the shift, or jump from one edge of the range to the other.

Note | Half-period | Effective Frequency | Relative Error
--- | --- | ---| ---
A0 | 873 | 27.49 | -0.03%
A♯0 | 824 | 29.13 | -0.03%
B0 | 778 | 30.85 | -0.06%
C1 | 734 | 32.70 | -0.02%
C♯1 | 693 | 34.63 | -0.05%
D1 | 654 | 36.70 | -0.03%
D♯1 | 617 | 38.90 | 0.02%
E1 | 582 | 41.24 | 0.08%
F1 | 550 | 43.64 | -0.04%
F♯1 | 519 | 46.24 | -0.01%
G1 | 490 | 48.98 | -0.04%
G♯1 | 462 | 51.95 | 0.07%
A1 | 436 | 55.05 | 0.08%
A♯1 | 412 | 58.25 | -0.03%
B1 | 389 | 61.70 | -0.06%
C2 | 367 | 65.40 | -0.02%
C♯2 | 346 | 69.36 | 0.10%
D2 | 327 | 73.39 | -0.03%
D♯2 | 309 | 77.67 | -0.14%
E2 | 291 | 82.47 | 0.08%
F2 | 275 | 87.27 | -0.04%
F♯2 | 259 | 92.66 | 0.18%
G2 | 245 | 97.96 | -0.04%
G♯2 | 231 | 103.90 | 0.07%
A2 | 218 | 110.09 | 0.08%
A♯2 | 206 | 116.50 | -0.03%
B2 | 194 | 123.71 | 0.19%
C3 | 183 | 131.15 | 0.26%
C♯3 | 173 | 138.73 | 0.10%
D3 | 163 | 147.24 | 0.28%
D♯3 | 154 | 155.84 | 0.18%
E3 | 146 | 164.38 | -0.26%
F3 | 137 | 175.18 | 0.33%
F♯3 | 130 | 184.62 | -0.21%
G3 | 122 | 196.72 | 0.37%
G♯3 | 116 | 206.90 | -0.36%
A3 | 109 | 220.18 | 0.08%
A♯3 | 103 | 233.01 | -0.03%
B3 | 97 | 247.42 | 0.19%
C4 | 92 | 260.87 | -0.29%
C♯4 | 87 | 275.86 | -0.48%
D4 | 82 | 292.68 | -0.33%
D♯4 | 77 | 311.69 | 0.18%
E4 | 73 | 328.77 | -0.26%
F4 | 69 | 347.83 | -0.40%
F♯4 | 65 | 369.23 | -0.21%
G4 | 61 | 393.44 | 0.37%
G♯4 | 58 | 413.79 | -0.36%
A4 | 55 | 436.36 | -0.83%
A♯4 | 51 | 470.59 | 0.95%
B4 | 49 | 489.80 | -0.83%
C4 | 46 | 521.74 | -0.29%
C♯5 | 43 | 558.14 | 0.68%
D5 | 41 | 585.37 | -0.33%
D♯5 | 39 | 615.38 | -1.10%
E5 | 36 | 666.67 | 1.12%
F5 | 34 | 705.88 | 1.06%
F♯5 | 32 | 750.00 | 1.35%
G5 | 31 | 774.19 | -1.25%
G♯5 | 29 | 827.59 | -0.36%
A5 | 27 | 888.89 | 1.01%
A♯5 | 26 | 923.08 | -0.99%
B5 | 24 | 1000.00 | 1.24%
C5 | 23 | 1043.48 | -0.29%
C♯6 | 22 | 1090.91 | -1.61%
D6 | 20 | 1200.00 | 2.16%
D♯6 | 19 | 1263.16 | 1.50%
E6 | 18 | 1333.33 | 1.12%
F6 | 17 | 1411.76 | 1.06%
F♯6 | 16 | 1500.00 | 1.35%

## Controllers

The SNES game runtime abstracts physical input devices to a unified 'logical' controller abstraction. This controller has the same layout as an SNES controller: a D-pad, L and R shoulder buttons, select, start, and four buttons labelled X, A, B and Y (clockwise from the top).

Each button on the logical controller can be in one of two states: in (being held down), or out (not being held down). If the button is currently in, but was out on the last update call, it is 'pressed' in addition to being in; similarly, if the button is currently out, but was in on the last update call, it is 'released' in addition to being out.

The state of each logical button is captured just before the call to the update lifecycle method. As a reasult, if the player manages to press and release the controller button between two update calls, the runtime will not register any change to the logical controller. 

## Recording and Playback

TBD