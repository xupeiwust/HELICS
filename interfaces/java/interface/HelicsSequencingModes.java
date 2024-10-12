/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.2.1
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.java.helics;

/**
 *  enumeration of sequencing modes for queries and commands<br>
 * fast is the default, meaning the query travels along priority channels and takes precedence of over<br>
 * existing messages; ordered means it follows normal priority patterns and will be ordered along with<br>
 * existing messages
 */
public final class HelicsSequencingModes {
  /**
   *  sequencing mode to operate on priority channels
   */
  public final static HelicsSequencingModes HELICS_SEQUENCING_MODE_FAST = new HelicsSequencingModes("HELICS_SEQUENCING_MODE_FAST", helicsJNI.HELICS_SEQUENCING_MODE_FAST_get());
  /**
   *  sequencing mode to operate on the normal channels
   */
  public final static HelicsSequencingModes HELICS_SEQUENCING_MODE_ORDERED = new HelicsSequencingModes("HELICS_SEQUENCING_MODE_ORDERED", helicsJNI.HELICS_SEQUENCING_MODE_ORDERED_get());
  /**
   *  select the default channel
   */
  public final static HelicsSequencingModes HELICS_SEQUENCING_MODE_DEFAULT = new HelicsSequencingModes("HELICS_SEQUENCING_MODE_DEFAULT", helicsJNI.HELICS_SEQUENCING_MODE_DEFAULT_get());

  public final int swigValue() {
    return swigValue;
  }

  public String toString() {
    return swigName;
  }

  public static HelicsSequencingModes swigToEnum(int swigValue) {
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (int i = 0; i < swigValues.length; i++)
      if (swigValues[i].swigValue == swigValue)
        return swigValues[i];
    throw new IllegalArgumentException("No enum " + HelicsSequencingModes.class + " with value " + swigValue);
  }

  private HelicsSequencingModes(String swigName) {
    this.swigName = swigName;
    this.swigValue = swigNext++;
  }

  private HelicsSequencingModes(String swigName, int swigValue) {
    this.swigName = swigName;
    this.swigValue = swigValue;
    swigNext = swigValue+1;
  }

  private HelicsSequencingModes(String swigName, HelicsSequencingModes swigEnum) {
    this.swigName = swigName;
    this.swigValue = swigEnum.swigValue;
    swigNext = this.swigValue+1;
  }

  private static HelicsSequencingModes[] swigValues = { HELICS_SEQUENCING_MODE_FAST, HELICS_SEQUENCING_MODE_ORDERED, HELICS_SEQUENCING_MODE_DEFAULT };
  private static int swigNext = 0;
  private final int swigValue;
  private final String swigName;
}

