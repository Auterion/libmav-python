<a id="libmav"></a>

# libmav

<a id="libmav.Connection"></a>

## Connection Objects

```python
class Connection()
```

A class representing a connection for sending and receiving MAVLink messages over a physical channel to a particular connection partner.

> **Note:** This represents a MAVLink communication channel to a particular connection partner, not a connection to a specific MAVLink system.
> Communication is with any/all MAVLink systems on the channel to the connection partner.

`Connection` objects are returned by `NetworkRuntime.await_connection()` and the `NetworkRuntime.on_connection()` callback.
Note that a `NetworkRuntime` using a server interface may return multiple connection objects, if more than one system connects to the server port.


Messages are sent using the `send()` method, which takes a `Message` instance.
The sender MAVLink address that is used in sent messages is set during `NetworkRuntime` initialization (and defaults to `system ID=componentID=97`).

Messages can be recieved either synchronously (blocking) or asychronously.
Note again that the messages can come from any MAVLink node on the channel, and there is no inbuilt filtering to just get messages from a particular component or system.

Applications register for asynchronous callbacks using the `add_message_callback()` method, passing a callback function and optional exception handler, and de-register using `remove_message_callback()`.
The callback is called for every message recieved over the connection.

Applications synchronously recieve a specific message using either of the two `receive()` methods (the methods block until the indicated message is received, or there is a timeout):
- The method that takes an "expectation" should be used when the expected message is sent in response to another message, such as a `COMMAND_ACK` in response to a particular `COMMAND_INT` message (the "expectation" is created using the `expect()` method).
- The method that takes a `message` argument should be used when the desired message is being streamed.

Lastly, you can syncronously check if the connection is alive using the `alive()` method.
If don't want to poll, you can get get asynchronous notification when a connection is lost using `NetworkRuntime.on_connection_lost()`.
```python
# Connect to a TCP server
conn_physical = libmav.UDPClient('192.168.1.12', 14550)
conn_runtime = libmav.NetworkRuntime(self.message_set, heartbeat, conn_physical)

connection = conn_runtime.await_connection(2000)
```

<a id="libmav.Connection.add_message_callback"></a>

#### add\_message\_callback

```python
def add_message_callback(callbackFn)
```

Add a callback function that is called whenever any message is received on the connection.

The callback function is called with a `Message` object.
For example, to print the name of every message, you might use the following code.


A callback can be removed using `remove_message_callback()`

```python
callback_handle = connection.add_message_callback(lambda msg: print(f'{msg.name}'))
```

**Arguments**:

- `callbackFn` _function_ - A callback function that takes a `Message` argument. This is called for every received message.

<a id="libmav.Connection.add_message_callback"></a>

#### add\_message\_callback

```python
def add_message_callback(callbackFn, exceptionFn)
```

Add a callback function for new messages with an exception handler callback.

The callback function is called with a `Message` object.
The exception handler callback is called with an `Exception` if an exception is raised in the callbackFunction.
<!-- raised from the network? - what errors do we get? -->

<!-- Example code? -->

A callback can be removed using `remove_message_callback()`

**Arguments**:

- `callbackFn` _function_ - A callback function that takes a `Message` argument. This is called for every received message.
- `exceptionFn` _function_ - A callback function that takes an `Exception` argument. This is called if there is an exception in the message callback <!-- correct? -->

<a id="libmav.Connection.alive"></a>

#### alive

```python
def alive()
```

Test if connection is still alive.

The connection is considered alive if any MAVLink traffic was received from the partner of this connection in the last 3 seconds.
Note that this is any message on the connection, not a HEARTBEAT from a particular MAVLink node.
The "alive" timeout value is hard coded.


```python
// Check if connection is still alive
if connection.alive():
    # Do something with the connection
```

**Returns**:

- `boolean` - `true` if the connection is alive, otherwise `false`.

<a id="libmav.Connection.expect"></a>

#### expect

```python
def expect(messageName)
```

Create an "expectation" that a particular message will be recieved.

This is used when a message will only be emitted in response to another message.
This lets you specify the message to be expected before sending the triggering message.
You then send the triggering message, and call the `receive()` that takes an "expectation".


Note that if you use the `receive()` message that takes a `messageName`, there is a race condition.
Using an expectation makes libmav start looking for the response immediately.

```python
# Define an expectation that COMMAND_ACK will be receivd
expectation = connection.expect("COMMAND_ACK")
connection.send( <a command packaged in a message> )
command_ack = connection.receive(expectation, 3000)
print(command_ack["result"])
```

**Arguments**:

- `messageName` _string_ - The name of the message to "expect".
  

**Returns**:

- `ExpectationWrapper` - An "expectation" object for the specified message
  (the type is largely irrelevant as it is intended for use in `receive()` only).

<a id="libmav.Connection.partner"></a>

#### partner

```python
def partner()
```

Returns information about the connection partner of this Connection.

This allows libmav applications to differentiate between connections on the same physical interface, for example to determine what connection partners were detected or lost.
This is needed for server interfaces (`UDPServer` or `TCPServer`), where more than one external system might connect to the port libmav is listening on,
The method is not useful for serial (`Serial`) and client interfaces (`UDPClient` or `TCPClient`), which only connect to one partner.

**Returns**:

- `ConnectionPartner` - Information about the address of the connected partner.

<a id="libmav.Connection.receive"></a>

#### receive

```python
def receive(messageName, timeout)
```

Synchronously receive a streamed message.

The method blocks and waits for the specified message to be received.
It either returns the message or raises an exception after a timeout (`TimeOut` Exception).

The code below shows how you might wait for a message with a 1 second timeout.


> **Note**
> - This method waits on only one message, and will ignore any other messages that arrive in the meantime.
>   If you want to wait on more than one message you will need to use `receive(self, expectation, timeout)`
or `add_message_callback()`.
> - Use the `receive(self, expectation, timeout)` version to wait on a message that is emitted in response to a message you have sent.
>   This avoids the race condition where you potentially start waiting for a message that has already been received.

       ```python
       // receive a message, with a 1s timeout
       messageReceived = connection.receive("PARAM_VALUE", 1000);
       ```

**Arguments**:

- `messageName` _string_ - The name of the message to wait for.
- `timeout` _int_ - Timeout in milliseconds. By default there is no timeout.
  

**Returns**:

- `Message` - The message that was waited on.

<a id="libmav.Connection.receive"></a>

#### receive

```python
def receive(expectation, timeout)
```

Synchronously receive an _expected_ message triggered by a send operation.

This version of the method should be used to receive a particular message that is emitted when you first send some other message.
If the message is streamed, it is simpler to use the `receive(self, message, timeout)` version.

The method blocks and waits for the expected message to be received.
It either returns the message or exits after a timeout.


You can also wait on multiple expected messages, which might be received in any order.
The example below shows this for `GLOBAL_POSITION_INT` and `ALTITUDE`:


In the above example, the first `receive()` call will block until `GLOBAL_POSITION_INT` is received.
If `ALTITUDE` was received while waiting for `GLOBAL_POSITION_INT`, the second `receive()` call will complete immediately: otherwise it will block until `ALTITUDE` arrives.
Or to put it another way, each `receive()` call blocks until its expectation is met, but an expectation may already be met by the time the corresponding  `receive()` method is called.

```python
# create a message
messageToSend = message_set.create('SOME_SEND_MESSAGE')

// Start watching for the response before sending
expectation = connection.expect("EXPECTED_RESPONSE_MESSAGE");

# send the message
connection.send(messageToSend);

# receive the expected message, with a 1s timeout
messageReceived = connection.receive(expectation, 1000);
```
```python
global_position_expectation = connection.expect(''GLOBAL_POSITION_INT')
altitude_expectation = connection.expect('ALTITUDE')
connection.receive(global_position_expectation)
connection.receive(altitude_expectation)
```

**Arguments**:

- `expectation` _ExpectationWrapper_ - An expectation, created using `Connection.expect()`.
- `timeout` _int_ - Timeout in milliseconds. By default there is no timeout.
  

**Returns**:

- `Message` - The message that was waited on.

<a id="libmav.Connection.remove_message_callback"></a>

#### remove\_message\_callback

```python
def remove_message_callback(callback_handle)
```

Removes a message callback that was previously added using `add_message_callback()`.

The code below shows how you might add and then remove a callback.


```python   
callback_handle = connection.add_message_callback(lambda msg: print(f'{msg.name}'))
connection.remove_message_callback(callback_handle)
```

**Arguments**:

- `callback_handle` _callback_ - The handle of the callback to remove.

<a id="libmav.Connection.send"></a>

#### send

```python
def send(message)
```

Send a message.

The code below shows how you might create a message and then send it.
See `MessageSet` and `Message` for more information on how you set the message properties.


```python
# create a message
heartbeat_message = message_set.create('HEARTBEAT')

# send the message
conection.send(heartbeat_message);
```

**Arguments**:

- `message` _Message_ - The message to send.

<a id="libmav.ConnectionPartner"></a>

## ConnectionPartner Objects

```python
class ConnectionPartner()
```

A class representing the remote partner of a `Connection`.

Objects of this type are returned by `Connection.partner()`.
Libmav applications can use these to differentiate between connections on the same `NetworkRuntime` interface.
This is needed for server interfaces (`UDPServer` or `TCPServer`), where more than one external system might connect to the port libmav is listening on,

For serial connections `is_uart` is set (there is no need to differentiate partners, because there can only be one partner on the interface).

**Attributes**:

- `address` _int_ - The IP address of the remote end of the connection (int32).
- `is_uart` _boolean_ - `true` if the connection is a serial port.
- `port` _int_ - The port of the remote partner.

<a id="libmav.Header"></a>

## Header Objects

```python
class Header()
```

A class representing the header of a MAVLink message.

This is the information in the [MAVLink frame](https://mavlink.io/en/guide/serialization.html#mavlink2_packet_format) before the payload.

**Attributes**:

- `compat_flags` _int_ - [MAVLink compat flags](https://mavlink.io/en/guide/serialization.html#compat_flags)
- `component_id` _int_ - MAVLink component id (of sender).
- `incompat_flags` _int_ - [MAVLink incompat flags](https://mavlink.io/en/guide/serialization.html#incompat_flags)
- `len` _int_ - Message payload length, in bytes (0 - 255).
- `magic` _int_ - MAVLink message packet start marker (0xFD for MAVLink 2).
- `message_id` _int_ - Message ID number
- `seq` _int_ - Message packet sequence number (Used to detect packet loss).
  Components increment value for each message sent.
- `system_id` _int_ - MAVLink system ID of sender.

<a id="libmav.Identifier"></a>

## Identifier Objects

```python
class Identifier()
```

NOT DONE A class representing a MAVLink system and component address.

**Attributes**:

- `?id` _int_ - ?

<a id="libmav.Message"></a>

## Message Objects

```python
class Message()
```

A class representing a MAVLink message.

Message objects are recieved in the callback passed to `Connection.add_message_callback()`, or returned by the `Connection.receive()` methods.
They can be created (for sending) using `MessageSet.create()`:


The message fields can be set individually:


Fields can also be set and retrieved as a dict:


You can also iterate the list using list comprehensions:


In addition to the attribute below, each message will have attributes for fields in the message payload, such as `system_status`.

```python
# Create a heartbeat message
heartbeat_message = message_set.create('HEARTBEAT')
```
```python
heartbeat['base_mode']=3;
heartbeat['cutom_mode']=3;    
```
```python
# Set HEARTBEAT values from dict
message.set_from_dict({ 
    "type": message_set.enum("MAV_TYPE_GCS"),
    "autopilot": message_set.enum("MAV_AUTOPILOT_GENERIC"), 
    "base_mode": 0,
    "custom_mode": 0,
    "system_status": message_set.enum("MAV_STATE_ACTIVE"),
    "mavlink_version": 2,
})

# Get fields as python dict
message_dict = message.to_dict()
```
```python
# This creates a list of all the values in my_float_array_field multiplied by 2
my_float_array_field = [x * 2 for x in message['my_float_array_field']]
```

**Attributes**:

- `header` _libmav.Header_ - Message header information, such as compatibility flag information.
- `id` _int_ - Message ID.
- `name` _string_ - Message name.

<a id="libmav.Message.set_from_dict"></a>

#### set\_from\_dict

```python
def set_from_dict(fields)
```

Set the fields of a message from a dictionary.

The `dict` properties are the message field names, and the values are the corresponding values to set in the message.

<!-- what if you set invalid fields that aren't in the message? -->

The method is used as shown below:


```python
message.set_from_dict({
    'my_numeric_field': 1,
    'my_char_array_field': 'Hello world',
    'my_float_array_field': [1.0, 2.0, 3.0],
    'my_int_array_field': [4, 5, 6]
})
```

**Arguments**:

- `fields` _dict_ - A dictionary for setting the corresponding `Message` field values.

<a id="libmav.Message.to_dict"></a>

#### to\_dict

```python
def to_dict()
```

Returns a dictionary of the fields in the message.

The dictionary keys are the message field names, and the values are their corresponding field values.


```python
# Get fields as python dict
message_dict = message.to_dict()
```

**Returns**:

- `dict` - A `dict` of the message fields and their current values.

<a id="libmav.Message.type"></a>

#### type

```python
def type()
```

Returns a high level `MessageDefinition`.

Some of the information such as name and id can be obtained from the `Message` itself.
The returned definition does provide additional information, such as the CRC_EXTRA.

**Returns**:

- ``MessageDefinition`` - High level message information, such as the id, name, CRC extra andso on.

<a id="libmav.MessageDefinition"></a>

## MessageDefinition Objects

```python
class MessageDefinition()
```

Message name, id, CRC extra and other high level informaton.

The message definition provides information such as the id, name and type of the message, along with the [CRC_EXTRA](https://mavlink.io/en/guide/serialization.html#crc_extra).

It is returned by the `Message.type()` method.

**Attributes**:

- `crc_extra` _int_ - The [CRC_EXTRA](https://mavlink.io/en/guide/serialization.html#crc_extra) for the message.
- `id` _int_ - Message id number
- `name` _string_ - Name of associated message, such as "HEARTBEAT"
- `max_buffer_length` _int_ - The size of buffer for the whole message.
- `max_payload_size` _int_ - The size of the payload part of the message, in bytes.

<a id="libmav.MessageDefinition.field_names"></a>

#### field\_names

```python
def field_names()
```

Returns a list of the payload field names in the message.

This can be iterated, as shown


```python
for fieldname in message.type.field_names():
    print(f"  {fieldname}")
```

**Returns**:

- `list` - A list of all the field names in the message.

<a id="libmav.MessageDefinition.keys"></a>

#### keys

```python
def keys()
```

Returns a list of the payload keys in the message (the same information as `field_names()`).

**Returns**:

- `list` - A list of all the field names in the message.

<a id="libmav.MessageSet"></a>

## MessageSet Objects

```python
class MessageSet()
```

A class representing a set of MAVLink message and enum definitions.

The object can be be populated from a MAVLink XML definition file, in which case it will automatically pull in any included XML definitions.
It can also be populated from a string containing an XML definition.

`MessageSet` can be used to create `Message` instances for sending, such as the `HEARTBEAT`.
It is also passed to the `NetworkRuntime` on construction, providing the definitions that libmav uses in order to parse MAVLink messages.

```python
# Construct the common message set
message_set = libmav.MessageSet('./mavlink/message_definitions/v1.0/common.xml')

# Check the id and create a message from it
message_id = heartbeat_message.id_for_message('HEARTBEAT')
heartbeat_message = message_set.create(message_id)

# Create a HEARTBEAT message from its name.       
heartbeat_message = message_set.create('HEARTBEAT')  
```

Note that the new messages are created with default values.

<a id="libmav.MessageSet.__init__"></a>

#### \_\_init\_\_

```python
def __init__()
```

Construct an empty `MessageSet`.


Definitions can be added to the new set using `add_from_xml_file()` or `add_from_xml_string()`.

```python
# Construct an empty message set
message_set = libmav.MessageSet()
```

**Returns**:

- `MessageSet` - An empty `MessageSet`.

<a id="libmav.MessageSet.__init__"></a>

#### \_\_init\_\_

```python
def __init__(definition_file)
```

Construct an empty `MessageSet`.

Additional definitions can be added to the new set using `add_from_xml_file()` or `add_from_xml_string()`.
<!-- what happens if there are clashes/overloads?? -->


```python
# Construct the common message set
message_set = libmav.MessageSet('./mavlink/message_definitions/v1.0/common.xml')

# Construct the ardupilotmega message set. 
message_set = libmav.MessageSet('./mavlink/message_definitions/v1.0/ardupilotmega.xml')
```

**Arguments**:

- `definition_file` _str_ - Full path to the XML definition file to load.
  

**Returns**:

- `MessageSet` - A new `MessageSet` object populated with the definitions in the indicated file.

<a id="libmav.MessageSet.add_from_xml_file"></a>

#### add\_from\_xml\_file

```python
def add_from_xml_file(definition_file)
```

Add definitions from a MAVLink XML definition file.

The new definitions are appended to the  to the `MessageSet`.
Overloaded definitions are replaced.


```python
# Construct an empty message set
message_set = libmav.MessageSet()

# Add the common message set
message_set.add_from_xml_file('./mavlink/message_definitions/v1.0/common.xml')
```

**Arguments**:

- `definition_file` _string_ - Full path to the XML definition file to load.

<a id="libmav.MessageSet.add_from_xml_string"></a>

#### add\_from\_xml\_string

```python
def add_from_xml_string(xml_string)
```

Add XML definitions defined in a string.

The new definitions are appended to the `MessageSet`.
Overloaded definitions are replaced. <!-- is this correct? -->

The example code below shows the approach.


```python
message_set.add_from_xml_string('''
  <mavlink>
    <messages>
      <message>
        ...
      </message>
    </messages>
  </mavlink>''')
```

**Arguments**:

- `xml_string` _str_ - Definition for XML messages and enums.

<a id="libmav.MessageSet.create"></a>

#### create

```python
def create(message_name)
```

Creates a `Message` instance from a message name.

This is used to create messages for sending (with `Connection.send()`) from a definition in the `MessageSet`.

The name must already be present (have been added to) the `MessageSet`.

For example, to create a [HEARTBEAT](https://mavlink.io/en/messages/common.html#HEARTBEAT) message you might use the following code:


```python
# Construct the common message set.
# This includes the `HEARTBEAT` message definition.
message_set = libmav.MessageSet('./mavlink/message_definitions/v1.0/common.xml')

# Create a HEARTBEAT message
heartbeat_message = message_set.create('HEARTBEAT')

# Note that the message will have default values for fields.
```

**Arguments**:

- `message_name` _str_ - The name of the message to create.
  

**Returns**:

- `Message` - The new `Message` instance.

<a id="libmav.MessageSet.create"></a>

#### create

```python
def create(message_id)
```

Creates a `Message` instance from a message id number.

This is used to create messages for sending (with `Connection.send()`) from a definition in the `MessageSet`.

The id must already be present (have been added to) the `MessageSet`.

For example, to create a [ATTITUDE_TARGET](https://mavlink.io/en/messages/common.html#ATTITUDE_TARGET) message with ID of 83, you can use the following code:


```python
# Construct the common message set.
# This includes the `ATTITUDE_TARGET` message definition.
message_set = libmav.MessageSet('./mavlink/message_definitions/v1.0/common.xml')

# Create a ATTITUDE_TARGET message
message = message_set.create(83)

# Note that the message will have default values for fields.
```

**Arguments**:

- `message_id` _str_ - The id of the message to create.
  

**Returns**:

- `Message` - The new `Message` instance.

<a id="libmav.MessageSet.id_for_message"></a>

#### id\_for\_message

```python
def id_for_message(message_name)
```

Gets the id of a message from its name.

The message name must already be present in the `MessageSet`.

For example, to get the ID of [ATTITUDE_TARGET](https://mavlink.io/en/messages/common.html#ATTITUDE_TARGET):


```python
# Construct the common message set.
# This includes the `ATTITUDE_TARGET` message definition.
message_set = libmav.MessageSet('./mavlink/message_definitions/v1.0/common.xml')

# Create a HEARTBEAT message
attitude_target_id = message_set.message_set('ATTITUDE_TARGET')

# Value should be 83
```

**Arguments**:

- `message_name` _str_ - The name of a message.
  

**Returns**:

- `int` - The message id corresponding to the `message_name`.

<a id="libmav.MessageSet.enum"></a>

#### enum

```python
def enum(enum_value_name)
```

Gets the id value for a specified enum value from its name.

The enum value name must already be present in the `MessageSet`.

This is particularly useful when you need to set an enum value in a field.
For example, when sending a command, you have to specify the command id a a number (commands are defined as enum value of the enum `MAV_CMD`).


```python
commandId = message_set.enum(`MAV_CMD_REQUEST_MESSAGE`)
# Would be 512
```

**Arguments**:

- `enum_value_name` _str_ - The enum value name for which the value is required.
  

**Returns**:

- `int` - The value of the indicated enum value.

<a id="libmav.NetworkInterface"></a>

## NetworkInterface Objects

```python
class NetworkInterface()
```

Abstract class representing a physical interface.

Implementations include [UDPClient](#libmav.UDPClient), [UDPServer](#libmav.UDPServer), [TCPClient](#libmav.TCPClient), [TCPServer](#libmav.TCPServer), and [Serial](#libmav.Serial).

<a id="libmav.NetworkRuntime"></a>

## NetworkRuntime Objects

```python
class NetworkRuntime()
```

NOT DONE An object for managing the fundaments properties of the MAVLink network connection, including connecting over a particular interface, and setting the MAVLink address and HEARTBEAT of the libmav application.

The object initializers allow you to specify the set of messages (`MessageSet`) that can be sent and received by the libmav application, and the physical connection to use (for example, you might use a `UDPServer` or `TCPServer` to listen for MAVLink traffic from an autopilot, or a `UDPClient` or `TCPClient` to initiate a connection).
You can also optionally specify the MAVLink system and component id of the libmav application, which will then be included in every message sent by the libmav application.
If not set these IDs both default to a value 97.

MAVLink components on a network advertise their presence by streaming a HEARTBEAT message with type and status information (norminally at 1 Hz).
You can optionally set a HEARTBEAT message to stream in the initializer, later set/update it using `set_heartbeat_message()`, or clear it using `clear_heartbeat_message()`
This API makes it trivially easy to set up a HEARTBEAT, and in most cases you would want to use this approach.

Once you have a `NetworkRuntime` object you can use its `await_connection()` method to wait for a connection, with a user-specified timeout.
If any MAVLink traffic is detected on the interface, the method returns a `Connection` object that you can use to send and receive messages.
There are also `on_connection()` and `on_connection_lost()` that can be used to asyncronously monitor for connections being regained/lost.

The example below shows how you might create the network runtime that specifies a HEARTBEAT and uses the default MAVLink address.
It then uses `await_connection()` to return the `Connection`.

```python
# Create the "common.xml" message set from a mavlink xml file
common_messages = libmav.MessageSet('./mavlink/message_definitions/v1.0/common.xml')

# Define a heartbeat message from the MessageSet
heartbeat_message = message_set.create('HEARTBEAT')
# Set properties of the HEARTBEAT, in this case using a dict
heartbeat_dict = { 
"type": message_set.enum("MAV_TYPE_GCS"),
"autopilot": message_set.enum("MAV_AUTOPILOT_GENERIC"), 
"base_mode": 0,
"custom_mode": 0,
"system_status": message_set.enum("MAV_STATE_ACTIVE"),
"mavlink_version": 2,
}
heartbeat_message.set_from_dict(heartbeat_dict)

# Create the a UDP server interface object that listens for MAVLink traffic.
connection_port = 14551
physical_interface = libmav.UDPServer(connection_port)

# Create the runtime object
connection_runtime = libmav.NetworkRuntime(common_messages, heartbeat_message, physical_interface)

# Wait for a connection with a 5 second timeout.
connection = connection_runtime.await_connection(5000)
```

If you wanted to also set the MAVLink address you could do so by modifying the constructor call to:

```python
# Create the runtime object
connection_runtime = libmav.NetworkRuntime({253, 1}, common_messages, heartbeat_message, physical_interface)
```
<!-- line above needs to be checked -->

<a id="libmav.NetworkRuntime.__init__"></a>

#### \_\_init\_\_

```python
def __init__(own_mavlink_id, message_set, interface)
```

Create a Connection object with a specified MAVLink address that does not automatically stream a HEARTBEAT message.

<!-- Example code ?? -->

**Arguments**:

- `own_mavlink_id` _Identifier_ - The MAVLink address (system id and component id) of this libmav node.
  This is included in all sent messages, including the HEARTBEAT.
- `message_set` _MessageSet_ - An object defining the set of messages
- `interface` _NetworkInterface_ - An object defining the physical connection that is being watched for MAVLink traffic.
  This is one of [UDPClient](#libmav.UDPClient), [UDPServer](#libmav.UDPServer), [TCPClient](#libmav.TCPClient), [TCPServer](#libmav.TCPServer), [Serial](#libmav.Serial).
  

**Returns**:

- `NetworkRuntime` - A new network runtime object that can be waited on for a connection.

<a id="libmav.NetworkRuntime.__init__"></a>

#### \_\_init\_\_

```python
def __init__(own_mavlink_id, message_set, heartbeat_message, interface)
```

Create a Connection object with a specified MAVLink address that streams the specified HEARTBEAT message.
<!-- Example code ?? -->

**Arguments**:

- `own_mavlink_id` _Identifier_ - The MAVLink address (system id and component id) of this libmav node.
  This is included in all sent messages, including the HEARTBEAT.
- `message_set` _MessageSet_ - An object defining the set of messages
- `heartbeat_message` _Message_ - The HEARTBEAT message to stream.
- `interface` _NetworkInterface_ - An object defining the physical connection that is being watched for MAVLink traffic.
  This is one of [UDPClient](#libmav.UDPClient), [UDPServer](#libmav.UDPServer), [TCPClient](#libmav.TCPClient), [TCPServer](#libmav.TCPServer), [Serial](#libmav.Serial).
  

**Returns**:

- `NetworkRuntime` - ?

<a id="libmav.NetworkRuntime.__init__"></a>

#### \_\_init\_\_

```python
def __init__(message_set, interface)
```

Create a Connection object with the default MAVLink address that does not stream a HEARTBEAT message.

The default MAVlink ID is set to: System ID = Component ID = 97.
<!-- Example code ?? -->


**Arguments**:

- `message_set` _MessageSet_ - An object defining the set of messages.
- `interface` _NetworkInterface_ - An object defining the physical connection that is being watched for MAVLink traffic.
  This is one of [UDPClient](#libmav.UDPClient), [UDPServer](#libmav.UDPServer), [TCPClient](#libmav.TCPClient), [TCPServer](#libmav.TCPServer), [Serial](#libmav.Serial).
  

**Returns**:

- `NetworkRuntime` - A new network runtime object that can be waited on for a connection.

<a id="libmav.NetworkRuntime.__init__"></a>

#### \_\_init\_\_

```python
def __init__(message_set, heartbeat_message, interface)
```

NOT DONE Create a Connection object with the default MAVLink address that streams the specified HEARTBEAT message.

<!-- Example code ?? -->

The default MAVlink ID is set to: System ID = Component ID = 97.

**Arguments**:

- `message_set` _MessageSet_ - An object defining the set of messages.
- `heartbeat_message` _Message_ - The HEARTBEAT message to stream.
- `interface` _NetworkInterface_ - An object defining the physical connection that is being watched for MAVLink traffic.
  This is one of [UDPClient](#libmav.UDPClient), [UDPServer](#libmav.UDPServer), [TCPClient](#libmav.TCPClient), [TCPServer](#libmav.TCPServer), [Serial](#libmav.Serial).
  

**Returns**:

- `NetworkRuntime` - A new network runtime object that can be waited on for a connection.

<a id="libmav.NetworkRuntime.await_connection"></a>

#### await\_connection

```python
def await_connection(timeout)
```

Wait for the remote end of network to connect, or timeout.

Note that a connection is established if _any_ decodable MAVLink message is received on the interface.
A connection is considered to be disconnected if no decodable message is received within three seconds.


```python
# Connect to a TCP server
conn_physical = libmav.TCPClient('192.168.1.12', 14550)
conn_runtime = libmav.NetworkRuntime(self.message_set, heartbeat, conn_physical)

# Return Connection or throw Exception
connection = conn_runtime.await_connection(2000)
```

**Arguments**:

- `timeout` _int_ - Time to wait for connection before raising exception, in milliseconds
  

**Returns**:

- `Connection` - An object representing a connection to the MAVLink network.

<a id="libmav.NetworkRuntime.clear_heartbeat_message"></a>

#### clear\_heartbeat\_message

```python
def clear_heartbeat_message()
```

Clear the heartbeat message (if set).

This stops streaming the heartbeat message, which may have been set in the class initializer or using `set_heartbeat_message()`.

```python
connection.clear_heartbeat_message()
```

<a id="libmav.NetworkRuntime.on_connection"></a>

#### on\_connection

```python
def on_connection(callbackFn)
```

Set a callback function that is called whenever a new `Connection` is detected.

A Connection is created if MAVLink traffic is detected from a unique remote address/port.

The callback function is called with the `Connection` object.
The identify of the unique `ConnectionPartner` associated with the `Connection` be determined using the `Connection.partner()` method.
Note that a `NetworkRuntime` constructed using a `TCPClient`, `UDPClient` or `Serial` network interface will only have one connection (because there is only one partner), while a `TCPServer` or `UDPServer` might have several remote systems connecting from different IP addresses.



```python
# Define a callback function
def callbackConnected(connection):
    print(f'New connection: {connection}')

conn_runtime.on_connection(callbackConnected)        
 ```

**Arguments**:

- `callbackFn` _method_ - Callback function that takes a `Connection` object as its argument.

<a id="libmav.NetworkRuntime.on_connection_lost"></a>

#### on\_connection\_lost

```python
def on_connection_lost()
```

Set a callback function that is called whenever a `Connection` is lost.

A connection is lost if no decodable MAVLink messages are detected from the corresponding connection partner within three seconds (i.e. it is no longer returning `true` for `Connection.alive()`).

The callback function is called with the `Connection` object for the lost connection.
The identify of the `ConnectionPartner` can be determined using the `Connection.partner()` method.


Note that if you lose a connection and want to recover it, you will need to recreate the physical interface and the `NetworkRuntime`, and then wait for the `Connection` again.

```python
# Define a callback function
def callbackDisconnected(connection):
    print(f'Disconnected: {connection}')

conn_runtime.on_connection_lost(callbackDisconnected)        
```

**Arguments**:

- `callbackFn` _method_ - Callback function that takes a `Connection` object as its argument.

<a id="libmav.NetworkRuntime.set_heartbeat_message"></a>

#### set\_heartbeat\_message

```python
def set_heartbeat_message(heartbeat_message)
```

Set a HEARTBEAT message to start streaming (or update the existing message).

All MAVLink systems should stream a heartbeat at 1 Hz, which is used to indicate that a component is connected, and how to route to it.
If supplied, libmav will automatically stream this message in a separate thread.

**Arguments**:

- `heartbeat_message` _Message_ - The HEARTBEAT message to stream.
  Updates/replaces existing streamed HEARTBEAT.

<a id="libmav.TCPClient"></a>

## TCPClient Objects

```python
class TCPClient()
```

Represents a TCP/IP socket connection to a port on a remote computer.

This type of object is used to connect to remote MAVLink connection partner that is acting as a TCP/IP server, and listening for connections on the indicated port.
The client must send a heartbeat, as the server is usually not initially sending MAVLink packets on the interface.

An object of this type may be passed to `NetworkRuntime`, as show below

```python
# Connect to a TCP client
conn_physical = libmav.TCPClient('192.168.1.12', 5760)

# Pass client connection as (last) argument.
conn_runtime = libmav.NetworkRuntime(self.message_set, heartbeat, conn_physical)
```

<a id="libmav.TCPClient.__init__"></a>

#### \_\_init\_\_

```python
def __init__(remote_address, remote_port)
```

Object initialiser

**Arguments**:

- `remote_address` _string_ - The IP address of the remote computer to connect to.
- `remote_port` _int_ - The port on remote computer to connect to.

<a id="libmav.TCPClient.close"></a>

#### close

```python
def close()
```

Close the TCPIP socket and clean up the (single) associated `ConnectionPartner` instance.

<a id="libmav.TCPServer"></a>

## TCPServer Objects

```python
class TCPServer()
```

Represents a physical connection for listening on a specified local TCPIP port for MAVLink traffic.

An object of this type may be passed to `NetworkRuntime` as shown.

```python
connection_port = 5760
physical_interface = libmav.TCPServer(connection_port)

# Create the runtime object
connection_runtime = libmav.NetworkRuntime(common_messages, heartbeat_message, physical_interface) 
```

Note that multiple remote computers might connect to this interface.
A separate `ConnectionPartner` instance is created for each, and made available to user code in the `NetworkRuntime.on_connection()` callback.

<a id="libmav.TCPServer.__init__"></a>

#### \_\_init\_\_

```python
def __init__(local_port)
```

Object initialiser

**Arguments**:

- `local_port` _int_ - The port on local computer to connect to.

<a id="libmav.TCPServer.close"></a>

#### close

```python
def close()
```

Close the current physical connection.

Close the socket connection and clean up any `ConnectionParnter` instances associated with it.

<a id="libmav.UDPClient"></a>

## UDPClient Objects

```python
class UDPClient()
```

Represents a UDP socket connection to a port on a remote computer.

This type of object is used to connect to remote MAVLink connection partner that is acting as a UDP server, and listening for connections.
The client must send a heartbeat, as the server is usually not initially sending MAVLink packets on the interface.

An object of this type may be passed to `NetworkRuntime`.

<a id="libmav.UDPClient.__init__"></a>

#### \_\_init\_\_

```python
def __init__(remote_address, remote_port)
```

Construct the UDPClient object, specifing the remote address and port.

**Arguments**:

- `remote_address` _string_ - The IP address of the remote computer to connect to.
- `remote_port` _int_ - The port on remote computer to connect to.

<a id="libmav.UDPClient.close"></a>

#### close

```python
def close()
```

Close the UDP socket and clean up the (single) associated `ConnectionPartner` instance.

<a id="libmav.UDPServer"></a>

## UDPServer Objects

```python
class UDPServer()
```

Represents a physical connection for listening on a specified local UDP port for MAVLink traffic.

An object of this type may be passed to `NetworkRuntime` as shown.

```python
# Create the a UDP server interface object that listens for MAVLink traffic.
connection_port = 14551
physical_interface = libmav.UDPServer(connection_port)
```

Multiple remote computers might connect to this interface.
A separate `ConnectionPartner` instance is created for each, and made available to user code in the `NetworkRuntime.on_connection()` callback.
Note that `NetworkRuntime.await_connection()` only waits for the first connection: if you don't use `on_connection()` the other `ConnectionPartner` instances will be inaccessible.

<a id="libmav.UDPServer.__init__"></a>

#### \_\_init\_\_

```python
def __init__(local_port)
```

Construct a UDPServer object, specifing a local UDP port.

**Arguments**:

- `local_port` _int_ - The port on the local computer to connect to.

<a id="libmav.UDPServer.close"></a>

#### close

```python
def close()
```

Close the current physical connection.

Close the socket connection and clean up any `ConnectionPartner` instances associated with it.

<a id="libmav.Serial"></a>

## Serial Objects

```python
class Serial()
```

Represents a connection for listening on a specified serial port for MAVLink traffic.
    <!-- does it listen or ping, or both? How do we describe this-->

    An object of this type may be passed to `NetworkRuntime` in order to define the physical connection is a serial port/UART.

    For example, to define a `Serial` network interface for a Linux computer (e.g. RasPi) connected to the vehicle via Serial portR:

    ```python
    device_name = '/dev/ttyAMA0'
    baud_rate = 57600
    physical_interface = libmav.Serial(device_name, baud_rate, false)
    ```

To use this interface you then pass it to a `NetworkRuntime` with a HEARTBEAT, and then call `NetworkRuntime.awaitConnection()` (on both ends of the connection).
This is the same pattern as for `TCPClient` and `UDPClient`.

<a id="libmav.Serial.__init__"></a>

#### \_\_init\_\_

```python
def __init__(device, baudrate, flow_control)
```

Construct a Serial port object, specifing the device name, baud rate, and whether flow control is supported.

**Arguments**:

- `device` _string_ - A string indicating the serial port to connect to.
- `baudrate` _int_ - The baud rate for the connection.
- `flow_control` _bool_ - `true` if flow control is enabled on the port, `false` otherwise.

<a id="libmav.Serial.close"></a>

#### close

```python
def close()
```

Close the current physical connection (a serial port).

Close the serial port and remove the associated `ConnectionPartner` instance.

