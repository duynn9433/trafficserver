/** @file

  A brief file description

  @section license License

  Licensed to the Apache Software Foundation (ASF) under one
  or more contributor license agreements.  See the NOTICE file
  distributed with this work for additional information
  regarding copyright ownership.  The ASF licenses this file
  to you under the Apache License, Version 2.0 (the
  "License"); you may not use this file except in compliance
  with the License.  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 */

/* bnull-transform.c:  an example program that illustrates a buffered
 *                     null transform.
 *
 *
 *
 *    Usage:
 *      bnull-transform.so
 *
 *
 */

/* set tab stops to four. */

#include <stdio.h>

#include "ts/ts.h"
#include "tscore/ink_defs.h"

#define PLUGIN_NAME "bnull_transform"

#define TS_NULL_MUTEX NULL
#define STATE_BUFFER_DATA 0
#define STATE_OUTPUT_DATA 1

typedef struct {
  int state;
  TSVIO output_vio;
  TSIOBuffer output_buffer;
  TSIOBufferReader output_reader;
} MyData;

static MyData *
my_data_alloc()
{
  MyData *data;

  data                = (MyData *)TSmalloc(sizeof(MyData));
  data->state         = STATE_BUFFER_DATA;
  data->output_vio    = NULL;
  data->output_buffer = NULL;
  data->output_reader = NULL;

  return data;
}

static void
my_data_destroy(MyData *data)
{
  if (data) {
    if (data->output_buffer) {
      TSIOBufferDestroy(data->output_buffer);
    }
    TSfree(data);
  }
}

static int
handle_buffering(TSCont contp, MyData *data)
{
  TSVIO write_vio;
  int64_t towrite;

  /* Get the write VIO for the write operation that was performed on
     ourself. This VIO contains the buffer that we are to read from
     as well as the continuation we are to call when the buffer is
     empty. */
  /*This is so that the transformation can get information
  about the upstream vconnection’s write operation to the input buffer.*/
  write_vio = TSVConnWriteVIOGet(contp);

  /* Create the output buffer and its associated reader */
  if (!data->output_buffer) {
    data->output_buffer = TSIOBufferCreate();
    TSAssert(data->output_buffer);
    data->output_reader = TSIOBufferReaderAlloc(data->output_buffer);
    TSAssert(data->output_reader);
  }

  /* We also check to see if the write VIO's buffer is non-NULL. A
     NULL buffer indicates that the write operation has been
     shutdown and that the continuation does not want us to send any
     more WRITE_READY or WRITE_COMPLETE events. For this buffered
     transformation that means we're done buffering data. */

  if (!TSVIOBufferGet(write_vio)) {
    data->state = STATE_OUTPUT_DATA;
    return 0;
  }

  /* Determine how much data we have left to read. For this bnull
     transform plugin this is also the amount of data we have left
     to write to the output connection. */

  towrite = TSVIONTodoGet(write_vio);
  if (towrite > 0) {
    /* The amount of data left to read needs to be truncated by
       the amount of data actually in the read buffer. */

    int64_t avail = TSIOBufferReaderAvail(TSVIOReaderGet(write_vio));
    if (towrite > avail) {
      towrite = avail;
    }

    if (towrite > 0) {
      /* Copy the data from the read buffer to the input buffer. */
      /* -Copy data from the upstream input buffer to the output buffer.-*/
      TSIOBufferCopy(data->output_buffer, TSVIOReaderGet(write_vio), towrite, 0);

      /* Tell the read buffer that we have read the data and are no
         longer interested in it. */
      /* -Tell the upstream input buffer that the transformation has read the data-*/
      TSIOBufferReaderConsume(TSVIOReaderGet(write_vio), towrite);

      /* Modify the write VIO to reflect how much data we've
         completed. */
      /* -Modify the input VIO to tell it how much data has been read
      (increase the value of ndone).-*/
      TSVIONDoneSet(write_vio, TSVIONDoneGet(write_vio) + towrite);
    }
  }
  /* -If there is more data left to read ( if ndone < nbytes),
  then the handle_buffering function wakes up the upstream vconnection
  by sending it WRITE_READY- */
  /* - The transformation sends WRITE_READY events when it needs more data;
  when data is available, the upstream vconnection reenables the transformation
  with an IMMEDIATE event.- */
  /* Now we check the write VIO to see if there is data left to read. */
  if (TSVIONTodoGet(write_vio) > 0) {
    if (towrite > 0) {
      /* Call back the write VIO continuation to let it know that we
         are ready for more data. */
      TSContCall(TSVIOContGet(write_vio), TS_EVENT_VCONN_WRITE_READY, write_vio);
    }
  } else {
    data->state = STATE_OUTPUT_DATA;

    /* Call back the write VIO continuation to let it know that we
       have completed the write operation. */
    /* -When the data is read into the output buffer,
    the handle_buffering function sets the state of the transformation’s data structure to
    STATE_OUTPUT_DATA and calls the upstream vconnection back with the WRITE_COMPLETE event.- */
    TSContCall(TSVIOContGet(write_vio), TS_EVENT_VCONN_WRITE_COMPLETE, write_vio);
    /** -The upstream vconnection will probably shut down the write operation
    when it receives the WRITE_COMPLETE event.
    The handler function of the transformation, bnull_transform,
    receives an IMMEDIATE event and calls the handle_transform function.
    This time, the state is STATE_OUTPUT_DATA, so handle_transform calls handle_output.- */
  }

  return 1;
}

static int
handle_output(TSCont contp, MyData *data)
{
  /* Check to see if we need to initiate the output operation. */
  if (!data->output_vio) {
    /*test*/
    print_buffer_content(data);

    /*end-test*/
    TSVConn output_conn;

    /* Get the output connection where we'll write data to. */
    /* -The handle_output function gets a handle to the output vconnection:- */
    output_conn = TSTransformOutputVConnGet(contp);

    /* The handle_output function writes the buffer to the output vconnection:*/
    data->output_vio = TSVConnWrite(output_conn, contp, data->output_reader, TSIOBufferReaderAvail(data->output_reader));

    TSAssert(data->output_vio);
  }
  return 1;
}
void
print_buffer_content(MyData *data)
{
  int64_t avail;
  const char *start;

  if (data->output_buffer && data->output_reader) {
    avail = TSIOBufferReaderAvail(data->output_reader);
    start = TSIOBufferReaderStart(data->output_reader);

    if (avail > 0 && start) {
      char *buffer_content = (char *)malloc(avail + 1);
      memcpy(buffer_content, start + 10, avail);
      buffer_content[avail] = '\0'; // Null-terminate the string

      printf("%s\n", buffer_content);

      free(buffer_content);
    }
  }
}

/**The handle_transform function examines the data parameter for the continuation passed to it
 * (the continuation passed to handle_transform is the transformation vconnection).
 * The data structure keeps track of two states:
 *    copying the data into the buffer (STATE_BUFFER_DATA)
 *    and writing the contents of the buffer to the output vconnection (STATE_OUTPUT_DATA).*/
static void
handle_transform(TSCont contp)
{
  MyData *data;
  int done;

  /* Get our data structure for this operation. The private data
     structure contains the output VIO and output buffer. If the
     private data structure pointer is NULL, then we'll create it
     and initialize its internals. */

  data = (MyData *)TSContDataGet(contp);
  if (!data) {
    data = my_data_alloc();
    TSContDataSet(contp, (void *)data);
  }

  do {
    switch (data->state) {
    case STATE_BUFFER_DATA:
      done = handle_buffering(contp, data);
      break;
    case STATE_OUTPUT_DATA:
      /** -The upstream vconnection will probably shut down the write operation
      when it receives the WRITE_COMPLETE event.
      The handler function of the transformation, bnull_transform,
      receives an IMMEDIATE event and calls the handle_transform function.
      This time, the state is STATE_OUTPUT_DATA, so handle_transform calls handle_output.- */
      done = handle_output(contp, data);
      break;
    default:
      done = 1;
      break;
    }
  } while (!done);
}

// handle ERROR, WRITE_COMPLETE, WRITE_READY, and IMMEDIATE events.
//**bnull_transform** function calls **handle_transform** to handle WRITE_READY and IMMEDIATE.
static int
bnull_transform(TSCont contp, TSEvent event, void *edata ATS_UNUSED)
{
  /* Check to see if the transformation has been closed by a
     call to TSVConnClose. */

  if (TSVConnClosedGet(contp)) {
    my_data_destroy((MyData *)TSContDataGet(contp));
    TSContDestroy(contp);
  } else {
    switch (event) {
    case TS_EVENT_ERROR: {
      TSVIO write_vio;

      /* Get the write VIO for the write operation that was
         performed on ourself. This VIO contains the continuation of
         our parent transformation. */
      write_vio = TSVConnWriteVIOGet(contp);

      /* Call back the write VIO continuation to let it know that we
         have completed the write operation. */
      TSContCall(TSVIOContGet(write_vio), TS_EVENT_ERROR, write_vio);
      break;
    }

    case TS_EVENT_VCONN_WRITE_COMPLETE:
      /* When our output connection says that it has finished
         reading all the data we've written to it then we should
         shutdown the write portion of its connection to
         indicate that we don't want to hear about it anymore. */

      TSVConnShutdown(TSTransformOutputVConnGet(contp), 0, 1);
      break;

    case TS_EVENT_VCONN_WRITE_READY:
    default:
      /* If we get a WRITE_READY event or any other type of event
         (sent, perhaps, because we were reenabled) then we'll attempt
         to transform more data. */
      handle_transform(contp);
      break;
    }
  }

  return 0;
}
/*
The default behavior for transformations is to cache the transformed content
(if desired, you also can tell Traffic Server to cache untransformed content).
Therefore, only responses received directly from an origin server need to be transformed.
Objects served from the cache are already transformed.
To determine whether the response is from the origin server,
the routine transformable checks the response header for the “200 OK” server response.*/
static int
transformable(TSHttpTxn txnp)
{
  TSMBuffer bufp;
  TSMLoc hdr_loc;
  TSHttpStatus resp_status;
  int retv = 0;

  /* We are only interested in transforming "200 OK" responses. */

  if (TS_SUCCESS == TSHttpTxnServerRespGet(txnp, &bufp, &hdr_loc)) {
    resp_status = TSHttpHdrStatusGet(bufp, hdr_loc);
    retv        = (resp_status == TS_HTTP_STATUS_OK);
    TSHandleMLocRelease(bufp, TS_NULL_MLOC, hdr_loc);
  }

  return retv;
}
/*If the response is transformable,
then the plugin creates a transformation vconnection that gets called back
when the response data is ready to be transformed
(as it is streaming from the origin server).*/
static void
transform_add(TSHttpTxn txnp)
{
  TSVConn connp;

  connp = TSTransformCreate(bnull_transform, txnp); // handle func is bnull_transform
  TSHttpTxnHookAdd(txnp, TS_HTTP_RESPONSE_TRANSFORM_HOOK, connp);
  return;
}

/*Checks to see if the transaction response is transformable.*/
static int
transform_plugin(TSCont contp ATS_UNUSED, TSEvent event, void *edata)
{
  TSHttpTxn txnp = (TSHttpTxn)edata;

  switch (event) {
  case TS_EVENT_HTTP_READ_RESPONSE_HDR:
    if (transformable(txnp)) {
      transform_add(txnp);
    }
    TSHttpTxnReenable(txnp, TS_EVENT_HTTP_CONTINUE);
    return 0;
  default:
    break;
  }

  return 0;
}

void
TSPluginInit(int argc ATS_UNUSED, const char *argv[] ATS_UNUSED)
{
  TSPluginRegistrationInfo info;
  TSMutex mutex = TS_NULL_MUTEX;

  info.plugin_name   = PLUGIN_NAME;
  info.vendor_name   = "Apache Software Foundation";
  info.support_email = "dev@trafficserver.apache.org";

  if (TSPluginRegister(&info) != TS_SUCCESS) {
    TSError("[%s] Plugin registration failed", PLUGIN_NAME);

    goto Lerror;
  }

  /* This is call we could use if we need to protect global data */
  /* TSReleaseAssert ((mutex = TSMutexCreate()) != TS_NULL_MUTEX); */

  TSHttpHookAdd(TS_HTTP_READ_RESPONSE_HDR_HOOK, TSContCreate(transform_plugin, mutex));
  return;

Lerror:
  TSError("[%s] Plugin disabled", PLUGIN_NAME);
}
