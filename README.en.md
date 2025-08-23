# A tool to overlay a grid over any screen region, providing a guide to help you in your composition and drawing.

![AGrilla](docs/images/grid-adjusted.jpg?raw=true)


AGrilla helps artists to create accurate and detailed drawings using the grid method. It is also of help in composition when using the rule of thirds or the golden ratio. See, for instance, this website
 <a href="https://letsdrawtoday.com/how-to-use-the-grid-method/" target="_blank">https://letsdrawtoday.com/how-to-use-the-grid-method/</a>.


## License
AGrilla is distributed under the MIT license, a permissive open source license to allow AGrilla to be used in any projects, whether open source or proprietary. This license is a GPL compatible license, so you can use AGrilla in GPL licensed projects.

For details, see file [LICENSE](LICENSE).



## How AGrilla can help you:

**In composition:**
* Adjust the grid dimensions to the size of the paper or canvas you'll be using and move it over the reference image to find the framing you like best.
![In composition](docs/images/grid-golden-lines.jpg?raw=true)

* Choose the grid with the golden ratio lines to find the locations to place important focal points, such as the main subject or other elements of interest.

**As a help for precise drawing:**
* Divide the image in halfs or thirds to quickly locate the center of the image or as an aid in composition, to use the "rule of thidrs".
* Use the grid as a drawing aid. Choose a higher number of grid divisions to accurately draw the most complicated parts of the model.
![More divisions](docs/images/grid-more-divisions.jpg?raw=true)




## Features

* **Customizable grid:**
Choose the size and divisions you want to adapt it to the needs of your project.
![Customize](docs/images/grid-options.jpg?raw=true)


* **Adjust to your needs:**
Easily and effortlessly adjust the color and thickness of the grid lines to suit your preferences and visual needs, thanks to its simple and intuitive interface that doesn't require studying any manual. You can also choose the type of grid: regular or based on the golden ratio.

* **Adjust the grid to the canvas size:**
Specify the size (width and height) of the paper or canvas you'll be using. The grid will immediately adopt the same aspect ratio as your canvas. And when you expand or decrease the grid by dragging its edges with the mouse, the chosen aspect ratio will automatically be maintained.


## Installation and Building

I've just started publishing the project, and for now, it can only be installed by compiling and building AGrilla. But it's very simple and quick.

### Step 1: Requirements
- CMake >= 3.10
- A C/C++ compatible compiler
- The wxWidgets librariee.

These requirement are normally already installed in Linux systems.


### Step 2: Download source code

Lets assume you have a folder named `projects`. Create there a sub-folder for all agrilla sources and download them from master repository at GitHub:

```
cd projects
git clone -b main https://github.com/cecilios/agrilla.git
cd agrilla
```

### Step 3: Generate the makefiles

Once the sources are downloaded, create a folder for building (e.g. `my-build`):

```
mkdir my-build
```

Now you will have the following folders structure:

```
    projects
        ├── agrilla
               ├── src
               ├── include
               ├── my-build
               ├──  ...
```

Now run cmake to check to generate the makefiles and to check for the required dependencies. If cmake fails, install the missing dependencies and repeat this step until the command runs successfully:

```
cd my-build
cmake -G "Unix Makefiles" ../
```

### Step 4: Building the Project


Now you can compile the sources and build. It is very fast:

```
make
```

If previous command completes successfully you can test the program:

```
./agrilla
```

Or you can install and run the program:

```
sudo make install
agrilla
```

Your comments and fixes to these instructions are welcome. Thank you.



## Contributing

If you would like to contribute or to join this project, you are welcome. The project has no commercial aim and all contributions are welcomed!

By contributing to the AGrilla repository through pull-requests, comments, or otherwise, the contributor releases their content to the
license and copyright terms as described in the [LICENSE](LICENSE).
Each contributor holds copyright over their contributions to AGrilla.

Read [Contributing](CONTRIBUTING.md) for information about how to contribute.



## Help, bug reports and feedback

Feel free to ask any question or to send feedback and suggestions for improvements. Go to the AGrilla [issues page](https://github.com/lenmus/AGrilla/issues) and create a new issue.

Thanks a lot for helping make AGrilla better!






