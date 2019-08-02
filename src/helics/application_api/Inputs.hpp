/*
Copyright © 2017-2019,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC.  See
the top-level NOTICE for additional details. All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

#include "HelicsPrimaryTypes.hpp"
#include "ValueFederate.hpp"
#include "helicsTypes.hpp"
#include <algorithm>
#include <array>

namespace units
{
class precise_unit;
}

namespace helics
{
/** base class for a input object*/
class Input
{
  protected:
    ValueFederate *fed = nullptr;  //!< reference to the value federate
    interface_handle handle;  //!< the id as generated by the Federate
  private:
    int referenceIndex = -1;  //!< an index used for callback lookup
    void *dataReference = nullptr;  //!< pointer to a piece of containing data

    data_type type = data_type::helics_unknown;  //!< the underlying type the publication is using
    bool changeDetectionEnabled = false;  //!< the change detection is enabled
    bool hasUpdate = false;  //!< the value has been updated
    bool disableAssign = false;  //!< disable assignment for the object
    size_t customTypeHash = 0;  //!< a hash code for the custom type
    defV lastValue;  //!< the last value updated
    std::shared_ptr<units::precise_unit> outputUnits;
    std::shared_ptr<units::precise_unit> inputUnits;

    double delta = -1.0;  //!< the minimum difference
    std::string actualName;  //!< the name of the Input
    // this needs to match the defV type
    mpark::variant<std::function<void(const double &, Time)>,
                   std::function<void(const int64_t &, Time)>,
                   std::function<void(const std::string &, Time)>,
                   std::function<void(const std::complex<double> &, Time)>,
                   std::function<void(const std::vector<double> &, Time)>,
                   std::function<void(const std::vector<std::complex<double>> &, Time)>,
                   std::function<void(const NamedPoint &, Time)>,
                   std::function<void(const bool &, Time)>,
                   std::function<void(const Time &, Time)>>
      value_callback;  //!< callback function for the federate
  public:
    /** Default constructor*/
    Input () = default;
    /** construct from a federate and handle, mainly used by the valueFederateManager*/
    Input (ValueFederate *valueFed,
           interface_handle id,
           const std::string &actName,
           const std::string unitsOut = std::string{});

    Input (ValueFederate *valueFed,
           const std::string &key,
           const std::string &defaultType = "def",
           const std::string &units = std::string{});

    template <class FedPtr>
    Input (FedPtr &valueFed,
           const std::string &key,
           const std::string &defaultType = "def",
           const std::string &units = std::string{})
        : Input (std::addressof (*valueFed), key, defaultType, units)
    {
        static_assert (std::is_base_of<ValueFederate, std::remove_reference_t<decltype (*valueFed)>>::value,
                       "first argument must be a pointer to a ValueFederate");
    }

    Input (interface_visibility locality,
           ValueFederate *valueFed,
           const std::string &name,
           const std::string &defaultType = "def",
           const std::string &units = std::string{});

    template <class FedPtr>
    Input (interface_visibility locality,
           FedPtr &valueFed,
           const std::string &name,
           const std::string &defaultType = "def",
           const std::string &units = std::string{})
        : Input (locality, std::addressof (*valueFed), name, defaultType, units)
    {
        static_assert (std::is_base_of<ValueFederate, std::remove_reference_t<decltype (*valueFed)>>::value,
                       "first argument must be a pointer to a ValueFederate");
    }

    Input (ValueFederate *valueFed,
           const std::string &name,
           data_type defType,
           const std::string &units = std::string{})
        : Input (valueFed, name, typeNameStringRef (defType), units)
    {
    }

    template <class FedPtr>
    Input (FedPtr &valueFed, const std::string &name, data_type defType, const std::string &units = std::string ())
        : Input (valueFed, name, typeNameStringRef (defType), units)
    {
    }

    Input (interface_visibility locality,
           ValueFederate *valueFed,
           const std::string &name,
           const std::string &units = std::string{})
        : Input (locality, valueFed, name, "def", units)
    {
    }

    template <class FedPtr>
    Input (interface_visibility locality,
           FedPtr &valueFed,
           const std::string &key,
           const std::string &units = std::string{})
        : Input (locality, valueFed, key, "def", units)
    {
    }

    Input (interface_visibility locality,
           ValueFederate *valueFed,
           const std::string &name,
           data_type defType,
           const std::string &units = std::string{})
        : Input (locality, valueFed, name, typeNameStringRef (defType), units)
    {
    }

    template <class FedPtr>
    Input (interface_visibility locality,
           FedPtr &valueFed,
           const std::string &name,
           data_type defType,
           const std::string &units = std::string{})
        : Input (locality, valueFed, name, typeNameStringRef (defType), units)
    {
    }

    /** get the time of the last update
    @return the time of the last update
    */
    Time getLastUpdate () const { return fed->getLastUpdateTime (*this); }
    /** get the underlying handle that can be used to make direct calls to the Core API
     */
    interface_handle getHandle () const { return handle; }
    /** implicit conversion operator for extracting the handle*/
    operator interface_handle () const { return handle; }
    /** check if the Publication links to a valid operation*/
    bool isValid () const { return handle.isValid (); }
    bool operator< (const Input &inp) const { return (handle < inp.handle); }
    bool operator> (const Input &inp) const { return (handle > inp.handle); }
    /** register a callback for an update notification
    @details the callback is called in the just before the time request function returns
    @param callback a function with signature void( Time time)
    time is the time the value was updated  This callback is a notification callback and doesn't return the value
    */
    void registerNotificationCallback (std::function<void(Time)> callback)
    {
        fed->setInputNotificationCallback (*this,
                                           [this, callback = std::move (callback)](const Input &, Time time) {
                                               if (isUpdated ())
                                               {
                                                   callback (time);
                                               }
                                           });
    }
    /** get the Name/Key for the input
    @details the name is the local name if given, key is the full key name*/
    const std::string &getName () const { return actualName; }
    /** get the Name/Key for the input
    @details the name is the local name if given, key is the full key name*/
    const std::string &getKey () const { return fed->getInterfaceName (handle); }
    /** get the display name for an input
    @details the name is the given local name or if empty the name of the target*/
    const std::string &getDisplayName () const { return (actualName.empty () ? getTarget () : actualName); }

    /** get the type of the data coming from the publication*/
    const std::string &getPublicationType () const
    {
        return ((type == data_type::helics_unknown) || (type == data_type::helics_custom)) ?
                 fed->getInjectionType (*this) :
                 typeNameStringRef (type);
    }
    /** get the type of the input*/
    const std::string &getType () const { return fed->getExtractionType (*this); }
    /** get the units associated with a input*/
    const std::string &getUnits () const { return fed->getExtractionUnits (*this); }
    /** get the units associated with a input*/
    const std::string &getInjectionUnits () const { return fed->getInjectionUnits (*this); }
    /** get an associated target*/
    const std::string &getTarget () const { return fed->getTarget (*this); }
    /** subscribe to a named publication*/
    void addTarget (const std::string &newTarget) { fed->addTarget (*this, newTarget); }
    /** remove a named publication from being a target*/
    void removeTarget (const std::string &targetToRemove) { fed->removeTarget (*this, targetToRemove); }
    /** get the interface information field of the input*/
    const std::string &getInfo () const { return fed->getInfo (handle); }
    /** set the interface information field of the input*/
    void setInfo (const std::string &info) { fed->setInfo (handle, info); }
    /** set a handle flag for the input*/
    void setOption (int32_t option, bool value = true) { fed->setInterfaceOption (handle, option, value); }

    /** get the current value of a flag for the handle*/
    bool getOption (int32_t option) const { return fed->getInterfaceOption (handle, option); }
    /** check if the value has been updated
    @details if changeDetection is Enabled this function also loads the value into the buffer
    @param assumeUpdate if set to true will assume there was a publication and not check it first, if set to
    false[default] it will check the federate first
    @return true if the value has been updated*/
    bool checkUpdate (bool assumeUpdate = false);

    /** clear the isUpdated flag*/
    void clearUpdate ();
    /** check if the value has been updated including interpretation of the change detection
     */
    bool isUpdated ();
    /** check if the value has been updated,
    @details the const version can in some circumstances return true even if the value would not be updated
    the circumstances in which this is true are a minimum change has been set, checkUpdate has not been
    call(meaning it is a standalone copy, not the one stored with the federate, and the value has been published
    but would not trigger the change detection. If this is to be avoided use the non-const version or call
    checkUpdate before calling this function.
     */
    bool isUpdated () const;

    /** register a callback for the update
    @details the callback is called in the just before the time request function returns
    @param callback a function with signature void(X val, Time time)
    val is the new value and time is the time the value was updated
    */
    template <class X>
    void setInputNotificationCallback (std::function<void(const X &, Time)> callback)
    {
        static_assert (
          helicsType<X> () != data_type::helics_custom,
          "callback type must be a primary helics type one of \"double, int64_t, named_point, bool, Time "
          "std::vector<double>, std::vector<std::complex<double>>, std::complex<double>\"");
        value_callback = std::move (callback);
        fed->setInputNotificationCallback (*this, [this](Input &, Time time) { handleCallback (time); });
    }

  private:
    template <class X>
    void setDefault_impl (std::integral_constant<int, 0> /*V*/, X &&val)
    {
        /** still need to make_valid for bool and Time*/
        lastValue = make_valid (std::forward<X> (val));
    }

    template <class X>
    void setDefault_impl (std::integral_constant<int, 1> /*V*/, X &&val)
    {
        lastValue = make_valid (std::forward<X> (val));
    }

    template <class X>
    void setDefault_impl (std::integral_constant<int, 2> /*V*/, X &&val)
    {
        fed->setDefaultValue (*this, ValueConverter<remove_cv_ref<X>>::convert (std::forward<X> (val)));
    }

  public:
    /** set the default value to use before any update has been published
     */
    template <class X>
    void setDefault (X &&val)
    {
        setDefault_impl<X> (typeCategory<X> (), std::forward<X> (val));
    }

    /** set the minimum delta for change detection
    @param deltaV a double with the change in a value in order to register a different value
    */
    void setMinimumChange (double deltaV)
    {
        // this first check enables change detection if it was disabled via negative delta
        if (delta < 0.0)
        {
            changeDetectionEnabled = true;
        }
        delta = deltaV;
        // the second checks if we should disable from negative delta
        if (delta < 0.0)
        {
            changeDetectionEnabled = false;
        }
    }
    /** enable change detection
    @param enabled (optional) set to false to disable change detection true(default) to enable it
    */
    void enableChangeDetection (bool enabled = true) { changeDetectionEnabled = enabled; }

  private:
    /** deal with the callback from the application API*/
    void handleCallback (Time time);
    template <class X>
    void getValue_impl (std::integral_constant<int, primaryType> /*V*/, X &out);

    /** handle special case for character return data*/
    void getValue_impl (std::integral_constant<int, primaryType> /*V*/, char &out) { out = getValueChar (); }

    /** handle special case for character return data*/
    void getValue_impl (std::integral_constant<int, convertibleType> /*V*/, char &out) { out = getValueChar (); }

    template <class X>
    void getValue_impl (std::integral_constant<int, convertibleType> /*V*/, X &out)
    {
        std::conditional_t<std::is_integral<X>::value,
                           std::conditional_t<std::is_same<X, char>::value, char, int64_t>, double>
          gval;
        getValue_impl (std::integral_constant<int, primaryType> (), gval);
        out = static_cast<X> (gval);
    }

    template <class X>
    void getValue_impl (std::integral_constant<int, nonConvertibleType> /*V*/, X &out)
    {
        ValueConverter<X>::interpret (fed->getValueRaw (*this), out);
    }

    template <class X>
    X getValue_impl (std::integral_constant<int, primaryType> /*V*/)
    {
        X val;  // NOLINT
        getValue_impl (std::integral_constant<int, primaryType> (), val);
        return val;
    }

    template <class X>
    X getValue_impl (std::integral_constant<int, convertibleType> /*V*/)
    {
        std::conditional_t<std::is_integral<X>::value,
                           std::conditional_t<std::is_same<X, char>::value, char, int64_t>, double>
          gval;
        getValue_impl (std::integral_constant<int, primaryType> (), gval);
        return static_cast<X> (gval);
    }

    template <class X>
    X getValue_impl (std::integral_constant<int, nonConvertibleType> /*V*/)
    {
        return ValueConverter<X>::interpret (fed->getValueRaw (*this));
    }

  public:
    /** get double vector value functions to retrieve data by a C array of doubles*/
    int getValue (double *data, int maxsize);
    /** get string value functions to retrieve data by a C string*/
    int getValue (char *str, int maxsize);
    /** get the latest value for the input
    @param[out] out the location to store the value
    */
    template <class X>
    void getValue (X &out)
    {
        getValue_impl<X> (typeCategory<X> (), out);
    }
    /** get the most recent value
    @return the value*/
    template <class X>
    auto getValue ()
    {
        return getValue_impl<remove_cv_ref<X>> (typeCategory<X> ());
    }

    template <class X>
    const X &getValueRef ();

    /** get the raw binary data*/
    data_view getRawValue ();
    /** get the size of the raw data*/
    size_t getRawSize ();
    /** get the size of the data if it were a string*/
    size_t getStringSize ();
    /** get the number of elements in the data if it were a vector*/
    size_t getVectorSize ();
    /** close a input during an active simulation
    @details it is not necessary to call this function unless you are continuing the simulation after the close*/
    void close () { fed->closeInterface (handle); }
    /** get the HELICS data type for the input*/
    data_type getHelicsType () const { return type; }

  private:
    /** load some information about the data source such as type and units*/
    void loadSourceInformation ();
    /** helper class for getting a character since that is a bit odd*/
    char getValueChar ();
    /** helper function to do the extraction and any necessary conversions for doubles*/
    friend class ValueFederateManager;
};

/** convert a dataview to a double and do a unit conversion if appropriate*/
double doubleExtractAndConvert (const data_view &dv,
                                const std::shared_ptr<units::precise_unit> &inputUnits,
                                const std::shared_ptr<units::precise_unit> &outputUnits);

/** class to handle an input and extract a specific type
@tparam X the class of the value associated with a input*/
template <class X>
class InputT : public Input
{
  public:
  private:
    std::function<void(X, Time)> value_callback;  //!< callback function for the federate
    std::function<double(const X &v1, const X &v2)>
      changeDetectionOperator;  //!< callback function for change detection
    // determine if we can convert to a primary type
    using is_convertible_to_primary_type =
      std::conditional_t<((helicsType<X> () != data_type::helics_custom) || (isConvertableType<X> ())),
                         std::true_type,
                         std::false_type>;

  public:
    InputT () = default;
    /**constructor to build an input with a defined object type
    @param valueFed  the ValueFederate to use
    @param name the name of the input
    @param units the units associated with a Federate
    */
    InputT (ValueFederate *valueFed, const std::string &name, const std::string &units = std::string ())
        : Input (valueFed, name, ValueConverter<X>::type (), units)
    {
    }
    /**constructor to build an input with a defined type
    @param valueFed  the ValueFederate to use
    @param name the name of the input
    @param units the units associated with a Federate
    */
    template <class FedPtr>
    InputT (FedPtr &valueFed, const std::string &name, const std::string &units = std::string ())
        : Input (valueFed, name, ValueConverter<X>::type (), units)
    {
    }

  public:
    /** get the most recent value
    @return the value*/
    X getValue () { return Input::getValue<X> (); }
    /** store the value in the given variable
    @param[out] out the location to store the value
    */
    void getValue (X &out) { Input::getValue<X> (out); }

    /** register a callback for the update
    @details the callback is called in the just before the time request function returns
    @param callback a function with signature void(X val, Time time)
    val is the new value and time is the time the value was updated
    */
    void setInputNotificationCallback (std::function<void(X, Time)> callback)
    {
        value_callback = callback;
        fed->setInputNotificationCallback (*this, [=](Input &, Time time) { handleCallback (time); });
    }
    /** set a default value
    @param val the value to set as the default
    */
    void setDefault (const X &val) { Input::setDefault (val); }

  private:
    void handleCallback (Time time)
    {
        X out;
        Input::getValue (out);
        value_callback (out, time);
    }
};

template <class X>
void Input::getValue_impl (std::integral_constant<int, primaryType> /*V*/, X &out)
{
    if (fed->isUpdated (*this) || (hasUpdate && !changeDetectionEnabled))
    {
        auto dv = fed->getValueRaw (*this);
        if (type == data_type::helics_unknown)
        {
            loadSourceInformation ();
        }

        if (type == helics::data_type::helics_double)
        {
            defV val = doubleExtractAndConvert (dv, inputUnits, outputUnits);
            valueExtract (val, out);
        }
        else
        {
            valueExtract (dv, type, out);
        }
        if (changeDetectionEnabled)
        {
            if (changeDetected (lastValue, out, delta))
            {
                lastValue = make_valid (out);
            }
            else
            {
                valueExtract (lastValue, out);
            }
        }
        else
        {
            lastValue = make_valid (out);
        }
    }
    else
    {
        valueExtract (lastValue, out);
    }
    hasUpdate = false;
}

template <class X>
inline const X &getValueRefImpl (defV &val)
{
    valueConvert (val, helicsType<X> ());
    return mpark::get<X> (val);
}

template <>
inline const std::string &getValueRefImpl (defV &val)
{
    // don't convert a named point to a string
    if ((val.index () == named_point_loc))
    {
        return mpark::get<NamedPoint> (val).name;
    }
    valueConvert (val, data_type::helics_string);
    return mpark::get<std::string> (val);
}

template <class X>
const X &Input::getValueRef ()
{
    static_assert (std::is_same<typeCategory<X>, std::integral_constant<int, primaryType>>::value,
                   "calling getValue By ref must be with a primary type");
    if (fed->isUpdated (*this) || (hasUpdate && !changeDetectionEnabled))
    {
        auto dv = fed->getValueRaw (*this);
        if (type == data_type::helics_unknown)
        {
            loadSourceInformation ();
        }

        if (changeDetectionEnabled)
        {
            X out;
            if (type == helics::data_type::helics_double)
            {
                defV val = doubleExtractAndConvert (dv, inputUnits, outputUnits);
                valueExtract (val, out);
            }
            else
            {
                valueExtract (dv, type, out);
            }
            if (changeDetected (lastValue, out, delta))
            {
                lastValue = make_valid (std::move (out));
            }
        }
        else
        {
            valueExtract (dv, type, lastValue);
        }
    }
    else
    {
        // TODO:: PT make some logic that it can get the raw data from the core again if it was converted already
    }

    return getValueRefImpl<remove_cv_ref<X>> (lastValue);
}
}  // namespace helics
