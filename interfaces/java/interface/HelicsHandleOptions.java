/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.java.helics;

public final class HelicsHandleOptions {
  public final static HelicsHandleOptions HELICS_HANDLE_OPTION_CONNECTION_REQUIRED = new HelicsHandleOptions("HELICS_HANDLE_OPTION_CONNECTION_REQUIRED", helicsJNI.HELICS_HANDLE_OPTION_CONNECTION_REQUIRED_get());
  public final static HelicsHandleOptions HELICS_HANDLE_OPTION_CONNECTION_OPTIONAL = new HelicsHandleOptions("HELICS_HANDLE_OPTION_CONNECTION_OPTIONAL", helicsJNI.HELICS_HANDLE_OPTION_CONNECTION_OPTIONAL_get());
  public final static HelicsHandleOptions HELICS_HANDLE_OPTION_SINGLE_CONNECTION_ONLY = new HelicsHandleOptions("HELICS_HANDLE_OPTION_SINGLE_CONNECTION_ONLY", helicsJNI.HELICS_HANDLE_OPTION_SINGLE_CONNECTION_ONLY_get());
  public final static HelicsHandleOptions HELICS_HANDLE_OPTION_MULTIPLE_CONNECTIONS_ALLOWED = new HelicsHandleOptions("HELICS_HANDLE_OPTION_MULTIPLE_CONNECTIONS_ALLOWED", helicsJNI.HELICS_HANDLE_OPTION_MULTIPLE_CONNECTIONS_ALLOWED_get());
  public final static HelicsHandleOptions HELICS_HANDLE_OPTION_BUFFER_DATA = new HelicsHandleOptions("HELICS_HANDLE_OPTION_BUFFER_DATA", helicsJNI.HELICS_HANDLE_OPTION_BUFFER_DATA_get());
  public final static HelicsHandleOptions HELICS_HANDLE_OPTION_STRICT_TYPE_CHECKING = new HelicsHandleOptions("HELICS_HANDLE_OPTION_STRICT_TYPE_CHECKING", helicsJNI.HELICS_HANDLE_OPTION_STRICT_TYPE_CHECKING_get());
  public final static HelicsHandleOptions HELICS_HANDLE_OPTION_IGNORE_UNIT_MISMATCH = new HelicsHandleOptions("HELICS_HANDLE_OPTION_IGNORE_UNIT_MISMATCH", helicsJNI.HELICS_HANDLE_OPTION_IGNORE_UNIT_MISMATCH_get());
  public final static HelicsHandleOptions HELICS_HANDLE_OPTION_ONLY_TRANSMIT_ON_CHANGE = new HelicsHandleOptions("HELICS_HANDLE_OPTION_ONLY_TRANSMIT_ON_CHANGE", helicsJNI.HELICS_HANDLE_OPTION_ONLY_TRANSMIT_ON_CHANGE_get());
  public final static HelicsHandleOptions HELICS_HANDLE_OPTION_ONLY_UPDATE_ON_CHANGE = new HelicsHandleOptions("HELICS_HANDLE_OPTION_ONLY_UPDATE_ON_CHANGE", helicsJNI.HELICS_HANDLE_OPTION_ONLY_UPDATE_ON_CHANGE_get());
  public final static HelicsHandleOptions HELICS_HANDLE_OPTION_IGNORE_INTERRUPTS = new HelicsHandleOptions("HELICS_HANDLE_OPTION_IGNORE_INTERRUPTS", helicsJNI.HELICS_HANDLE_OPTION_IGNORE_INTERRUPTS_get());
  public final static HelicsHandleOptions HELICS_HANDLE_OPTION_MULTI_INPUT_HANDLING_METHOD = new HelicsHandleOptions("HELICS_HANDLE_OPTION_MULTI_INPUT_HANDLING_METHOD", helicsJNI.HELICS_HANDLE_OPTION_MULTI_INPUT_HANDLING_METHOD_get());
  public final static HelicsHandleOptions HELICS_HANDLE_OPTION_INPUT_PRIORITY_LOCATION = new HelicsHandleOptions("HELICS_HANDLE_OPTION_INPUT_PRIORITY_LOCATION", helicsJNI.HELICS_HANDLE_OPTION_INPUT_PRIORITY_LOCATION_get());
  public final static HelicsHandleOptions HELICS_HANDLE_OPTION_CLEAR_PRIORITY_LIST = new HelicsHandleOptions("HELICS_HANDLE_OPTION_CLEAR_PRIORITY_LIST", helicsJNI.HELICS_HANDLE_OPTION_CLEAR_PRIORITY_LIST_get());
  public final static HelicsHandleOptions HELICS_HANDLE_OPTION_CONNECTIONS = new HelicsHandleOptions("HELICS_HANDLE_OPTION_CONNECTIONS", helicsJNI.HELICS_HANDLE_OPTION_CONNECTIONS_get());

  public final int swigValue() {
    return swigValue;
  }

  public String toString() {
    return swigName;
  }

  public static HelicsHandleOptions swigToEnum(int swigValue) {
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (int i = 0; i < swigValues.length; i++)
      if (swigValues[i].swigValue == swigValue)
        return swigValues[i];
    throw new IllegalArgumentException("No enum " + HelicsHandleOptions.class + " with value " + swigValue);
  }

  private HelicsHandleOptions(String swigName) {
    this.swigName = swigName;
    this.swigValue = swigNext++;
  }

  private HelicsHandleOptions(String swigName, int swigValue) {
    this.swigName = swigName;
    this.swigValue = swigValue;
    swigNext = swigValue+1;
  }

  private HelicsHandleOptions(String swigName, HelicsHandleOptions swigEnum) {
    this.swigName = swigName;
    this.swigValue = swigEnum.swigValue;
    swigNext = this.swigValue+1;
  }

  private static HelicsHandleOptions[] swigValues = { HELICS_HANDLE_OPTION_CONNECTION_REQUIRED, HELICS_HANDLE_OPTION_CONNECTION_OPTIONAL, HELICS_HANDLE_OPTION_SINGLE_CONNECTION_ONLY, HELICS_HANDLE_OPTION_MULTIPLE_CONNECTIONS_ALLOWED, HELICS_HANDLE_OPTION_BUFFER_DATA, HELICS_HANDLE_OPTION_STRICT_TYPE_CHECKING, HELICS_HANDLE_OPTION_IGNORE_UNIT_MISMATCH, HELICS_HANDLE_OPTION_ONLY_TRANSMIT_ON_CHANGE, HELICS_HANDLE_OPTION_ONLY_UPDATE_ON_CHANGE, HELICS_HANDLE_OPTION_IGNORE_INTERRUPTS, HELICS_HANDLE_OPTION_MULTI_INPUT_HANDLING_METHOD, HELICS_HANDLE_OPTION_INPUT_PRIORITY_LOCATION, HELICS_HANDLE_OPTION_CLEAR_PRIORITY_LIST, HELICS_HANDLE_OPTION_CONNECTIONS };
  private static int swigNext = 0;
  private final int swigValue;
  private final String swigName;
}

