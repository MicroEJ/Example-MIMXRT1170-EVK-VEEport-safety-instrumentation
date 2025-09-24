// Copyright 2025 Copyright NXP
// Copyright MicroEJ Corp. This file has been modified by MicroEJ Corp.

plugins {
    id("com.microej.gradle.application") version "1.3.1"
}

group="com.microej"
version="1.0.0"

microej {
    applicationEntryPoint = "com.microej.sampleapplication.Main"

    // Uncomment to use "prod" architecture when using a VEE Port (defaults to "eval")
    architectureUsage = "eval"
}

dependencies {
    implementation("ej.api:edc:1.3.7")
    implementation("ej.api:bon:1.4.4")
    implementation("ej.api:device:1.2.0")
    implementation("ej.library.eclasspath:logging:1.2.1")

    //Uncomment the microejVee dependency to set the VEE Port or Kernel to use
    microejVee("com.nxp.vee.mimxrt1170:vee-port:3.1.0")
}

testing {
   suites {
      val test by getting(JvmTestSuite::class) {
         microej.useMicroejTestEngine(this)

         dependencies {
             implementation(project())
             implementation("ej.api:edc:1.3.7")
             implementation("ej.api:bon:1.4.4")
             implementation("ej.library.test:junit:1.12.0")
         }
      }
   }
}
