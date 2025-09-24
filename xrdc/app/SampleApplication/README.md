# Overview

This is a Java application used by the [i.MXRT1170-EVK VEE Port](https://gitlab.cross/P0293_MIMXRT1170-EVK/P0293_MIMXRT1170-EVK) to execute the eDMA/XRDC sample. 

## Run on Device

This section outlines the process of building the entire project, including the `bsp` and the application.

In IntelliJ IDEA or Android Studio:
- Open the Gradle tool window by clicking on the elephant on the right side.
- Expand the `sample-application` list.
- Expand the `Tasks` list.
- From the `Tasks` list, expand the `microej` list.
- Double-click on `runOnDevice`.
- The executable has been built, and the device has been flashed. Use the appropriate tool to retrieve the execution traces.

Otherwise, go inside the `xrdc/app/SampleApplication` folder by using a terminal and run the following command (or click the Play button next to the line below when opening this README in IntelliJ IDEA):
```
./gradlew runOnDevice
```

Alternative ways to run the on device are described in the [Run on Device](https://docs.microej.com/en/latest/SDK6UserGuide/runOnDevice.html) documentation.

---
_Markdown_  
_Copyright 2024-2025 MicroEJ Corp. All rights reserved._  
_Use of this source code is governed by a BSD-style license that can be found with this software._ 