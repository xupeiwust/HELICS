/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.3.0
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.java.helics;

/**
 *  enumeration of return values from the C interface functions
 */
public final class HelicsErrorTypes {
  /**
   *  global fatal error for federation 
   */
  public final static HelicsErrorTypes HELICS_ERROR_FATAL = new HelicsErrorTypes("HELICS_ERROR_FATAL", helicsJNI.HELICS_ERROR_FATAL_get());
  /**
   *  an unknown non-helics error was produced 
   */
  public final static HelicsErrorTypes HELICS_ERROR_EXTERNAL_TYPE = new HelicsErrorTypes("HELICS_ERROR_EXTERNAL_TYPE", helicsJNI.HELICS_ERROR_EXTERNAL_TYPE_get());
  /**
   *  the function produced a helics error of some other type 
   */
  public final static HelicsErrorTypes HELICS_ERROR_OTHER = new HelicsErrorTypes("HELICS_ERROR_OTHER", helicsJNI.HELICS_ERROR_OTHER_get());
  /**
   *  user code generated exception 
   */
  public final static HelicsErrorTypes HELICS_USER_EXCEPTION = new HelicsErrorTypes("HELICS_USER_EXCEPTION", helicsJNI.HELICS_USER_EXCEPTION_get());
  /**
   *  insufficient space is available to store requested data 
   */
  public final static HelicsErrorTypes HELICS_ERROR_INSUFFICIENT_SPACE = new HelicsErrorTypes("HELICS_ERROR_INSUFFICIENT_SPACE", helicsJNI.HELICS_ERROR_INSUFFICIENT_SPACE_get());
  /**
   *  the function execution has failed 
   */
  public final static HelicsErrorTypes HELICS_ERROR_EXECUTION_FAILURE = new HelicsErrorTypes("HELICS_ERROR_EXECUTION_FAILURE", helicsJNI.HELICS_ERROR_EXECUTION_FAILURE_get());
  /**
   *  the call made was invalid in the present state of the calling object 
   */
  public final static HelicsErrorTypes HELICS_ERROR_INVALID_FUNCTION_CALL = new HelicsErrorTypes("HELICS_ERROR_INVALID_FUNCTION_CALL", helicsJNI.HELICS_ERROR_INVALID_FUNCTION_CALL_get());
  /**
   *  error issued when an invalid state transition occurred 
   */
  public final static HelicsErrorTypes HELICS_ERROR_INVALID_STATE_TRANSITION = new HelicsErrorTypes("HELICS_ERROR_INVALID_STATE_TRANSITION", helicsJNI.HELICS_ERROR_INVALID_STATE_TRANSITION_get());
  /**
   *  the function issued a warning of some kind 
   */
  public final static HelicsErrorTypes HELICS_WARNING = new HelicsErrorTypes("HELICS_WARNING", helicsJNI.HELICS_WARNING_get());
  /**
   *  the federate has terminated unexpectedly and the call cannot be completed 
   */
  public final static HelicsErrorTypes HELICS_ERROR_SYSTEM_FAILURE = new HelicsErrorTypes("HELICS_ERROR_SYSTEM_FAILURE", helicsJNI.HELICS_ERROR_SYSTEM_FAILURE_get());
  /**
   *  the input was discarded and not used for some reason 
   */
  public final static HelicsErrorTypes HELICS_ERROR_DISCARD = new HelicsErrorTypes("HELICS_ERROR_DISCARD", helicsJNI.HELICS_ERROR_DISCARD_get());
  /**
   *  the parameter passed was invalid and unable to be used 
   */
  public final static HelicsErrorTypes HELICS_ERROR_INVALID_ARGUMENT = new HelicsErrorTypes("HELICS_ERROR_INVALID_ARGUMENT", helicsJNI.HELICS_ERROR_INVALID_ARGUMENT_get());
  /**
   *  indicator that the object used was not a valid object 
   */
  public final static HelicsErrorTypes HELICS_ERROR_INVALID_OBJECT = new HelicsErrorTypes("HELICS_ERROR_INVALID_OBJECT", helicsJNI.HELICS_ERROR_INVALID_OBJECT_get());
  /**
   *  the operation to connect has failed 
   */
  public final static HelicsErrorTypes HELICS_ERROR_CONNECTION_FAILURE = new HelicsErrorTypes("HELICS_ERROR_CONNECTION_FAILURE", helicsJNI.HELICS_ERROR_CONNECTION_FAILURE_get());
  /**
   *  registration has failed 
   */
  public final static HelicsErrorTypes HELICS_ERROR_REGISTRATION_FAILURE = new HelicsErrorTypes("HELICS_ERROR_REGISTRATION_FAILURE", helicsJNI.HELICS_ERROR_REGISTRATION_FAILURE_get());
  /**
   *  the function executed successfully 
   */
  public final static HelicsErrorTypes HELICS_OK = new HelicsErrorTypes("HELICS_OK", helicsJNI.HELICS_OK_get());
  /**
   *  user system abort to match typical SIGINT value
   */
  public final static HelicsErrorTypes HELICS_ERROR_USER_ABORT = new HelicsErrorTypes("HELICS_ERROR_USER_ABORT", helicsJNI.HELICS_ERROR_USER_ABORT_get());
  /**
   *  force termination to match typical SIGKILL value
   */
  public final static HelicsErrorTypes HELICS_ERROR_TERMINATED = new HelicsErrorTypes("HELICS_ERROR_TERMINATED", helicsJNI.HELICS_ERROR_TERMINATED_get());

  public final int swigValue() {
    return swigValue;
  }

  public String toString() {
    return swigName;
  }

  public static HelicsErrorTypes swigToEnum(int swigValue) {
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (int i = 0; i < swigValues.length; i++)
      if (swigValues[i].swigValue == swigValue)
        return swigValues[i];
    throw new IllegalArgumentException("No enum " + HelicsErrorTypes.class + " with value " + swigValue);
  }

  private HelicsErrorTypes(String swigName) {
    this.swigName = swigName;
    this.swigValue = swigNext++;
  }

  private HelicsErrorTypes(String swigName, int swigValue) {
    this.swigName = swigName;
    this.swigValue = swigValue;
    swigNext = swigValue+1;
  }

  private HelicsErrorTypes(String swigName, HelicsErrorTypes swigEnum) {
    this.swigName = swigName;
    this.swigValue = swigEnum.swigValue;
    swigNext = this.swigValue+1;
  }

  private static HelicsErrorTypes[] swigValues = { HELICS_ERROR_FATAL, HELICS_ERROR_EXTERNAL_TYPE, HELICS_ERROR_OTHER, HELICS_USER_EXCEPTION, HELICS_ERROR_INSUFFICIENT_SPACE, HELICS_ERROR_EXECUTION_FAILURE, HELICS_ERROR_INVALID_FUNCTION_CALL, HELICS_ERROR_INVALID_STATE_TRANSITION, HELICS_WARNING, HELICS_ERROR_SYSTEM_FAILURE, HELICS_ERROR_DISCARD, HELICS_ERROR_INVALID_ARGUMENT, HELICS_ERROR_INVALID_OBJECT, HELICS_ERROR_CONNECTION_FAILURE, HELICS_ERROR_REGISTRATION_FAILURE, HELICS_OK, HELICS_ERROR_USER_ABORT, HELICS_ERROR_TERMINATED };
  private static int swigNext = 0;
  private final int swigValue;
  private final String swigName;
}

