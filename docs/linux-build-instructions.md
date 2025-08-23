## Linux: Building from sources

This page is a step-by-step guide for building and installing in Linux. Please send a PR or send your patches and instructions for improving and fixing this building guide. Thank you.

Before starting, you should know that AGrilla source tree does not include makefiles. All makefiles are generated with CMake build system. CMake can generate different kinds of native build files for many systems and IDEs (e.g. Unix Makefiles, Eclipse CDT 4.0 project files, Visual Studio project files, and many other). The procedure described here is for installing from command line, using a Linux makefile.

### Step 1: Install prerequisites

AGrilla needs the wxWidgets libraries and they are normally already installed in Linux systems. In any case, there is no harm in trying to install them, just in case any is missing. 

#### Debian based distributions (e.g. Ubuntu)

Open a terminal window and type:

```
sudo apt-get install build-essential
sudo apt-get install cmake
sudo apt-get install libwxgtk3.2-dev
```

#### RPM based distributions (e.g. Fedora)

Open a terminal window and type:
```
dnf -y groupinstall "Development Tools"
dnf -y install cmake
dnf -y install wxGTK3-devel
```

### Step 2: Download source code and prepare to build

Lets assume you have a folder named 'projects'. We are going to create there a sub-folder for all AGrilla sources and download them from master repository at GitHub, either the latest code:

```
cd projects
git clone -b master --single-branch --depth 1 https://github.com/cecilios/agrilla.git
cd agrilla
```

or a specific Release (e.g. 5.4.2)

```
cd projects
git clone -b '5.4.2' --single-branch --depth 1 https://github.com/cecilios/agrilla.git
cd agrilla
```

Once the sources are downloaded, create a folder for building (e.g. `my-build`):

```
mkdir my-build
```

Now you will have the following folders structure:

```
    projects
       +-- agrilla
               +-- src
               +-- include
               +-- ...
               +-- my-build
```

### Step 3: Generate the makefiles, build and install

Run cmake to check for the required build dependencies and to generate the makefile. If cmake fails, install the missing dependencies and repeat this step until the command runs successfully:

```
cd my-build
cmake -G "Unix Makefiles" ../
```

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

