/*
 * Sample program to demonstrate IoTKit publish feature through Edison API
 * Copyright (c) 2014, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 */

/**
 * @file sample_async_cloud-service.c
 * @brief Sample to demonstrate MQTT publisher through Edison API
 *
 * Provides features to connect to an MQTT Broker and publish a topic
 */

#include <stdio.h>
#include <cJSON.h>
#include <stdbool.h>
#include <sys/types.h>
#include "edisonapi.h"
#include "util.h"

#ifndef DEBUG
    #define DEBUG 0
#endif

ServiceDescription *srvDesc = NULL;
int msgnumber = 0;

/**
 * @name callback to handle the communication
 * @brief handles the communication with an MQTT broker once the connection is established
 * @param[in] error_code specifies the error code is any
 * @param[in] serviceHandle is the client object initialized with the required APIs
 *
 * handles the communication such as publishing data to an MQTT broker once the connection is established
 */
void callback(void *handle, int32_t error_code, void *serviceHandle)
{
    Context context;
    char msg[256];

    if(serviceHandle != NULL){
        CommHandle *commHandle = (CommHandle *)serviceHandle;

        int (**send)(char *message,Context context);

        send = commInterfacesLookup(commHandle, "send");
        if(send == NULL){
            printf("Function \'send\' is not available; please verify the Plugin documentation !!\n");
        }


        context.name = "topic";
        context.value = "data";

        while(1){
        //{"n": "garage","v": i};
            sprintf(msg, "{\"n\": \"garage\", \"v\": %d}", msgnumber++);
            printf("Publishing msg:%s\n", msg);

            (*send)(msg, context);
            sleep(2);
        }
    }
}

/**
 * @name Starts the application
 * @brief Starts the application to demonstrate publish for a topic
 *
 * Establishes the connection with an MQTT broker
 */
int main(void) {

	puts("Sample program to publish data to IoT Cloud !!");

    srvDesc = (ServiceDescription *) parseServiceDescription("../serviceSpecs/temperatureServiceIoTKit.json");

    #if DEBUG
        printf("status:%d:service_name:%s:address:%s:port:%d:name:%s:protocol:%s\n", srvDesc->status, srvDesc->service_name, srvDesc->address, srvDesc->port, srvDesc->type.name, srvDesc->type.protocol);
    #endif


    if (srvDesc){
        createClientForGivenService(srvDesc, callback);
    }
//	    WaitToAdvertiseService(srvDesc, callback);

	return 0;
}

