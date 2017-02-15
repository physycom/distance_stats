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


### Purpose


### Installation
**make** and a **C++11** compatible compiler are required. Clone the repo and type ``make``, it should be enough in most cases to build the two executables!   
There's also a **VS2015** solution avalaible.   
Contains [jsoncons](https://github.com/danielaparker/jsoncons) as a git submodule.   


### Usage
```
distance_stats.exe -i input.json -o output_file
```
where *input.json* must be an existing and valid .json file and *output_file* must be accessible.


