
.. image:: https://shields.microej.com/endpoint?url=https://repository.microej.com/packages/badges/sdk_6.0.json
   :alt: sdk_6.0 badge
   :align: left

.. class:: center

Overview
========

This project gathers examples demonstrating ways of using various safety instrumentation to protect the VEE runtime.

Each sample includes a ``README.md`` file that provides instructions on how to run it.

Details
=======

Memory Protection Unit (MPU)
~~~~
The Memory Protection Unit (MPU) sample run on a Zephyr VEE Port. It demonstrates how to configure the MPU to protect the MICROEJ VEE thread from memory corruption of a third party code. 
It also shows a way to share data between the MICROEJ VEE and the third-party code. 

See the `mpu folder <mpu>`_ for this sample.

Extended Resource Domain Controller (XRDC)
~~~~
Extended Resource Domain Controller (XRDC) samples runs on a FreeRTOS VEE Port. It demonstrates how to configure the XRDC to protect the MICROEJ VEE thread from memory corruption of a third-party code making an Enhanced Direct Memory Access (eDMA) transfer. 

See the `xrdc folder <xrdc>`_ for this sample.

--------------

.. ReStructuredText
.. Copyright 2025 MicroEJ Corp. All rights reserved.
.. Use of this source code is governed by a BSD-style license that can be found with this software.

