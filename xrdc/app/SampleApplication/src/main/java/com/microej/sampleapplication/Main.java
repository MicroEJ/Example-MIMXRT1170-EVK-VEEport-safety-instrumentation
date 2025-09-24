/*
 * Java
 *
 * Copyright 2025 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */
package com.microej.sampleapplication;

import ej.util.Device;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Main class of the XRDC2/eDMA sample application.
 * <p>
 * This program demonstrates how the MICROEJ VEE interacts with
 * memory protected by XRDC2 (eXtended Resource Domain Controller).
 * The Java code periodically reads a boolean flag
 * to check whether the data has been corrupted by the eDMA
 * (enhanced Direct Memory Access).
 * </p>
 */
public class Main {

	/** Logger used for printing application messages. */
	private static final Logger LOGGER = Logger.getLogger("[Main]");

	/** Hexadecimal characters used for byte-to-hex conversion. */
	private static final char[] HEX_CHAR_ARRAY = "0123456789ABCDEF".toCharArray();

	/**
	 * Entry point of the sample application.
	 * <p>
	 * At startup, the device ID and architecture are retrieved.
	 * The program then continuously checks the VEE data:
	 * <ul>
	 *   <li>If the data is intact it logs a "Hello World from Java" message and the current flag value every second.</li>
	 *   <li>If the data becomes corrupted it logs a "Data corrupted" message and the final flag value then terminates.</li>
	 * </ul>
	 * </p>
	 *
	 * @param args command-line arguments (not used).
	 */
	public static void main(String[] args) {

		// Retrieve device information
		String id = bytesToHexString(Device.getId());
		String architecture = Device.getArchitecture();
		
		// Loop until the VEE data becomes corrupted
		while (!getVeeData()){
			LOGGER.info("MICROEJ VEE : Hello World from Java");
			LOGGER.info("MICROEJ VEE : Data value = " + getVeeData());

			try {
				// Sleep for 1 second
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				LOGGER.log(Level.SEVERE, "Thread interrupted", e);
				Thread.currentThread().interrupt();
			}
		}
		
		// At this point the data is corrupted
		LOGGER.info("MICROEJ VEE : Data corrupted"); 
		LOGGER.info("MICROEJ VEE : Data value = " + getVeeData());
	}

	/**
	 * Converts a byte array into a hexadecimal string.
	 * <p>
	 * Each byte is represented by two hexadecimal characters.
	 * </p>
	 *
	 * @param bytes the byte array to convert.
	 * @return a string representing the array in hexadecimal.
	 */
	private static String bytesToHexString(byte[] bytes) {
		char[] hexChars = new char[bytes.length * 2];
		for (int i = 0; i < bytes.length; i++) {
			int b = bytes[i] & 0xFF;
			hexChars[i * 2] = HEX_CHAR_ARRAY[b >>> 4];
			hexChars[i * 2 + 1] = HEX_CHAR_ARRAY[b & 0x0F];
		}
		return new String(hexChars);
	}

	/*******************************************************************************
     * Native method
     ******************************************************************************/

    /**
     * Retrieves the current VEE (Virtual Execution Environment) data.
     * <p>
     * The data is a boolean flag stored in memory at the address
     * {@code DEST_ADDR} which is declared in {@code edma_xrdc_demo.h}.
     * </p>
     *
     * @return {@code true} if the data is corrupted or {@code false} if intact.
     */
	public static native boolean getVeeData();
}
