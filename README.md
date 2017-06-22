---
documentclass: physycomen
title:  "distance_stats"
author: "Fabbri, Sinigardi"
---

<a href="http://www.physycom.unibo.it"> 
<div class="image">
<img src="https://cdn.rawgit.com/physycom/templates/697b327d/logo_unibo.png" width="90" height="90" alt="© Physics of Complex Systems Laboratory - Physics and Astronomy Department - University of Bologna"> 
</div>
</a>
<a href="https://travis-ci.org/physycom/distance_stats"> 
<div class="image">
<img src="https://travis-ci.org/physycom/distance_stats.svg?branch=master" width="90" height="20" alt="Build Status"> 
</div>
</a>
<a href="https://ci.appveyor.com/project/cenit/distance-stats"> 
<div class="image">
<img src="https://ci.appveyor.com/api/projects/status/8u6ovumgy7pr4jyd?svg=true" width="90" height="20" alt="Build Status"> 
</div>
</a>


### Installation
**CMake** and a **C++11** compatible compiler are required. To build the executable, clone the repo and then type  
```
mkdir build ; cd build ; cmake .. ; cmake --build . --target install
```
With CMake you can also deploy projects for the most common IDEs.  

Contains [jsoncons](https://github.com/danielaparker/jsoncons) as a git submodule.   


### Synopsis
```
distance_stats.exe -i input.json -o output_file
trip_stats.exe input1.json input2.json ...
global_stats.exe input1.TRIPSTATS.json input2.TRIPSTATS.json ...
```


