
/*
Copyright (C) 2017, Battelle Memorial Institute
All rights reserved.

This software was co-developed by Pacific Northwest National Laboratory, operated by the Battelle Memorial Institute; the National Renewable Energy Laboratory, operated by the Alliance for Sustainable Energy, LLC; and the Lawrence Livermore National Laboratory, operated by Lawrence Livermore National Security, LLC.

*/

#ifndef HELICS_API_OBJECTS_H_
#define HELICS_API_OBJECTS_H_
#pragma once

#include <memory>
#include "shared_api_library/api-data.h"
#include "application_api/helicsTypes.hpp"
#include "core/core-data.h"
namespace helics
{
	class Core;
	class Federate;
	class CoreBroker;
	class ValueFederate;
	class MessageFederate;
	class MessageFilterFederate;
	class Subscription;
	class Publication;
	class Endpoint;
	class Filter;

	enum class vtype:int
	{
		genericFed,
		valueFed,
		messageFed,
		filterFed,
		combinFed,
	};

	/** object wrapping a broker for the c-api*/
	class BrokerObject
	{
	public:
		std::shared_ptr<CoreBroker> brokerptr;
	};

	/** object wrapping a core for the c-api*/
	class coreObject
	{
	public:
		std::shared_ptr<Core> coreptr;
	};
	
	/** object wrapping a federate for the c-api*/
	class FedObject
	{
	public:
		vtype type;
		int valid;
		std::shared_ptr<Federate> fedptr;
		std::unique_ptr<Message> lastMessage;
	};

	/** object wrapping a subscription*/
	class SubscriptionObject
	{
	public:
		std::shared_ptr<Subscription> subptr;
		subscription_id_t id;
		bool rawOnly = false;
		std::shared_ptr<ValueFederate> fedptr;
	};

	class PublicationObject
	{
	public:
		std::shared_ptr<Publication> pubptr;
		publication_id_t id;
		bool rawOnly = false;
		std::shared_ptr<ValueFederate> fedptr;
	};

	class EndpointObject
	{
	public:
		std::shared_ptr<Endpoint> endptr;
		std::shared_ptr<MessageFederate> fedptr;
		std::unique_ptr<Message> lastMessage;
	};

	class SourceFilterObject
	{
	public:
		std::shared_ptr<Filter> filtptr;
		std::shared_ptr<MessageFilterFederate> fedptr;
	};

	class DestFilterObject
	{
	public:
		std::shared_ptr<Filter> subptr;
		std::shared_ptr<MessageFilterFederate> fedptr;
	};
}

helics::Federate *getFed(helics_federate fed);
helics::ValueFederate *getValueFed(helics_value_federate fed);
helics::MessageFederate *getMessageFed(helics_message_federate fed);
helics::MessageFilterFederate *getFilterFed(helics_message_filter_federate fed);

std::shared_ptr<helics::Federate> getFedSharedPtr(helics_federate fed);
std::shared_ptr<helics::ValueFederate> getValueFedSharedPtr(helics_value_federate fed);
std::shared_ptr<helics::MessageFederate> getMessageFedSharedPtr(helics_message_federate fed);
std::shared_ptr<helics::MessageFilterFederate> getFilterFedSharedPtr(helics_message_filter_federate fed);

#endif