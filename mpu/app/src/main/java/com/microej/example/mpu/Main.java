/*
 * Java
 *
 * Copyright 2025 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */
package com.microej.example.mpu;


public class Main {

    // Defines string for logs
    public static final String HELLO_WORLD = "MICROEJ VEE : Hello World from Java";
    public static final String DATA_CORRUPTED = "MICROEJ VEE : Data corrupted";

    public static void main(String[] args) {

        // Prints "MICROEJ VEE : Hello World from Java" 
        // while the MICROEJ VEE data is not corrupted
        while(!getVeeData()){
            System.out.println(HELLO_WORLD);
            try {
                Thread.sleep(1000);
            } catch(InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        
        System.out.println(DATA_CORRUPTED);

        // Reset the MICROEJ VEE data to it's default state 
        resetVeeData();

        // Prints "MICROEJ VEE : Hello World from Java" 
        // while the MICROEJ VEE data is not corrupted
        while(!getVeeData()){
            System.out.println(HELLO_WORLD);
            try {
                Thread.sleep(1000);
            } catch(InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }

        System.out.println(DATA_CORRUPTED);
    }
    
    // Natives methods :

    /**
     * Retrieves the current VEE (Virtual Execution Environment) data.
     * Data is defined as the "vee_data" variable in the main.c
     *
     * @return {@code true} or {@code false} depending of the state of the "vee_data" variable.
     */
    public static native boolean getVeeData();

    /**
     * Resets the VEE (Virtual Execution Environment) data ("vee_data" variable defined in the main.c) 
     * to its default value.
     *
     * @return void
     */
    public static native void resetVeeData();
}
