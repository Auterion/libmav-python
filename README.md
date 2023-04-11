# Libmav Python

Native python bindings for the [libmav](https://github.com/ThomasDebrunner/libmav) library.

## Installation

```
pip install libmav
```

## Example usage

```python
import libmav

# Create a message set from a mavlink xml file
message_set = libmav.MessageSet('<PATH TO common.xml>')
# You can also add additional messages here with inline xml
message_set.add_from_xml_string('''
<mavlink>
    <messages>
        <message>
            ...
        </message>
    </messages>
</mavlink>''')

# Create a message from the message set
message = self.message_set.create('MY_PROTOCOL_MESSAGE')

# Set fields of the message individually
message['my_numeric_field'] = 1
message['my_char_array_field'] = 'Hello world'
message['my_float_array_field'] = [1.0, 2.0, 3.0]
message['my_int_array_field'] = [4, 5, 6]

# Get fields individually
my_num = message['my_numeric_field']
my_string = message['my_char_array_field']
my_float_array_field = message['my_float_array_field']

# You can also use pythonic things like list comprehensions
    # This creates a list of all the values in my_float_array_field multiplied by 2
my_float_array_field = [x * 2 for x in message['my_float_array_field']]

# Set fields from a dict
message.set_from_dict({
            'my_numeric_field': 1,
            'my_char_array_field': 'Hello world',
            'my_float_array_field': [1.0, 2.0, 3.0],
            'my_int_array_field': [4, 5, 6]
        })


# Get fields as python dict
message_dict = message.to_dict()


# Connect to a TCP server
conn_physical = libmav.TCPClient('192.168.1.12', 14550)
conn_runtime = libmav.NetworkRuntime(self.message_set, heartbeat, client_physical)

connection = conn_runtime.await_connection(2000)

# Check if connection is still alive
if not connection.alive():
    print('Connection lost, waiting for reconnect...')
    connection = conn_runtime.await_connection(2000)

# Send a message
connection.send(message)

# Receive a message, timeout 1s
received_message = connection.receive("HEARTBEAT", 1000)

# Receive a message, no timeout
received_message = connection.receive("HEARTBEAT")

# Receive a message as the result of a message we send
# This avoids the race condition between us sending and then waiting for reception
expectation = connection.receive("PARAM_VALUE")
connection.send(our_param_request_message)
received_message = connection.receive(expectation, 1000)


# Connect to a Serial port
conn_physical = libmav.TCPClient('/dev/ttyUSB0', 14550)
conn_runtime = libmav.NetworkRuntime(self.message_set, heartbeat, client_physical)

```

## Install from source

### Prerequisites

* A compiler with C++11 support
* Pip 10+ or CMake >= 3.4 (or 3.14+ on Windows, which was the first version to support VS 2019)
* Ninja or Pip 10+

Just clone this repository and pip install. Note the `--recursive` option which is
needed for the pybind11 submodule:

```bash
git clone --recursive https://github.com/Auterion/libmav-python.git
pip install ./libmav-python
```

With the `setup.py` file included in this project, the `pip install` command will
invoke CMake and build the pybind11 module as specified in `CMakeLists.txt`.