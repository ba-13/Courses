# Assignment 1

Create a 2D scene that resembles the provided video closely.

To view this, open [index.html](./index.html) in your browser that supports `WebGL2`

## File Structure

- [`_glsl.js`](./_glsl.js) contains GLSL code as strings
- [`_glInit.js`](./_glInit.js) contains boilerplate functions for GLSL compilation and WebGL initialisation
- [`_utils.js`](./_utils.js) contains utility functions
- [`glMatrix-0.9.5.min.js`](./glMatrix-0.9.5.min.js) library for matrix manipulations
- [`_shapes.js`](./_shapes.js) basic shapes class definition
- [`index.js`](./index.js) entry point of javascript, contains drawScene function
- [`jsconfig.json`](./jsconfig.json) used by VSCode to consider all above files within a single project
- [`index.html`](./index.html) HTML file including all above js files, contains canvas

## Debugging

Press `Ctrl` and hover over a function/variable/keyword to check out it's definition across all files in VSCode.
