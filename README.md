# ScreenRes

This commandline-tool (screenres.exe) can be used on a Windows-Computer to 

- display the available screen-resolutions,
- display the current screen-resolution, and
- set the screen-resolution.

This tool is written in C++ and calls `EnumDisplaySettingsA` and 
`ChangeDisplaySettingsA` from the Win32-API.

ScreenRes has been written to be a replacement for 
[Qres](http://qres.sourceforge.net/). ScreenRes uses the same syntax for 
command-line-parameters, and it produces the same output as Qres. 

The main difference is, that there is no need to install ScreenRes, you can 
just use screenres.exe. Note, that there are also portable QRes.exe-files out 
there in the Internet. I tried one, but it didn't work for one of my 
Windows-machines. Also, I wasn't able to find a source-code of this 
QRes.exe-file, so I didn't trust it.

Another difference is, that ScreenRes first clones all attaches displays, 
before it performs another action (this is what i need).

## Usage

Set the screen-resolution to nnn x mmm pixels:
```
screenres.exe /X:nnn /Y:mmm
```

Display the current screen-resolution:
```
screenres.exe /S
```

List all possible screen-resolutions:
```
screenres.exe /L
```

Print usage-information:
```
screenres.exe /H
```
or
```
screenres.exe /?
```

An additional /V command-line-argument is allowed in each case. If this argument is present, NO version-information is printed at the beginning of the output.
